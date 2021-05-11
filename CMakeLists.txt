

cmake_minimum_required(VERSION 3.15)
project(simple-cplusplus)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)



# options

# use value in config.cmake
# include(config.cmake)

message("USING CMAKE_BUILD_TYPE = " ${CMAKE_BUILD_TYPE})

add_subdirectory(src)


