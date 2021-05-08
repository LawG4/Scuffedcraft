# The Toolchain file to allow CMake to target the DevkitPPC toolchain.
cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)

# Firstly ensure that the user has devkitPPC and devkitPro environment variables 
if(DEFINED ENV{DEVKITPRO} AND DEFINED ENV{DEVKITPPC})
	message(STATUS "DevkitPro environment variable: $ENV{DEVKITPRO}")
	message(STATUS "DevkitPPC environment variable: $ENV{DEVKITPPC}")
else()
	message(FATAL_ERROR "Could not find both DevkitPPC and DevkitPRO environement variables, please set the environment variables correctly.")
endif()

# Verify that the devkit actually exists at expected locations and if it does set a variable that is used as a prefix to all the tools 
# Since windows sets DEVKITPPC env variable to be local to the MSYS2 file system (i.e. /opt/devkitPro) instead of local to the windows one
# This means the environement variable most likely can't be used to access the toolchain on windows, so trial a few locations 
# Once we have confirmed devkits location export the file path to the toolchain.
if(WIN32)  
	message(STATUS "Searching for DevkitPro and DevkitPRO on Windows...")
	if(EXISTS ENV{DEVKITPPC} AND EXISTS{DEVKITPRO})
		# Check the environement variable first on windows, is devkit possibly installed there?
		message(STATUS "Using the following paths \n\tDevkitPRO : $ENV{DEVKITPRO} \n\tDevkitPPC : $ENV{DEVKITPPC}")
		set(DEVKITPRO_PATH "$ENV{DEVKITPRO}")
		set(DEVKITPPC_PATH "$ENV{DEVKITPPC}")
	
	elseif(EXISTS c:/devkitPro AND EXISTS c:/devkitPro/devkitPPC)
		# Check the location that devkit is usually installed to on windows
		message(STATUS "Using the following paths \n\tDevkitPRO : C:/devkitPro \n\tDevkitPPC : C:/devkitPro/devkitPPC")
		set(DEVKITPRO_PATH "C:/devkitPro")
		set(DEVKITPPC_PATH "C:/devkitPro/devkitPPC")
	
	elseif(EXISTS "$ENV{WIN_DEVKITPRO}" AND EXISTS "$ENV{WIN_DEVKITPRO}/devkitPPC")
		# Check the WIN_DEVKITPRO environemnt variable for a custom install location
		message(STATUS "Using the following paths \n\tDevkitPRO : $ENV{WIN_DEVKITPRO} \n\tDevkitPPC : $ENV{WIN_DEVKITPRO}/devkitPPC")
		set(DEVKITPRO_PATH "$ENV{WIN_DEVKITPRO}")
		set(DEVKITPPC_PATH "$ENV{WIN_DEVKITPRO}/devkitPPC")
	
	else()
		# Could not find it on windows at all fail.
		message(FATAL_ERROR "Failed to find devkitPPC at any of the checked locations\n"
			"Environement vatriable DEVKITPPC : $ENV{DEVKITPRO}\n"
			"Default Windows location : C:/devkitPro\n"
			"Custom environement variable WIN_DEVKITPRO : $ENV{WIN_DEVKITPRO}\n"
			"Please make sure to install devkitPro properly, if you have installed it to a custom location add the WIN_DEVKITPRO environement variable\n"
			"Ensure that devkitPPC is installed at devkitPro/devkitPPC")
	endif()
else()
	
	# We are on the UNIX like system and we can use the environement variable
	# But just as a sanity check, make sure the files exist at that location
	if(EXISTS "$ENV{DEVKITPRO}" AND EXISTS "$ENV{DEVKITPPC}")
		message(STATUS "Using the following paths \n\tDevkitPRO : $ENV{DEVKITPRO} \n\tDevkitPPC : $ENV{DEVKITPPC}")
		set(DEVKITPRO_PATH "$ENV{DEVKITPRO}")
		set(DEVKITPPC_PATH "$ENV{DEVKITPPC}")
	else()
		message(STATUS "\n\t $ENV{DEVKITPRO} \n\t $ENV{DEVKITPPC}")
		message(FATAL_ERROR "DevkitPPC was not found at the location stored in the environement variable, please double check it")
	endif()
