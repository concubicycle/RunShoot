include(ExternalProject)

ExternalProject_Add(nlohmann_json
    PREFIX nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.7.2
	GIT_SHALLOW TRUE
	CMAKE_ARGS  -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}/nlohmann_json
                -DJSON_BuildTests=OFF
)

set (JSON_INCLUDE ${EXTERNAL_INSTALL_LOCATION}/nlohmann_json/include)
