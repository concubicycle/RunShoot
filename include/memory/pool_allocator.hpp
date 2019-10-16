/*Implementation based on Game Engine Programming*/

#ifndef __POOL_ALLOCATOR_H_
#define __POOL_ALLOCATOR_H_

#include <cstdint>
#include <stdlib.h>
#include <string>
#include <vector>

namespace allocators
{
typedef struct pa_mem_block
{
public:
    void set(void* original_ptr, void* aligned_ptr, std::uint32_t chunk_count)
    {
        this->original_pointer = original_ptr;
        this->aligned_pointer = aligned_ptr;
        this->chunk_count = chunk_count;
    }

    void *original_pointer = nullptr;
    void *aligned_pointer = nullptr;
    std::uint32_t chunk_count;

} pa_mem_block_t;

class pool_allocator
{
    // a hard limit on the number of memory blocks the allocator can keep track of.
    const static int MaxMemoryBlocks = 1024;

public:
	pool_allocator(std::uint32_t chunk_size, std::uint32_t chunk_count, uintptr_t alignment);
	pool_allocator(const pool_allocator& other) = delete;

	std::uint32_t size() const { return _total_allocated; }

	void *allocate();
	void free_element(void *ptr);
	void linked_expand(std::uint32_t size_bytes);
	void free_pool();


private:
    uintptr_t _alignment;

    std::uint32_t _chunk_size;
    std::uint32_t _total_allocated;
    std::uint32_t _next_expand;

    std::uint8_t *_head;

    pa_mem_block_t _memory_blocks[MaxMemoryBlocks];
    std::uint32_t _memory_block_count = 0;

    void prepare_memory(std::uint8_t *mem, std::uint32_t chunk_count);

    std::uint32_t track_mem_block(void* original_ptr, void* aligned_ptr, std::uint32_t chunk_count);
    void link_last_two_mem_blocks();
};

} // namespace allocators

#endif