endif()

# Clean the variables that contain the paths to the devkit tools, to ensure that they are usable throughout the rest of Cmake
file(TO_CMAKE_PATH "${DEVKITPRO_PATH}" DEVKITPRO_PATH)
file(TO_CMAKE_PATH "${DEVKITPPC_PATH}" DEVKITPPC_PATH) 
file(TO_CMAKE_PATH "${DEVKITPPC_PATH}/bin/powerpc-eabi-" DEVKITPPC_PREFIX)
# At this point we have confirmed that the user has devkitPro installed and we have a variable pointing to toolchain
# Set the link to all of the toolchain binaries but first ensure the path is a cmake one

set(CMAKE_AR                    "${DEVKITPPC_PREFIX}ar")
set(CMAKE_ASM_COMPILER          "${DEVKITPPC_PREFIX}gcc")
set(CMAKE_C_COMPILER            "${DEVKITPPC_PREFIX}gcc")
set(CMAKE_CXX_COMPILER          "${DEVKITPPC_PREFIX}g++")
set(CMAKE_LINKER                "${DEVKITPPC_PREFIX}ld")
set(CMAKE_OBJCOPY               "${DEVKITPPC_PREFIX}objcopy")
set(CMAKE_RANLIB                "${DEVKITPPC_PREFIX}ranlib")
set(CMAKE_SIZE                  "${DEVKITPPC_PREFIX}size")
set(CMAKE_STRIP                 "${DEVKITPPC_PREFIX}strip")

# Display the toolchain details to the user if they ask
if(DEFINED PPC_VERBOSE)
	message(STATUS "CMAKE_AR            : ${CMAKE_AR}")
	message(STATUS "CMAKE_ASM_COMPILER  : ${CMAKE_ASM_COMPILER}")
	message(STATUS "CMAKE_C_COMPILER    : ${CMAKE_C_COMPILER}")
	message(STATUS "CMAKE_CXX_COMPILER  : ${CMAKE_CXX_COMPILER}")
	message(STATUS "CMAKE_LINKER        : ${CMAKE_LINKER}")
	message(STATUS "CMAKE_OBJCOPY       : ${CMAKE_OBJCOPY}")
	message(STATUS "CMAKE_RANLIB        : ${CMAKE_RANLIB}")
	message(STATUS "CMAKE_SIZE          : ${CMAKE_SIZE}")
	message(STATUS "CMAKE_STRIP         : ${CMAKE_STRIP}")
endif()

# On windows using the visual studio generator the CMAKE_???_COMPILER variables will be ignored 
# This is because VSGenerator computes the variables based on the toolset selected by MSBuild
# So instead we feed MSBuild a toolset so that it will select devkitPPC compiler
# This might not be required, instead perhpas the test compile program failed before
set(CMAKE_GENERATOR_TOOLSET "")

# Describe a cmake system for the PPC, we use generic because any other name gives a warning that:
# This platform is not supported by cmake, add it to our system on website...
set(CMAKE_SYSTEM_NAME		"Generic")
set(CMAKE_SYSTEM_PROCESSOR	"powerpc-eabi")

# Cmake sets some basic command line flags by default we want to reset them so windows settings aren't carried over
set(CMAKE_C_FLAGS               "")
set(CMAKE_CXX_FLAGS             "")
set(CMAKE_C_FLAGS_DEBUG         "")
set(CMAKE_C_FLAGS_RELEASE       "")
set(CMAKE_CXX_FLAGS_DEBUG       "")
set(CMAKE_CXX_FLAGS_RELEASE     "")
set(CMAKE_EXE_LINKER_FLAGS      "")

# Toolchain systemroot settings
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM     NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY     ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE     ONLY)
# Optionally reduce compiler sanity check when cross-compiling.
set(CMAKE_TRY_COMPILE_TARGET_TYPE         STATIC_LIBRARY)

set(CMAKE_CROSSCOMPILING true)

