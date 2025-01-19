add_library(GUI STATIC main.h)

find_package(imgui CONFIG REQUIRED)

target_link_libraries(GUI PRIVATE imgui::imgui core)
