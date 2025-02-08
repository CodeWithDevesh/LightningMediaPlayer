cmake_minimum_required(VERSION 3.15)
project(LightningMediaPlayer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add core library
add_subdirectory(src/core)

# Build modes
option(TUTORIAL "Build the tutorial target" OFF)
option(USE_GUI "Build with GUI (qt)" ON)

if(TUTORIAL)
    message(STATUS "Building tutorial")
    add_subdirectory(src/tutorial)
elseif(USE_GUI)
    message(STATUS "Building with GUI")
    add_subdirectory(src/gui)
    add_executable(LightningMediaPlayer)
    target_compile_definitions(LightningMediaPlayer PRIVATE USE_GUI)
    target_link_libraries(LightningMediaPlayer PRIVATE core GUI)
else()
    message(STATUS "Building with Console Interface")
    add_subdirectory(src/console)
endif()
