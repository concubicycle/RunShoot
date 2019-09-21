#ifndef __BEHAVIOR_COMPONENT_H_
#define __BEHAVIOR_COMPONENT_H_

#include <ecs/component.hpp>

namespace ecs
{
class behavior_component : public component
{
public:
    virtual void update() = 0;
};
} // namespace ecs

#endif
