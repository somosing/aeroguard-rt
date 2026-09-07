cmake_minimum_required(VERSION 3.20)

project(
    AeroGuardRT
    VERSION 0.1.0
    LANGUAGES CXX
)

add_library(aeroguard_core
    src/version.cpp
)

target_compile_features(aeroguard_core
    PUBLIC
        cxx_std_20
)

target_include_directories(aeroguard_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(aeroguard_core
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(aeroguard
    src/main.cpp
)

target_link_libraries(aeroguard
    PRIVATE
        aeroguard_core
)

include(CTest)

if(BUILD_TESTING)
    add_executable(aeroguard_tests
        tests/version_test.cpp
    )

    target_link_libraries(aeroguard_tests
        PRIVATE
            aeroguard_core
    )

    add_test(
        NAME version_test
        COMMAND aeroguard_tests
    )
endif()
