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
    typed_pool_allocator(std::uint32_t num_elements) :
        _size(sizeof(T)),
        _alignment(alignof(T)),
        _size_bytes(num_elements * (_size)),
        raw_allocator(_size, num_elements, _alignment)
    {
    }

    T *allocate(TArgs... ctr_params)
    {
        auto raw_ptr = raw_allocator.allocate();
        auto t_ptr = new (raw_ptr) T(ctr_params...);
        return t_ptr;
    }

    void free(T *element)
    {
        raw_allocator.free_element((void *)element);
    }

    void free_pool()
    {
        raw_allocator.free_pool();
    }

private:
    std::uint32_t _size;
    uintptr_t _alignment;
    std::uint32_t _size_bytes;
    pool_allocator raw_allocator;
};
} // namespace allocators

#endif
