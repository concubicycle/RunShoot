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