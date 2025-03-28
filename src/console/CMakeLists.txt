message(STATUS "Setting up CONSOLE")

add_library(console STATIC main.cpp "cli_handler.cpp")

find_package(CLI11 REQUIRED)
find_package(ftxui CONFIG REQUIRED)
target_link_libraries(console PRIVATE CLI11::CLI11 core ftxui::dom ftxui::screen ftxui::component)
target_include_directories(console PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
