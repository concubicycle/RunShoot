#include <cstring>
#include "memory/pool_allocator.hpp"

allocators::pool_allocator::pool_allocator(
        std::uint32_t chunk_size,
        std::uint32_t chunk_count,
        uintptr_t alignment) :
        _chunk_size(chunk_size > sizeof(void*) ? chunk_size : sizeof(void*)),
        _next_expand(chunk_size * chunk_count),
        _total_allocated(0),
        _alignment(alignment)
{
    if (_chunk_size == 0 || chunk_count == 0)
        return;

    std::uint32_t size_bytes = chunk_size * chunk_count;

    // Determine total amount of memory to allocate
    // Get an extra chunk to be safe when we link chunks together.
    std::uint64_t expanded_bytes = size_bytes + _chunk_size + _alignment;

    // Use non-aligned alloc to get memory
    uintptr_t raw = (uintptr_t) malloc(expanded_bytes + _chunk_size);

    // Calculate adjustment by masking off the lower bits of the address,
    // to determine how 'misaligned' it is.
    uintptr_t mask = (alignment - 1);
    uintptr_t misalignment = raw & mask;
    uintptr_t adjustment = misalignment > 0
                               ? _alignment - misalignment
                               : 0;

    uintptr_t aligned_address = raw + adjustment;
    track_mem_block((std::uint8_t*)raw, (std::uint8_t*) aligned_address, chunk_count);
    prepare_memory((std::uint8_t *)aligned_address, chunk_count);
    _head = (std::uint8_t *)aligned_address;
}


void allocators::pool_allocator::linked_expand(std::uint32_t size_bytes)
{
    std::uint32_t chunk_count = (size_bytes / _chunk_size);
    std::uint64_t expanded_bytes = size_bytes + _chunk_size;
    std::uint8_t* new_block_aligned;
    std::uint8_t* new_block_raw;

    if (_alignment != 0)
    {
        expanded_bytes += _alignment;

        uintptr_t raw = (uintptr_t)malloc(expanded_bytes);

        //Calculate adjustment by masking off the lower bits of the address,
        //to determine how 'misaligned' it is.
        uintptr_t mask = (_alignment - 1);
        uintptr_t misalignment = (raw & mask);
        uintptr_t adjustment = misalignment > 0
                     ? _alignment - misalignment
                     : 0;


        new_block_aligned = (std::uint8_t*)(raw + adjustment);
        new_block_raw = (std::uint8_t*)raw;
    }
    else
    {
        new_block_raw = new_block_aligned = (std::uint8_t*)malloc(size_bytes + _chunk_size);
    }

    prepare_memory(new_block_aligned, chunk_count);

    std::uint32_t block_index = track_mem_block(new_block_raw, new_block_aligned, chunk_count);
    link_last_two_mem_blocks();
}


/*prepare a recently allocated chunk of memory for use as a pool by setting each chunk
to point to the next chunk, and the last one to point to NULL.*/
void allocators::pool_allocator::prepare_memory(std::uint8_t *mem, std::uint32_t chunk_count)
{
	for (std::uint32_t chunk_index = 0; chunk_index < chunk_count; ++chunk_index)
	{
		std::uint8_t *chunk = mem + (chunk_index * _chunk_size);
		*((std::uint8_t **)chunk) = chunk + _chunk_size;
	}

	std::uint32_t last_chunk_offset = chunk_count * _chunk_size;
	std::uint8_t* last_chunk_ptr = mem + last_chunk_offset;
	*((std::uint8_t **)last_chunk_ptr) = 0; /* terminating NULL */
}



void *allocators::pool_allocator::allocate()
{
	if (!_head)
	{
		_next_expand *= 2;
		linked_expand(_next_expand);
	}

	std::uint8_t *current_block = _head;
    _head = (*((std::uint8_t **)(_head)));

	_total_allocated++;
	return current_block;
}

void allocators::pool_allocator::free_element(void *ptr)
{
	if (!ptr)
		return;

	*((std::uint8_t **)ptr) = _head;
    _head = (std::uint8_t *)ptr;

	_total_allocated--;
}

void allocators::pool_allocator::free_pool()
{
	std::uint64_t malloc_ret_address;

	for (int i = 0; i < _memory_block_count; ++i)
    {
	    free(_memory_blocks[i].original_pointer);
    }
}

std::uint32_t allocators::pool_allocator::track_mem_block(
        void* original_ptr,
        void* aligned_ptr,
        std::uint32_t chunk_count)
{
    if (_memory_block_count == MaxMemoryBlocks)
    {
        throw "Pool allocator memory block exceeded. Implement a dynamically allocated auxiliary mem block array.";
    }

    _memory_blocks[_memory_block_count].set(original_ptr, aligned_ptr, chunk_count);
    return _memory_block_count++;
}

void allocators::pool_allocator::link_last_two_mem_blocks() {
    pa_mem_block_t& last_block = _memory_blocks[_memory_block_count - 2];
    pa_mem_block_t& this_block = _memory_blocks[_memory_block_count - 1];

    auto last_chunk_byte_offset = (last_block.chunk_count * _chunk_size);
    std::uint8_t * last_chunk_ptr = (std::uint8_t *)last_block.aligned_pointer + last_chunk_byte_offset;

    //link memory blocks
    *((std::uint8_t **)last_chunk_ptr) = (std::uint8_t *)this_block.aligned_pointer;


    if (!_head)
        _head = last_chunk_ptr;
}
