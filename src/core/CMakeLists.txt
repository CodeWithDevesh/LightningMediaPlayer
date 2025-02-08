# Define the core source directory as the current directory
message(STATUS "Setting up core")

set(core_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Create a static library named core using player.cpp and utils.cpp
add_library(core STATIC  "utils.cpp" "player.cpp" "core.cpp" "AudioPlayer.cpp")

# Find and link spdlog library
find_package(spdlog CONFIG REQUIRED)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(core PRIVATE $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main> $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)

# Find and link FFMPEG library
find_package(FFMPEG REQUIRED)
target_include_directories(core PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_directories(core PRIVATE ${FFMPEG_LIBRARY_DIRS})
target_link_libraries(core PUBLIC ${FFMPEG_LIBRARIES} spdlog::spdlog)
