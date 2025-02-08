add_executable(TUTORIAL main.cpp)
target_include_directories(TUTORIAL PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(TUTORIAL PRIVATE core)