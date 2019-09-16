#ifndef __TYPED_POOL_ALLOCATOR_H_
#define __TYPED_POOL_ALLOCATOR_H_

#include <cstdint>
#include <memory/pool_allocator.hpp>

namespace allocators
{
template <typename T, typename... TArgs>
class typed_pool_allocator
{
public:
    typed_pool_allocator(std::uint32_t num_elements)
    {
        auto size_bytes = num_elements * (_size);
        raw_allocator.init_aligned(size_bytes, _size, _alignment);
    }

    T *allocate(TArgs... ctr_params)
    {
        auto raw_ptr = raw_allocator.allocate();
        auto t_ptr = new (raw_ptr) T(ctr_params...);
        return t_ptr;
    }

    // T *allocate()
    // {
    //     auto raw_ptr = raw_allocator.allocate();
    //     auto t_ptr = new (raw_ptr) T();
    //     return *t_ptr;
    // }

    void free(T *element)
    {
        raw_allocator.free_element((void *)element);
    }

    void free_pool()
    {
        raw_allocator.free_pool();
    }

private:
    std::uint32_t _size = sizeof(T);
    std::uint32_t _alignment = alignof(T);
    pool_allocator raw_allocator;
};
} // namespace allocators

#endif
