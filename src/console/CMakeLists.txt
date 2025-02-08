message(STATUS "Setting up CONSOLE")


add_executable(Lightning_console main.cpp "cli_handler.cpp")

find_package(CLI11 REQUIRED)
target_link_libraries(Lightning_console PRIVATE CLI11::CLI11 core)
target_include_directories(Lightning_console PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
