
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(CMAKE_AUTOMOC ON)  # Auto-generate MOC files
set(CMAKE_AUTORCC ON)  # Auto-generate Qt resource files
set(CMAKE_AUTOUIC ON)  # Auto-generate UI files


add_library(GUI STATIC main.cpp widgets/videoWidget.cpp widgets/mainWindow.cpp)
target_link_libraries(GUI PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui core)
target_include_directories(GUI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)