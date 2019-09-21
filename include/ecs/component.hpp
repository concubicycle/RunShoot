#ifndef __COMPONENT_H_
#define __COMPONENT_H_

#include <cstddef>
#include <cstdint>

#include <fstream>

#include <ecs/component_store.hpp>
#include <ecs/archetype_store.hpp>

namespace ecs
{

class component
{

protected:
    const component_store &_component_store;
    const archetype_store &_archetype_store;

private:
};

#endif
