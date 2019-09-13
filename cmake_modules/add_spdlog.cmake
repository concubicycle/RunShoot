include(ExternalProject)

ExternalProject_Add(spdlog
    PREFIX spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.x
    CMAKE_ARGS  
        -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}/spdlog)

set(SPDLOG_INCLUDE ${EXTERNAL_INSTALL_LOCATION}/spdlog/include)

