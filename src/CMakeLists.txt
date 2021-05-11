
add_subdirectory(lib/fmt)

add_executable(format_example bin/format_main.cpp)
target_link_libraries(format_example PRIVATE simpleFormat)