# Run Shoot

CS 529 Project. First person shooter/endless runner

## Milestone 1

The main is in /apps/main.cpp.

This will just have a quad that you can move around with WASD.
The scale of the quad is shrunk and reset along various axes on key down/key up. 


## Building

This project relies on git-cloning and adding libraries as external cmake projects whenever it can. 
These are added with Add_ExternalProject, and installed to `/external`. 

The first time you build this project, all `external` libs will be cloned and build, so it will take a while.
It will download and build dependencies. The upside is that the dependencies can be modified, and will build 
on any platform.

Whatever libraries are not conveniently available as public repos with CMake projects are placed in `/thirdparty`. 

*NOTE*: Currently, there is currently a weird linker error that may occur on first time build, but not afterwards. I am looking into it. 
If you encounter a zlib-related error, try building again:
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