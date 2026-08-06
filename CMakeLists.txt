cmake_minimum_required(VERSION 3.20)

project(Nexus VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(NEXUS_BUILD_TESTS "Build Nexus tests" ON)

add_library(NexusCore STATIC
    src/Nexus.Core/AppMetadata.cpp
)
target_include_directories(NexusCore PUBLIC src)

add_library(NexusModules STATIC
    src/Nexus.Modules/Home/HomeModule.cpp
    src/Nexus.Modules/Settings/SettingsModule.cpp
)
target_include_directories(NexusModules PUBLIC src)
target_link_libraries(NexusModules PUBLIC NexusCore)

if(MSVC)
    target_compile_options(NexusCore PRIVATE /W4 /permissive-)
    target_compile_options(NexusModules PRIVATE /W4 /permissive-)
else()
    target_compile_options(NexusCore PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(NexusModules PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(WIN32)
    add_executable(Nexus WIN32
        src/Nexus.App/main.cpp
        src/Nexus.Platform/WindowsApplication.cpp
        src/Nexus.UI/MainWindow.cpp
    )
    target_include_directories(Nexus PRIVATE src)
    target_compile_definitions(Nexus PRIVATE UNICODE _UNICODE)
    target_link_libraries(Nexus PRIVATE NexusCore NexusModules user32 gdi32)

    if(MSVC)
        target_compile_options(Nexus PRIVATE /W4 /permissive-)
    else()
        target_compile_options(Nexus PRIVATE -Wall -Wextra -Wpedantic)
        target_link_options(Nexus PRIVATE -municode -static -static-libgcc -static-libstdc++)
    endif()

    set_target_properties(Nexus PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
else()
    message(STATUS "Nexus.exe is only built when targeting Windows. Core and module tests remain available on this host.")
endif()

if(NEXUS_BUILD_TESTS)
    enable_testing()

    add_executable(NexusCoreTests
        tests/CoreTests.cpp
    )
    target_link_libraries(NexusCoreTests PRIVATE NexusCore NexusModules)

    if(MSVC)
        target_compile_options(NexusCoreTests PRIVATE /W4 /permissive-)
    else()
        target_compile_options(NexusCoreTests PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    add_test(NAME NexusCoreTests COMMAND NexusCoreTests)
endif()
