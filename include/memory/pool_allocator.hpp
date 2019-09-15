/*Implementation based on Game Engine Programming*/

#ifndef __STACK_ALLOCATOR_H_
#define __STACK_ALLOCATOR_H_

#include <cstdint>
#include <stdlib.h>
#include <iostream>

namespace allocators
{
	typedef struct pool_allocator_memblock
	{
		std::uint8_t* mem;
		std::uint32_t size;
		std::uint32_t offset;
	} PAMemBlock;

	class pool_allocator
	{
	private:
		std::uint32_t _chunk_size;
		std::uint32_t m_size;
		std::uint32_t m_capacity;
		std::uint32_t m_alignment;

		//new:
		pool_allocator_memblock* blocks;
		std::uint16_t _mem_block_count;

		std::uint8_t* head;
		std::uint8_t* mem_block;
	
		void prepare_memory(std::uint8_t* mem, std::uint32_t num_chunks);
	public:
		pool_allocator();
		~pool_allocator(void);

		void init(std::uint32_t size_bytes, std::uint32_t chunk_size);
		void initAligned(std::uint32_t size_bytes, std::uint32_t chunkSize, std::uint32_t alignment);

		void linkedExpand(std::uint32_t size_bytes);

		//A FLEXIBLE IMPLEMENTATION OF THIS REQUIRES SMART PTRS?
		//void expand(std::uint32_t size_bytes);

		//WARNING: ALLIGNED ALLOCATE METHOD MAY BE NEEDED!!!
		void* allocate();

		void freeElement(void* ptr);
		std::uint32_t size() { return m_size; }
		std::uint32_t capacity() { return m_capacity; }

		void freePool();
	};

#endif
}

#endif