cmake_minimum_required(VERSION 3.4.0 FATAL_ERROR)

# Use the devkitPPC toolchain with verbose output 
set(PPC_VERBOSE "true")
#set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/WiiToolchain.cmake")
#include("${CMAKE_CURRENT_LIST_DIR}/cmake/WiiToolchain.cmake")
# Set a release and debug type build
# set(CMAKE_CONFIGURATION_TYPES "Release" "Debug")

# Create the Project
project(ScuffedCraft LANGUAGES C CXX)
include("${CMAKE_CURRENT_LIST_DIR}/cmake/WiiToolchain.cmake")

# Gather all of our C files
file(GLOB_RECURSE C_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "source/*.c")
add_executable(ScuffedCraft.elf ${C_SOURCES})

set_property(TARGET ScuffedCraft.elf PROPERTY C_STANDARD 99)

# Link and Include GRRLIB
set(GRRLIB_INC "${DEVKITPRO_PATH}/portlibs/ppc/include/GRRLIB"
	"${DEVKITPRO_PATH}/portlibs/ppc/include/libpng16"
	"${DEVKITPRO_PATH}/portlibs/ppc/include/freetype2"
	"${DEVKITPRO_PATH}/portlibs/ppc/include")
set(GRRLIB_LIB "${DEVKITPRO_PATH}/portlibs/ppc/lib/libgrrlib.a"
	"${DEVKITPRO_PATH}/portlibs/ppc/lib/libfreetype.a"
	"${DEVKITPRO_PATH}/portlibs/ppc/lib/libbz2.a"
	"${DEVKITPRO_PATH}/portlibs/ppc/lib/libpngu.a"
	"${DEVKITPRO_PATH}/portlibs/ppc/lib/libpng.a"
	"${DEVKITPRO_PATH}/portlibs/ppc/lib/libjpeg.a"
	"${DEVKITPRO_PATH}/portlibs/ppc/lib/libz.a")


# Link and Include libOGC
set(OGC_INC "${DEVKITPRO_PATH}/libogc/include")
set(OGC_LIB "${DEVKITPRO_PATH}/libogc/lib/wii/libfat.a"
	"${DEVKITPRO_PATH}/libogc/lib/wii/libwiiuse.a"	
	"${DEVKITPRO_PATH}/libogc/lib/wii/libbte.a"
	"${DEVKITPRO_PATH}/libogc/lib/wii/libogc.a"
	)

# Link and include Gamecube
set(GCC_INC "${DEVKITPPC_PATH}/powerpc-eabi/include") 
set(GCC_LIB "${DEVKITPPC_PATH}/powerpc-eabi/lib/libm.a"
	"${DEVKITPPC_PATH}/powerpc-eabi/lib/libc.a")

target_include_directories(ScuffedCraft.elf PUBLIC ${GCC_INC} ${OGC_INC} ${GRRLIB_INC})
target_link_libraries(ScuffedCraft.elf PUBLIC ${GRRLIB_LIB} ${OGC_LIB} ${GCC_LIB})

# Debug information because sometimes Cmake replaces the compiler if it failed the test compile
message(STATUS "Selected C compiler : ${CMAKE_C_COMPILER}")

# Set ScuffedCraft to our start up project 
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ScuffedCraft.elf)