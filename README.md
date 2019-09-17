# Run Shoot

CS 529 Project. First person shooter/endless runner

## Building

This project relies on cloning and adding libraries as external cmake projects whenever it can. These are added with Add_ExternalProject, and installed to `/external`. 

When libraries are not conveniently available as public repos with CMake projects, the binaries/code are placed in `/thirdparty`. 

The first time you build this project, all `external` libs will be cloned and build, so it will take a while (especially assimp..). 

### Linux

```
mkdir build
cd build
cmake ..
make
```

### Windows

Use CMake GUI. You may have to tweak/fix build, as this is being developed on Linux, and periodically tested on Windows. 