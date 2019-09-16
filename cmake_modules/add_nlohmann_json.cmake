include(ExternalProject)

ExternalProject_Add(nlohmann_json
    PREFIX nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG release/3.7.0
    CMAKE_ARGS  -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}/nlohmann_json
                -DJSON_BuildTests=OFF
)

set (JSON_INCLUDE ${EXTERNAL_INSTALL_LOCATION}/nlohmann_json/include)