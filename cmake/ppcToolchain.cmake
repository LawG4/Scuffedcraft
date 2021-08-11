cmake_minimum_required(VERSION 3.2)

# Set the path to the repo root
set(SC_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." CACHE FILEPATH "The root directory to the Scuffedcraft repo")
mark_as_advanced(SC_ROOT)

# Firstly ensure that the user has got the sc_env.sh file created.
# and if so add those variables to the cmake cache
if(NOT EXISTS "${SC_ROOT}/cmake/sc_env.cmake")
	message(FATAL_ERROR "You have not initialised the repo")
else()
	include("${SC_ROOT}/cmake/sc_env.cmake")
	set(DEVKITPRO "${DEVKITPRO}" CACHE FILEPATH "The file path to devkitpro")
	set(DEVKITARM "${DEVKITARM}" CACHE FILEPATH "The file path to devkitarm")
	set(DEVKITPPC "${DEVKITPPC}" CACHE FILEPATH "The file path to devkitppc")
	set(SC_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." CACHE FILEPATH "The command to run dolphin")
endif()

# Tell Cmake where it can look for the definition of the wii platform 
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

# Tell CMake we're crosscompiling
set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_PROCESSOR "powerpc-eabi")
set(CMAKE_LIBRARY_ARCHITECTURE powerpc-eabi CACHE INTERNAL "abi")


# Set the prefix to the build tools
set(TOOLS_PREFIX "${DEVKITPPC}/bin/powerpc-eabi-")

# Set all of the toolchain paths
set(CMAKE_AR                    "${TOOLS_PREFIX}ar"		 CACHE PATH "")
set(CMAKE_ASM_COMPILER          "${TOOLS_PREFIX}gcc"	 CACHE PATH "")
set(CMAKE_C_COMPILER            "${TOOLS_PREFIX}gcc"	 CACHE PATH "")
set(CMAKE_CXX_COMPILER          "${TOOLS_PREFIX}g++"	 CACHE PATH "")
set(CMAKE_LINKER                "${TOOLS_PREFIX}ld"		 CACHE PATH "")
set(CMAKE_OBJCOPY               "${TOOLS_PREFIX}objcopy" CACHE PATH "")
set(CMAKE_RANLIB                "${TOOLS_PREFIX}ranlib"	 CACHE PATH "")
set(CMAKE_SIZE                  "${TOOLS_PREFIX}size"	 CACHE PATH "")
set(CMAKE_STRIP                 "${TOOLS_PREFIX}strip"	 CACHE PATH "")

# Set the C flags required for the wii, including the include path
# We need to set these not just for the target because CMake checks for a working compiler
# and if it doesn't detect one it reverts back to the system one
set(CMAKE_C_FLAGS "-g -O2 -Wall -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${DEVKITPRO}/portlibs/wii/include -I${DEVKITPRO}/portlibs/ppc/include -I${DEVKITPRO}libogc/include")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I/opt/devkitpro/portlibs/wii/include -I/opt/devkitpro/portlibs/ppc/include -I/opt/devkitpro/libogc/include")
# Clear the other compiler flags to prevent the defaults being set up
add_link_options("g -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float")

# Make Cmake's compiler tests less rigurous
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Shared libs not available" )

message("===== End of toolchain file =====")