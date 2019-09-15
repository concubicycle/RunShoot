#include "pool_allocator.hpp"


allocators::pool_allocator::pool_allocator()
{
	m_alignment = 1;
}
allocators::pool_allocator::~pool_allocator(void)
{
}

void allocators::pool_allocator::init(std::uint32_t size_bytes, std::uint32_t chunk_size)
{
	if (size_bytes == 0 || chunk_size == 0)
		return;

	_chunk_size = chunk_size;
	m_capacity = (size_bytes / chunk_size) - 1;

	mem_block = (std::uint8_t*)malloc(size_bytes);

	prepare_memory(mem_block, m_capacity);

	mem_block = head = mem_block;
		
	_mem_block_count = 1;
	blocks = (pool_allocator_memblock*)malloc(sizeof(pool_allocator_memblock));

	//init new memblock
	blocks[0].mem = mem_block;
	blocks[0].size = size_bytes;
	blocks[0].offset = 0;

	m_size = 0;
	std::cout << "Initialized pool allocator, from mem address " << (std::uint32_t)mem_block << " to " << (std::uint32_t)(mem_block + (m_capacity * _chunk_size)) << std::endl;
	return;
}
void allocators::pool_allocator::initAligned(std::uint32_t size_bytes, std::uint32_t chunkSize, std::uint32_t alignment)
{
	if (size_bytes == 0 || chunkSize == 0)
		return;

	_chunk_size = chunkSize;
	m_capacity = (size_bytes / chunkSize) - 1;
	m_alignment = alignment;

	//Determine total amount of memory to allocate
	std::uint32_t expandedSize_bytes = size_bytes + alignment;

	//Use non-alligned alloc to get memory
	std::uint32_t rawAddress = std::uint32_t(malloc(expandedSize_bytes));

	//Calculate adjustment by masking off the lower bits of the address,
	//to determine how 'misaligned' it is.
	std::uint32_t mask = (alignment - 1);
	std::uint32_t misalignment = (rawAddress & mask);
	std::uint32_t adjustment = alignment - misalignment;

	//Calculate the adjusted address, and return as a pointer
	std::uint32_t aligned_address = rawAddress + adjustment;

	//allocate a pointer to the first memory block.
	blocks = (pool_allocator_memblock*)malloc(sizeof(pool_allocator_memblock));
	_mem_block_count = 1;

	//init the memblock
	blocks[0].mem = (std::uint8_t*)aligned_address;
	blocks[0].size = expandedSize_bytes - adjustment;
	blocks[0].offset = adjustment;

	prepare_memory(blocks[0].mem, m_capacity);

	mem_block = head = blocks[0].mem;
	m_size = 0;
	return;
}

/*prepare a recently allocated chunk of memory for use as a pool by setting each chunk
to point to the next chunk, and the last one to point to NULL.*/
void allocators::pool_allocator::prepare_memory(std::uint8_t* mem, std::uint32_t numberOfChunks)
{
	for (std::uint32_t chunkIndex = 0; chunkIndex < numberOfChunks; ++chunkIndex)
	{
		std::uint8_t* currChunk = mem + (chunkIndex * _chunk_size);
		*((std::uint8_t**)currChunk) = currChunk + _chunk_size;
	}

	*((std::uint8_t**)&mem[numberOfChunks * _chunk_size]) = 0; /* terminating NULL */
}


//TODO: ASSERT AGAINST EVERY POSSIBLE FAILURE OF THIS METHOD PLEASE
void allocators::pool_allocator::linkedExpand(std::uint32_t size_bytes)
{
	std::uint8_t* new_block;
	std::uint32_t count = (size_bytes / _chunk_size) - 1;
	std::uint32_t adjustment = 0;
	std::uint32_t expandedSize_bytes = size_bytes;

	if (m_alignment != 0)
	{
		//Determine total amount of memory to allocate
		std::uint32_t expanded_bytes = size_bytes + m_alignment;

		//Use non-alligned alloc to get memory
		std::uint32_t rawAddress = std::uint32_t(malloc(expanded_bytes));

		//Calculate adjustment by masking off the lower bits of the address,
		//to determine how 'misaligned' it is.
		std::uint32_t mask = (m_alignment - 1);
		std::uint32_t misalignment = (rawAddress & mask);
		adjustment = m_alignment - misalignment;

		//Calculate the adjusted address, and return as a pointer
		std::uint32_t alignedAddress = rawAddress + adjustment;
		new_block = (std::uint8_t*)alignedAddress;
	}
	else
		new_block = (std::uint8_t*)malloc(size_bytes);

	prepare_memory(new_block, count);
	std::cout << "Expanded pool allocator, linked to mem address " << (std::uint32_t)new_block << " to " << (std::uint32_t)(new_block + (count * _chunk_size)) << std::endl;

	//create a new memblock structure 
	_mem_block_count++;
	blocks = (pool_allocator_memblock*)realloc(blocks, sizeof(pool_allocator_memblock) * _mem_block_count);

	//init new memblock
	blocks[_mem_block_count - 1].mem = new_block;
	blocks[_mem_block_count - 1].size = expandedSize_bytes - adjustment;
	blocks[_mem_block_count - 1].offset = adjustment;

	std::uint8_t* lastMemBlock = blocks[_mem_block_count - 2].mem;
	std::uint32_t last_mem_block_size = blocks[_mem_block_count - 2].size;

	//link memory blocks
	*((std::uint8_t**)&lastMemBlock[last_mem_block_size]) = new_block;

	m_capacity += count;

	if (!head)
		head = new_block;
}
void* allocators::pool_allocator::allocate()
{
	if (!head)
		return 0;

	std::uint8_t* curPtr = head;
	head = (*((std::uint8_t**)(head)));

	std::cout << "Returning address " << (std::uint32_t)curPtr << std::endl;
	if (head)
		std::cout << "head is now " << (std::uint32_t)head << std::endl;
	else
		std::cout << "head is now null, allocator out of memory" << std::endl;

	m_size++;
	return curPtr;
}
void allocators::pool_allocator::freeElement(void* ptr)
{
	if (!ptr)
		return;

	*((std::uint8_t**)ptr) = head;
	head = (std::uint8_t*)ptr;

	m_size--;
}

void allocators::pool_allocator::freePool()
{
	std::uint32_t malloc_ret_address;

	for (int i = 0; i < this->_mem_block_count; i++)
	{
		malloc_ret_address = (std::uint32_t)(blocks[i].mem) - blocks[i].offset;
		free((std::uint8_t*)(malloc_ret_address));
	}
	free(blocks);
}