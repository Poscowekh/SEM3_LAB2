cmake_minimum_required(VERSION 3.5)

project(SEM3_LAB2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(Python2 COMPONENTS Development NumPy)
#target_include_directories(SEM3_LAB2 PRIVATE ${Python2_INCLUDE_DIRS} ${Python2_NumPy_INCLUDE_DIRS})
#target_link_libraries(SEM3_LAB2 Python2::Python Python2::NumPy)

add_executable(SEM3_LAB2 src/main.cpp)
