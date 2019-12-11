# Run Shoot

CS 529 Project. First person shooter/endless runner


## Milestone 1

The main is in /apps/main.cpp.

This will just have a quad that you can move around with WASD.
The scale of the quad is shrunk and reset along various axes on key down/key up. 

## Milestone 2

You can now move around via wasd/mouse in free-fly mode (like noclip mode in source engine).
There's a drone going up and down in front of the player, and the first track segment (in progress)
behind the player (it hasn't been rotated to face -z direction yet).

The ECS in this project tries to be similar to one described in [this Unity blog post](https://blogs.unity3d.com/2019/03/08/on-dots-entity-component-system/).

An `archetype` is a a set of components, represented as a std::bitset.

At the lowest level, there is a `memory_pool` that allocates big memory blocks, and splits
them up into chunks that it distributes. 

On top of that is an `archetype_pool`, which takes
those chunks and initializes an entity with some set of components in them. 

`chunk_component_accessor`
actually does the heavy lifting when it comes to getting stuff into/out-of the raw memory.

`entity_factory` maintains a map of archetype_id to `archetype_pool`, for each archetype. 

`entity_world` is the top-most entity manager class. 

`basic_components.hpp` defines a bunch of built-in component structs, and the corresponding 
cpp file assigns bit-shifts to the components. an archetype is actually a bitset, which 
consists of the bit that identifies each component. 

Loaders for all components must be registered in `asset::component_loader::loader_functions`, 
which is a map of component bitshift to loader function. These functions have the prototype:
```c++
void load_some_component(const json &j, ecs::entity &e, string_table& hashes);
```

The registration of custom components takes place in add_custom_components(). Here, we have to
provide loaders for our components, and also add entries to `component_meta::bit_metas`, 
which is a map of metadata objects for each component type (kind of like manual reflection). 

Components are mostly just structs with data. The logic resides in `core::behavior` implementations,
such as `character_controller` and `drone_controller`.

## Milestone 3

The event system has been implemented using `std::function` callbacks, and variadic templates.
The main class for it is `event_exchange`.

Subscribing to events works like:
```oclight
auto listener_id = _event_exchange.subscribe<ecs::entity&>(
    events::entity_created, 
    std::function<void(ecs::entity&)>(on_entity_created));
```

Then, its clients need to unsubscribe like:
```oclight
auto listener_id = _event_exchange.unsubscribe(
    events::entity_created, 
    listener_id);
```

There are analogous `invoke` and `invoke_delayed` functions:
```oclight
 _events.invoke<ecs::entity&>(
            events::entity_created,
            e);
```

```oclight
 _events.invoke_delayed<ecs::entity&>(
            events::delay_entity_jump,
            std::chrono::duration<float>(3),
            e);
```

Physics and Rendering classes are relying on the entity_created event to 
actually handle anything, so if the game works at all, those are working. 

In order to show the delayed event, a dummy 'delayed jump' event is invoked
when the player jumps (space). This will fire 3 seconds after the actual jump. 


## Building

This project relies on git-cloning and adding libraries as external cmake projects whenever it can. 
These are added with Add_ExternalProject, and installed to `/external`. 

The first time you build this project, all `external` libs will be cloned and build, so it will take a while.
It will download and build dependencies. The upside is that the dependencies can be modified, and will build 
on any platform.

Whatever libraries are not conveniently available as public repos with CMake projects are placed in `/thirdparty`. 

*NOTE*: 
If you encounter a zlib-related error, (or some other linking error) try building/running again. I think I resolved this,
but it was an issue on windows machines:
fatal error LNK1105: cannot close file 'C:/sandbox/dev/RunShoot/build/assimp/src/assimp-build/contrib/zlib/Debug/zlibd.lib'




### Linux

```
mkdir build
cd build
cmake ..
make
```

### Windows

*NOTE*: For a Release build, you have to change the mode to Release in CMakeLists.txt, and then change it in Visual Studio when building. Same for debug

This is being tested on Visual Studio 2017. The paths for projects pulled from github can be finicky depending 
on platform/tools, so it's best to work with VS2017. Milestone 1 has been tested and works on VS2019.

I use cmake GUI on windows. In CMake GUI,

* Set Project directory to project
* Set build directory to {PROJECT_DIR}/build. Like:
```
C:/sandbox/dev/RunShoot
C:/sandbox/dev/RunShoot/build
```
* Configure
* Generate
* Open Project
* Build/run RunShoot


## Scene file format

The level files are in JSON format, and allow for a basic description of a scene, 
with some parenting and data inheritance support. 

### Scene

A scene is a collection of entities. Child relationships can be represented
as an an entity id:

```json
{
  "entities": [
    {
      "entity_id": 1,
      "components": []
    },
    {
      "entity_id": 2,
      "parent_id": 1,
      "components": []
    }]
}
```

All the ids in these files should be set, since they ought to be generated 
by an editor eventually. But I guess there's no real reason not to auto-generate
and id if one is omitted.


### Components

Components are currently identified by a `component_bitshift` property, which 
isn't very friendly, but again, these files should probably be editor-generated
anyway. There are currently a maximum of 16 built in components. The current ones
are:

```
template<> const component_shift ecs::component<ecs::transform_component>::component_bitshift = 0;
template<> const component_shift ecs::component<ecs::camera_component>::component_bitshift = 1;
template<> const component_shift ecs::component<ecs::render_component_ogl>::component_bitshift = 2;
template<> const component_shift ecs::component<ecs::punctual_light_component>::component_bitshift = 3;
template<> const component_shift ecs::component<ecs::aabb_collider_component>::component_bitshift = 4;
template<> const component_shift ecs::component<ecs::sphere_collider_component>::component_bitshift = 5;
template<> const component_shift ecs::component<ecs::rigid_body_component>::component_bitshift = 6;
```

All the properties other than the component_bitshift depend on the type of component.

### Prototypes

A prototype is like a Unity prefab. It is a template for some entity, which can have
children entities. Its format is:
```
{
    "root": {},
    "children": []
}
```
Where the root object and each `children` element are entities. These do not have ids.
The elements of "children" have the `root` entity as their implied parent. 
They can, alternatively, define parents that are within the `children` array
by index. So the property is called `parent_index` 

### Authoring Components

WIP