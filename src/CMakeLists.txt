# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

# Optionally glob, but only for CMake 3.12 or later:
# file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${ModernCMakeExample_SOURCE_DIR}/include/modern/*.hpp")
set(HEADER_LIST
${PROJECT_SOURCE_DIR}/include/core/startup.hpp
${PROJECT_SOURCE_DIR}/include/core/startup_config.hpp
)

# Make an automatic library - will be static or dynamic based on user setting
add_library(core
${PROJECT_SOURCE_DIR}/src/startup.cpp
${PROJECT_SOURCE_DIR}/src/startup_config.cpp
    ${HEADER_LIST}
)

# We need this directory, and users of our library will need it too
target_include_directories(core PUBLIC ../include)

# All users of this library will need at least C++11
target_compile_features(core PUBLIC cxx_std_11)

# IDEs should put the headers in a nice place
source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})

