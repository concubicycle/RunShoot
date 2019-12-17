//
// Created by sava on 10/8/19.
//

#ifndef __ARCHETYPE_POOL_TESTS_H_
#define __ARCHETYPE_POOL_TESTS_H_

#include <cstdio>
#include <cstring>

#include <ecs/ecs_types.hpp>
#include <ecs/archetype_pool.hpp>
#include <ecs/components/basic_components.hpp>
#include <ecs/component.hpp>

void test_ctor();
void test_make_entity();

int test_archetype_pool_all()
{
    test_ctor();
    test_make_entity();

    printf("Done with archetype pool tests.\n");
    return 0;
}


void test_ctor()
{
    auto arch_id = ecs::transform_component::archetype_bit | ecs::render_component_ogl::archetype_bit;
    ecs::archetype_pool pool(arch_id, 1000);
}

void test_make_entity()
{
    auto arch_id = ecs::transform_component::archetype_bit | ecs::render_component_ogl::archetype_bit;
    ecs::archetype_pool pool(arch_id, 1000);

    for(int i = 0; i < 10000; ++i)
    {
        auto e = pool.allocate_entity(123);
        auto t = e.get_component<ecs::transform_component>();
        auto r = e.get_component<ecs::render_component_ogl>();

        auto m = t.to_mat4();

        for (int j = 0; j < 4; ++j)
        {
            if (m[j][j] != 1.f)
                throw "matrix not identity";
        }

        t.pos = glm::vec3(1, 2, 3);
        t.pitch = 1;
        t.yaw = 2;
        t.roll = 3;
    }

    printf("Done with make entity test.\n");
}

#endif //__ARCHETYPE_POOL_TESTS_H_
