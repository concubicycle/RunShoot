//
// Created by sava on 9/21/19.
//

#ifndef ECS_DEV_ARCHETYPE_POOL_H
#define ECS_DEV_ARCHETYPE_POOL_H

#include <memory/pool_allocator.hpp>
#include <unordered_map>
#include <tuple>
#include <memory>
#include <algorithm>
#include <mutex>
#include <functional>

#include <iostream>

namespace ecs
{

class archetype_pool
{
public:
    archetype_pool(
            std::uint32_t archetype_size,
            std::uint32_t num_archetypes,
            uintptr_t archetype_alignment)
            : _allocator(
                (std::uint32_t )archetype_size,
                (std::uint32_t)num_archetypes,
                (uintptr_t)archetype_alignment)
    {
    }

    archetype_pool(const archetype_pool& other) = delete;

    ~archetype_pool()
    {

        _allocator.free_pool();
    }

    template <class TTuple>
    std::shared_ptr<TTuple> allocate()
    {
        auto tupleMem = _allocator.allocate();
        new (tupleMem) TTuple(); // construct in place

        std::shared_ptr<TTuple> ptr(
            (TTuple *)tupleMem, [this](TTuple *dataPtr) -> void {
                _allocator.free_element(dataPtr);
                archetype_all<TTuple>(dataPtr);
            });

        archetype_all<TTuple>(nullptr).push_back(ptr);
        return ptr;
    }

    /**
     * A way to get a const reference to a vector of all archetypes of a type.
     *
     * @tparam TTuple TTuple TTuple This is the std::tuple that identifies the archetype (a combination of the component types).
     * @return a const reference to the archetype vector with all entities of this archetype.
     */
    template <typename TTuple>
    const std::vector<std::shared_ptr<TTuple>> &read_archetype_all()
    {
        return archetype_all<TTuple>(nullptr);
    }

    /**
     * A way of accessing all entities of an archetype that permits modifications to the vector.
     * A lambda should be passed, which will be run after locking a guard, which hopefully will
     * protect the archetype vector.
     *
     * @tparam TTuple TTuple This is the std::tuple that identifies the archetype (a combination of the component types).
     * @param operation A function to run, which will receive a non-const reference to the tuple vector.
     */
    template <typename TTuple>
    void run_with_archetype_all(std::function<void(std::vector<std::shared_ptr<TTuple>> &)> operation)
    {
        std::lock_guard<std::mutex>{guard};
        operation(archetype_all<TTuple>(nullptr));
    }

private:
    allocators::pool_allocator _allocator;
    static std::mutex guard;

    /**
     * A way of getting a static vector of some tuple type. For each tuple type, this function will create and return
     * arch_vec, which is a vector of pointers to all the archetypes TTuple type.
     *
     * TODO: Apart from the to_remove param, is this hacky? Is it bad for cache coherence?
     *
     * @tparam TTuple This is the std::tuple that identifies the archetype (a combination of the component types).
     * @tparam to_remove a ptr to remove from the vector
     * @return A static vector of all the active archetypes of this type.
     */
    template <typename TTuple>
    std::vector<std::shared_ptr<TTuple>> &archetype_all(TTuple *to_remove)
    {
        static std::vector<std::shared_ptr<TTuple>> arch_vec;

        if (to_remove)
            remove_ptr<TTuple>(arch_vec, to_remove);

        return arch_vec;
    }

    template <typename TTuple>
    void remove_ptr(std::vector<std::shared_ptr<TTuple>> &arch_vec, TTuple *to_remove)
    {
        std::lock_guard<std::mutex>{guard};

        auto new_end = std::remove_if(arch_vec.begin(), arch_vec.end(),
                                      [to_remove](const std::shared_ptr<TTuple> &ptr) { return ptr.get() == to_remove; });

        arch_vec.erase(new_end, arch_vec.end());
    }
};

std::mutex archetype_pool::guard;

} // namespace ecs

#endif //ECS_DEV_ARCHETYPE_POOL_H
