add_library(CONSOLE STATIC main.cpp main.h)

target_link_libraries(CONSOLE PRIVATE core)
