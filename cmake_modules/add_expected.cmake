include(ExternalProject)

ExternalProject_Add(expected
    PREFIX expected
    GIT_REPOSITORY https://github.com/TartanLlama/expected.git
    GIT_TAG v1.0.0
    CMAKE_ARGS  -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}/expected
                -DEXPECTED_ENABLE_TESTS=OFF
)

# not sure how to handle this perfectly. on windows, lib name is different

set(EXPECTED_INCLUDE ${EXTERNAL_INSTALL_LOCATION}/expected/include)


