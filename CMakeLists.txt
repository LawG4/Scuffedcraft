# Set the project and include the toolchain

cmake_minimum_required(VERSION 3.2)
include("${CMAKE_CURRENT_LIST_DIR}/cmake/ppcToolchain.cmake")
project(ScuffedCraft LANGUAGES C)


# Glob all the C files in the source directory
file(GLOB_RECURSE C_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "source/*.c")
add_executable(ScuffedCraft.elf ${C_SOURCES})


message("CMAKE_AR           : ${CMAKE_AR}")  
message("CMAKE_ASM_COMPILER : ${CMAKE_ASM_COMPILER}")  
message("CMAKE_C_COMPILER   : ${CMAKE_C_COMPILER}")  
message("CMAKE_CXX_COMPILER : ${CMAKE_CXX_COMPILER}")  
message("CMAKE_LINKER       : ${CMAKE_LINKER}")  
message("CMAKE_OBJCOPY      : ${CMAKE_OBJCOPY}")  
message("CMAKE_RANLIB       : ${CMAKE_RANLIB}")  
message("CMAKE_SIZE         : ${CMAKE_SIZE}")  
message("CMAKE_STRIP        : ${CMAKE_STRIP}")  






