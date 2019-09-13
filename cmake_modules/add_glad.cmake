    
add_library(${GLAD} STATIC ${PROJECT_SOURCE_DIR}/thirdparty/GLAD/src/glad.c)
target_include_directories(${GLAD} PUBLIC ${PROJECT_SOURCE_DIR}/thirdparty/GLAD/include)

set(GLAD_INCLUDE ${PROJECT_SOURCE_DIR}/thirdparty/GLAD/include)