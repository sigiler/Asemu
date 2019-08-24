
cmake_minimum_required(VERSION 3.0)

project(Asemu)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Og -g")

set(SOURCE_FILES
    src/main.cpp
    src/emu.cpp
    src/core/board.cpp
    src/core/cpu.cpp
    src/core/keys.cpp
    src/core/mem.cpp
    src/core/ports.cpp
    src/core/screen.cpp
    src/core/speaker.cpp
    src/core/timer.cpp
    src/frontend/sdl_app.cpp
    src/frontend/sdl_input.cpp
    src/frontend/sdl_output.cpp
    src/libs/configfile.cpp
    src/libs/ringbuffer.cpp
    src/tests/test_hw.cpp
    src/tests/test_opcodes.cpp
    src/utils/logger.cpp
    src/utils/profiler.cpp
    src/utils/tester.cpp
)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
link_libraries(${SDL2_LIBRARIES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME})
