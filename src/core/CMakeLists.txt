find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

add_library(core STATIC player.cpp)

target_link_libraries(core PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)

target_include_directories(core PRIVATE ${Qt6_INCLUDE_DIRS})
