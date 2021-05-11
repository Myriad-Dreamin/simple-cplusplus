
add_library(simpleFormat Format.cpp FormatAction.cpp)
target_include_directories(simpleFormat PUBLIC include)
target_include_directories(simpleFormat PRIVATE .)
