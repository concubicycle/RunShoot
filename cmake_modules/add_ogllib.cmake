file(GLOB OGLLIB_SOURCE
     ${PROJECT_SOURCE_DIR}/${THIRD_PARTY}/opengllib/*.h
     ${PROJECT_SOURCE_DIR}/${THIRD_PARTY}/opengllib/*.cpp
     ${PROJECT_SOURCE_DIR}/${THIRD_PARTY}/opengllib/shader_program.cpp)

add_library(${OGLLIB_NAME} ${OGLLIB_SOURCE})

set (OGLLIB_INCLUDE ${PROJECT_SOURCE_DIR}/${THIRD_PARTY}/opengllib/)