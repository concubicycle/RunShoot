# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

set(CORE_SOURCE_LIST
${PROJECT_SOURCE_DIR}/src/frame_limiter.cpp
${PROJECT_SOURCE_DIR}/src/frame_timer.cpp
${PROJECT_SOURCE_DIR}/src/framerate.cpp
${PROJECT_SOURCE_DIR}/src/startup_config.cpp
${PROJECT_SOURCE_DIR}/src/startup.cpp
)

set(PLATFORM_SOURCE_LIST
${PROJECT_SOURCE_DIR}/src/sleep.cpp
)

set(UTIL_SOURCE_LIST
)

# Optionally glob, but only for CMake 3.12 or later:
# file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${ModernCMakeExample_SOURCE_DIR}/include/modern/*.hpp")
set(HEADER_LIST
${PROJECT_SOURCE_DIR}/include/platform/sleep.hpp
${PROJECT_SOURCE_DIR}/include/core/frame_limiter.hpp
${PROJECT_SOURCE_DIR}/include/core/frame_timer.hpp
${PROJECT_SOURCE_DIR}/include/core/framerate.hpp
${PROJECT_SOURCE_DIR}/include/core/startup_config.hpp
${PROJECT_SOURCE_DIR}/include/core/startup.hpp
${PROJECT_SOURCE_DIR}/include/util/running_average.hpp
)

# Make an automatic library - will be static or dynamic based on user setting
add_library(core SHARED ${CORE_SOURCE_LIST} ${HEADER_LIST})
add_library(platform SHARED ${PLATFORM_SOURCE_LIST} ${HEADER_LIST})
#add_library(util SHARED ${UTIL_SOURCE_LIST} ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(core PUBLIC ../include)
target_include_directories(platform PUBLIC ../include)
#target_include_directories(util PUBLIC ../include)

# All users of this library will need at least C++11
target_compile_features(core PUBLIC cxx_std_11)
target_compile_features(platform PUBLIC cxx_std_11)
#target_compile_features(util PUBLIC cxx_std_11)

