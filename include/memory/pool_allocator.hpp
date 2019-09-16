/*Implementation based on Game Engine Programming*/

#ifndef __POOL_ALLOCATOR_H_
#define __POOL_ALLOCATOR_H_

#define __PA_DEBUG_OUTPUT_

#include <cstdint>
#include <stdlib.h>
#include <string>
#include <vector>

#ifdef __PA_DEBUG_OUTPUT_
#include <iostream>
#include <sstream>
#endif

namespace allocators
{
typedef struct pool_allocator_memblock
{
	std::uint8_t *mem;
	std::uint32_t size;
	std::uint32_t offset;

	pool_allocator_memblock(
		std::uint8_t *mem,
		std::uint32_t size,
		std::uint32_t offset) : mem(mem), size(size), offset(offset) {}

#ifdef __PA_DEBUG_OUTPUT_
	std::string to_string();
#endif
} pa_memblock;

class pool_allocator
{
private:
	std::uint32_t _chunk_size;
	std::uint32_t _size;
	std::uint32_t _chunk_capacity;
	std::uint64_t _alignment;

	std::uint32_t _next_expand;

	//new:
	std::vector<pa_memblock> _mem_blocks;

	std::uint8_t *head;
	std::uint8_t *mem_block;

	void prepare_memory(std::uint8_t *mem, std::uint32_t num_chunks);

public:
	pool_allocator();
	~pool_allocator(void);

	std::uint32_t capacity() { return _chunk_capacity; }
	std::uint32_t size() { return _size; }
	void *allocate();
	void free_element(void *ptr);
	void init_aligned(std::uint32_t size_bytes, std::uint32_t chunkSize, std::uint32_t alignment);
	void init(std::uint32_t size_bytes, std::uint32_t chunk_size);
	void linked_expand(std::uint32_t size_bytes);

	void free_pool();

#ifdef __PA_DEBUG_OUTPUT_
	std::string all_memblock_info();
#endif
};

} // namespace allocators

#endif