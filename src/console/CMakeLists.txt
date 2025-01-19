message(STATUS "Setting up CONSOLE")


add_library(CONSOLE STATIC main.cpp "cli_handler.cpp")

find_package(CLI11 REQUIRED)
target_link_libraries(CONSOLE PRIVATE CLI11::CLI11 core)
target_include_directories(CONSOLE PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
