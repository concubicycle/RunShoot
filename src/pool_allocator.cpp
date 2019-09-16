#include "memory/pool_allocator.hpp"

allocators::pool_allocator::pool_allocator()
{
	_alignment = 1;
}
allocators::pool_allocator::~pool_allocator(void)
{
}

void allocators::pool_allocator::init(std::uint32_t size_bytes, std::uint32_t chunk_size)
{
	if (size_bytes == 0 || chunk_size == 0)
		return;

	_chunk_size = chunk_size;
	_chunk_capacity = (size_bytes / chunk_size) - 1;

	mem_block = (std::uint8_t *)malloc(size_bytes + _chunk_size); // extra chunk

	prepare_memory(mem_block, _chunk_capacity);

	mem_block = head = mem_block;

	_mem_blocks.emplace_back(mem_block, size_bytes, 0);

	_size = 0;
	return;
}
void allocators::pool_allocator::init_aligned(std::uint32_t size_bytes, std::uint32_t chunkSize, std::uint32_t alignment)
{
	if (size_bytes == 0 || chunkSize == 0)
		return;

	_chunk_size = chunkSize;
	_next_expand = size_bytes;
	_chunk_capacity = (size_bytes / chunkSize) - 1;
	_alignment = alignment;

	//Determine total amount of memory to allocate
	std::uint32_t expandedSize_bytes = size_bytes + alignment;

	//Use non-alligned alloc to get memory
	auto raw = std::uint64_t(malloc(expandedSize_bytes + _chunk_size));

	//Calculate adjustment by masking off the lower bits of the address,
	//to determine how 'misa5ligned' it is.
	std::uint64_t mask = (alignment - 1);
	std::uint64_t misalignment = (raw & mask);
	std::uint64_t adjustment = misalignment > 0
								   ? _alignment - misalignment
								   : 0;

	//Calculate the adjusted address, and return as a pointer
	std::uint64_t aligned_address = raw + adjustment;

	//allocate a pointer to the first memory block.
	_mem_blocks.emplace_back((std::uint8_t *)aligned_address, expandedSize_bytes - adjustment, adjustment);

	prepare_memory(_mem_blocks[0].mem, _chunk_capacity);

	mem_block = head = _mem_blocks[0].mem;
	_size = 0;
	return;
}

/*prepare a recently allocated chunk of memory for use as a pool by setting each chunk
to point to the next chunk, and the last one to point to NULL.*/
void allocators::pool_allocator::prepare_memory(std::uint8_t *mem, std::uint32_t num_chunk)
{
	for (std::uint32_t chunk_index = 0; chunk_index < num_chunk; ++chunk_index)
	{
		std::uint8_t *chunk = mem + (chunk_index * _chunk_size);
		*((std::uint8_t **)chunk) = chunk + _chunk_size;
	}

	*((std::uint8_t **)&mem[num_chunk * _chunk_size]) = 0; /* terminating NULL */
}

//TODO: ASSERT AGAINST EVERY POSSIBLE FAILURE OF THIS METHOD PLEASE
void allocators::pool_allocator::linked_expand(std::uint32_t size_bytes)
{
	std::uint8_t *new_block;
	std::uint32_t count = (size_bytes / _chunk_size) - 1;
	std::uint32_t adjustment = 0;
	std::uint32_t expanded_bytes = size_bytes;
	auto malloc_size = expanded_bytes + _chunk_size;

	if (_alignment != 0)
	{
		//Determine total amount of memory to allocate
		expanded_bytes += _alignment;

		//Use non-alligned alloc to get memory
		auto raw = std::uint64_t(malloc(malloc_size));

		//Calculate adjustment by masking off the lower bits of the address,
		//to determine how 'misaligned' it is.
		std::uint64_t mask = (_alignment - 1);
		std::uint64_t misalignment = (raw & mask);
		adjustment = misalignment > 0
						 ? _alignment - misalignment
						 : 0;

		//Calculate the adjusted address, and return as a pointer
		std::uint64_t aligned = raw + adjustment;
		new_block = (std::uint8_t *)aligned;
	}
	else
	{
		new_block = (std::uint8_t *)malloc(size_bytes + _chunk_size);
	}

	prepare_memory(new_block, count);

	//create a new memblock structure
	_mem_blocks.emplace_back(new_block, expanded_bytes - adjustment, adjustment);

#ifdef __PA_DEBUG_OUTPUT_
	std::cout << "added new block: " << std::endl
			  << _mem_blocks.back().to_string() << std::endl
			  << std::endl;
#endif

	std::uint8_t *last_memblock = _mem_blocks[_mem_blocks.size() - 2].mem;
	std::uint32_t last_mem_block_size = _mem_blocks[_mem_blocks.size() - 2].size;

	//link memory blocks
	*((std::uint8_t **)&last_memblock[last_mem_block_size]) = new_block;

	_chunk_capacity += count;

	if (!head)
		head = new_block;
}

void *allocators::pool_allocator::allocate()
{
	if (!head)
	{
		_next_expand *= 2;

#ifdef __PA_DEBUG_OUTPUT_
		std::cout << std::endl
				  << "reallocing (" << _next_expand << ")..."
				  << std::endl;
#endif

		linked_expand(_next_expand);

#ifdef __PA_DEBUG_OUTPUT_
		std::cout << "All Memblock Info:" << std::endl
				  << "===============" << std::endl
				  << all_memblock_info() << std::endl
				  << "===============" << std::endl;
#endif
	}

	std::uint8_t *current_block = head;
	head = (*((std::uint8_t **)(head)));

	_size++;
	return current_block;
}
void allocators::pool_allocator::free_element(void *ptr)
{
	if (!ptr)
		return;

	*((std::uint8_t **)ptr) = head;
	head = (std::uint8_t *)ptr;

	_size--;
}

void allocators::pool_allocator::free_pool()
{
	std::uint64_t malloc_ret_address;

	for (auto block : _mem_blocks)
	{
		malloc_ret_address = (std::uint64_t)(block.mem) - (std::uint64_t)block.offset;

#ifdef __PA_DEBUG_OUTPUT_
		std::cout << "Freeing " << malloc_ret_address << std::endl;
#endif

		free((std::uint8_t *)(malloc_ret_address));
	}

	_mem_blocks.clear();
}

#ifdef __PA_DEBUG_OUTPUT_
std::string allocators::pa_memblock::to_string()
{
	auto mem_int = std::uint64_t(mem);
	std::stringstream stream;
	stream << "mem: " << mem_int << std::endl
		   << "size: " << size << std::endl
		   << "offset: " << offset << std::endl
		   << "actual mem (returned by malloc):" << (mem_int - offset);

	return stream.str();
}

std::string allocators::pool_allocator::all_memblock_info()
{
	std::stringstream stream;

	for (auto mb : _mem_blocks)
		stream << mb.to_string() << std::endl
			   << "-----------" << std::endl;

	return stream.str();
}
#endif