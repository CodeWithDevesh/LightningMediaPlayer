# Define the core source directory as the current directory
message(STATUS "Setting up core")

set(core_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Create a static library named core using player.cpp and utils.cpp
add_library(core STATIC core.cpp utils.cpp player.cpp playlist.cpp)

# Find and link spdlog library
find_package(spdlog CONFIG REQUIRED)

# Find libvlc library
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBVLC REQUIRED libvlc)

target_include_directories(core PUBLIC ${LIBVLC_INCLUDE_DIRS})
target_link_directories(core PUBLIC ${LIBVLC_LIBRARY_DIRS})


target_link_libraries(core PUBLIC spdlog::spdlog ${LIBVLC_LIBRARIES})