//
// Created by sava on 10/23/19.
//

#include <ecs/chunk_component_accessor.hpp>

ecs::chunk_component_accessor::chunk_component_accessor(void *chunk_ptr, component_bitset archetype_id) :
    _chunk_ptr(chunk_ptr),
    _description(chunk_size_for(archetype_id)),
    _header(calc_component_offsets())
{
    ecs::component_ptr* cursor = _header;
    std::uint8_t* chunk_ptr_byte = (std::uint8_t*) _chunk_ptr;
    std::uint8_t* component_ptr = chunk_ptr_byte + _description.components_offset;

    for(std::uint8_t i = 0; i < ECS_MAX_COMPONENT_TYPES; ++i)
    {
        component_bitset bit = component_bitset(1) << i;
        if (!(archetype_id & bit))
            continue;

        auto meta = component_meta::bit_metas.find(i)->second;

        component_ptr += calc_align_adjustment((uintptr_t)component_ptr, meta.align());

        cursor->component_bitshift = i;
        cursor->component_offset =  component_ptr - chunk_ptr_byte;
        cursor++;

        component_ptr += meta.size();
    }
}

ecs::chunk_component_accessor::chunk_component_accessor(const chunk_component_accessor& other) :
    _chunk_ptr(other._chunk_ptr),
    _description(other._description),
    _header(other._header)
{
}



ecs::chunk_description ecs::chunk_component_accessor::chunk_size_for(component_bitset archetype_id)
{
    uintptr_t chunk_size = 0;
    uintptr_t components_offset = 0;
    uintptr_t header_offset = 0;
    uint16_t component_count = 0;

    auto component_ptr_size = sizeof(component_ptr);
    auto component_ptr_align = alignof(component_ptr);

    chunk_size = component_ptr_align;

    // first, do the component pointers
    for(std::uint8_t i = 0; i < ECS_MAX_COMPONENT_TYPES; ++i)
    {
        component_bitset bit = component_bitset(1) << i;
        if (!(archetype_id & bit))
            continue;

        component_count++;

        auto meta = component_meta::bit_metas.find(i)->second;

        // we will have one component_ptr at the start
        chunk_size += calc_align_adjustment(chunk_size, component_ptr_align);

        if (header_offset == 0)
            header_offset = chunk_size;

        chunk_size += component_ptr_size;
    }

    // now, the components
    for(std::uint8_t i = 0; i < ECS_MAX_COMPONENT_TYPES; ++i)
    {
        component_bitset bit = component_bitset(1) << i;
        if (!(archetype_id & bit))
            continue;

        auto meta = component_meta::bit_metas.find(i)->second;

        chunk_size += calc_align_adjustment(chunk_size, meta.align());

        if (components_offset == 0)
            components_offset = chunk_size;

        chunk_size += meta.size();
    }

    return { chunk_size, header_offset, components_offset, component_count };
}

uintptr_t ecs::chunk_component_accessor::calc_align_adjustment(uintptr_t raw, uintptr_t alignment)
{
    if (alignment == 0) return 0;

    uintptr_t mask = (alignment - 1);
    uintptr_t misalignment = raw & mask;

    std::uint32_t mask_32 = ((std::uint32_t) alignment) - 1;
    std::uint32_t raw_32 = (std::uint32_t) raw;
    std::uint32_t misalignment_32 = mask_32 & raw_32;

    return misalignment > 0
           ? alignment - misalignment
           : 0;
}

ecs::component_ptr *ecs::chunk_component_accessor::calc_component_offsets()
{
    std::uint8_t* ptr = (std::uint8_t*) _chunk_ptr;
    ptr += _description.header_offset;
    return (ecs::component_ptr*) ptr;
}

void ecs::chunk_component_accessor::construct()
{
    auto cursor = _header;
    auto count = _description.component_count;

    while (count-- > 0)
    {
        std::uint8_t* ptr_byte = (std::uint8_t*)_chunk_ptr;

        void *ptr = (void*)(ptr_byte + cursor->component_offset);
        auto meta = component_meta::bit_metas.find(cursor->component_bitshift)->second;
        meta.construct(ptr);
        cursor++;
    }
}

void ecs::chunk_component_accessor::destruct()
{
    auto cursor = _header;
    auto count = _description.component_count;

    while (count-- > 0)
    {
        std::uint8_t* ptr_byte = (std::uint8_t*)_chunk_ptr;

        void *ptr = (void*)(ptr_byte + cursor->component_offset);
        auto meta = component_meta::bit_metas.find(cursor->component_bitshift)->second;
        meta.destruct(ptr);
        cursor++;
    }
}

