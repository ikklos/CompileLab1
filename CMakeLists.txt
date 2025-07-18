cmake_minimum_required(VERSION 3.16)
project(lab02)
set(CMAKE_CXX_STANDARD 14)

include(FindFLEX)
if(FLEX_FOUND)
    message("Info: flex found!")
else()
    message("Error: flex not found!")
endif()
include_directories(${CMAKE_SOURCE_DIR})
FLEX_TARGET(MyScanner lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
add_compile_options(-pedantic)
#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)
add_executable(Compilerlab2
    compilelab1.cpp
    mytab.cpp
    ${FLEX_MyScanner_OUTPUTS}
)
target_compile_features(Compilerlab2 PRIVATE cxx_std_14)