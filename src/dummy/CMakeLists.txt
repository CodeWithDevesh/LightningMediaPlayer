message(STATUS "Setting up DUMMY")

set(CMAKE_AUTOMOC ON)  # Auto-generate MOC files
set(CMAKE_AUTORCC ON)  # Auto-generate Qt resource files
set(CMAKE_AUTOUIC ON)  # Auto-generate UI files

add_library(dummy STATIC main.cpp dummy_widget.cpp)

target_link_libraries(dummy PRIVATE core)
target_include_directories(dummy PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
