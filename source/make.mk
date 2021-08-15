# Define all of the .c files used starting with the main file
CFILES=Main.c 

# Render subdirectory 
CFILES+=Render.c

VPATH = source source/Render

# For each of the source files we need to automatically generate the 
# dependencies files and the object files. 
# Dependency files track the files that each source file depeneds on
# The object files are compiled source files 
# Automatically build a list of these files from the list of source files
DEP_FILES = $(patsubst %.c, $(BUILD_DIR)/%.d, $(CFILES))
OBJ_FILES = $(patsubst %.c, $(BUILD_DIR)/%.o, $(CFILES))
# Define the static libraries that we will link against 

# Add the external dependencies to the include directories 
# GRRLib the graphics assitance library
INCLUDE_DIRS+=-Iexternal/GRRLIB/GRRLIB/GRRLIB
# libogc the library for interfacing with the Wii hardware
INCLUDE_DIRS+=-I$(DEVKITPRO)/libogc/include

# Create a list of C flags, that will be used to compile the source files
# g           : Generates debug symbols
# O2          : Optimisation level 2
# Wall        : Turn on warnings
# Wextra      : Turn on even more warnings
# DGEKKO      : Define the Gekko preprocessor variable (Wii CPU Chipset)
# mrvl        : Unknonw (???)
# mcpu        : Specify the name of the CPU so compiler knows correct instruction set
# meabi       : Unknown (???)
# mhard-float : Use hardware for floating point operations
CFLAGS=-g -O2 -Wall -Wextra -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float

# Define a build rule that will compile all the source files into an object file
# Create one object file for each source file and the auto generated make dependencies
# $< = name of dependency : $@ = name of target
$(BUILD_DIR)/%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@
	@echo -e "\t $<"

# Define the build rule that generates the .d file for each .c file
# the .d file automatically tracks the dependencies of each .c file
# by reading the #include directives 
# That way we know when a .o file needs rebuildings
$(BUILD_DIR)/%.d: %.c
# Build the dependencies files (MMD MP buid deps) (MF output file) (c compile without linking)
	@$(CC) -MMD -MP -MF $@ $(INCLUDE_DIRS) -c $< 
	
# The dependencies files generated are just plain text that list them like a makefile
# so we can just include them
include $(DEP_FILES)

# Make a phony target that informs the user that compilation has started 
$(OBJ_FILES): compile_informer
.PHONY: compile_informer
compile_informer:
	@echo "Compiling object files ..."