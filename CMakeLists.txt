cmake_minimum_required(VERSION 3.20)

project(
    AeroGuardRT
    VERSION 0.1.0
    LANGUAGES CXX
)

find_package(MAVSDK 3.17.4 EXACT CONFIG REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(
    AEROGUARD_ENABLE_SANITIZERS
    "Enable AddressSanitizer and UndefinedBehaviorSanitizer"
    OFF
)

add_library(aeroguard_warnings INTERFACE)

target_compile_options(aeroguard_warnings
    INTERFACE
        -Wall
        -Wextra
        -Wpedantic
)

add_library(aeroguard_sanitizers INTERFACE)

if(AEROGUARD_ENABLE_SANITIZERS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(aeroguard_sanitizers
            INTERFACE
                -fsanitize=address,undefined
                -fno-omit-frame-pointer
        )

        target_link_options(aeroguard_sanitizers
            INTERFACE
                -fsanitize=address,undefined
        )
    else()
        message(FATAL_ERROR
            "AEROGUARD_ENABLE_SANITIZERS currently supports GCC and Clang only"
        )
    endif()
endif()

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

target_link_libraries(aeroguard_core
    PUBLIC
        aeroguard_warnings
        aeroguard_sanitizers
)

add_executable(aeroguard
    src/main.cpp
)

target_link_libraries(aeroguard
    PRIVATE
        aeroguard_core
        MAVSDK::mavsdk
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
