find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

add_library(CONSOLE STATIC main.h)

target_link_libraries(CONSOLE PRIVATE core)

target_include_directories(CONSOLE PRIVATE ${Qt6_INCLUDE_DIRS})
