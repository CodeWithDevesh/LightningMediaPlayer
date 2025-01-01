find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

add_library(GUI STATIC main.h)

target_link_libraries(GUI PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui core)

target_include_directories(GUI PRIVATE ${Qt6_INCLUDE_DIRS})
