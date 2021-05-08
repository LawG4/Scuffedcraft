cmake_minimum_required(VERSION 3.4.0 FATAL_ERROR)

# Start by including the base rules for devkitPPC
include("${CMAKE_CURRENT_LIST_DIR}/DevkitppcToolchain.cmake")

# These are the C flags that Change based on if you are using Wii or GBC
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float")

# Link against the portlibs and the add them as an include directory
# Do this via command line arguments because we don't have a target yet for cmake to link to.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${DEVKITPRO_PATH}/portlibs/wii -I${DEVKITPRO_PATH}/portlibs/ppc" )