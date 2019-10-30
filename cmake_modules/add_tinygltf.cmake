include(ExternalProject)

ExternalProject_Add(tinygltf
                    PREFIX tinygltf
                    GIT_REPOSITORY https://github.com/syoyo/tinygltf.git
                    GIT_TAG master
                    CMAKE_ARGS
                    -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}/tinygltf
                    -DTINYGLTF_BUILD_EXAMPLES=OFF)

set(TINYGLTF_INCLUDE ${EXTERNAL_INSTALL_LOCATION}/tinygltf/include)

