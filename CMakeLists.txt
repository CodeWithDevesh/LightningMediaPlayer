cmake_minimum_required(VERSION 3.15)
project(LightningMediaPlayer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add core library
add_subdirectory(src/core)

# Build modes
option(DUMMY "Build the dummy target" OFF)
option(USE_GUI "Build with GUI (qt)" ON)
option(USE_CONSOLE "Build with Console Interface" OFF)

if(USE_GUI)
    message(STATUS "Building the GUI Interface")
    add_subdirectory(src/gui)
    add_executable(LightningMediaPlayer src/gui_main.cpp)
    target_compile_definitions(LightningMediaPlayer PRIVATE USE_GUI)
    target_link_libraries(LightningMediaPlayer PRIVATE core GUI)
endif()

if(USE_CONSOLE)
    message(STATUS "Building the Console Interface")
    add_subdirectory(src/console)
    add_executable(LightningConsole src/console_main.cpp)
    target_compile_definitions(LightningConsole PRIVATE USE_CONSOLE)
    target_link_libraries(LightningConsole  PRIVATE core console)
endif()

if(DUMMY)
    message(STATUS "Building dummy")
    add_subdirectory(src/dummy)
    add_executable(LightningDummy src/dummy_main.cpp)
    target_link_libraries(LightningDummy PRIVATE core dummy)
    target_compile_definitions(LightningDummy PRIVATE USE_DUMMY)
endif()