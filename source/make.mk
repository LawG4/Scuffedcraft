#
# I want to keep exact explicit control over the files that get compiled into SC
# For each subdirectory we need to add :
#	VPATH        : Where make looks for the c files
#	INCLUDE_DIRS : Where the compiler looks for header files
#	cFILES       : List of source files
#	

# Root directory of source tree
VPATH = source
INCLUDE_DIRS = -Isource
CFILES = Main.c 

# Render subdirectory 
VPATH += source/Render
INCLUDE_DIRS += -Isource/Render
CFILES +=Render.c block.c Mesh.c

# Physics subdirectory
VPATH += source/Physics
INCLUDE_DIRS += -Isource/Physics
CFILES += Collision.c

# Logging
VPATH += source/Logs
INCLUDE_DIRS += -Isource/Logs
CFILES += Log.c

# gfx
VPATH += source/gfx
INCLUDE_DIRS += -Isource/gfx
CFILES += albedo_cube.c font.c

# Entities
VPATH += source/Entities
INCLUDE_DIRS += -Isource/Entities
CFILES += Player.c

# Chunks
VPATH += source/Chunks
INCLUDE_DIRS += -Isource/Chunks
CFILES += Chunk.c ChunkMemoryManagement.c

#
# Create a list of flags that will get passed to the compiler 
# The first being the C flags, the next being the linker flags
#

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

# grrlib : The graphics assistance library
LINK_FLAGS=-lgrrlib -lfreetype -lbz2 -lpngu -lpng -ljpeg -lz -lfat -lwiiuse -lbte -logc -lm


#
# Add the External directories, Tells the compiler where to look for header files
# and where to look for the external libraries to link against
#

# Include directories for external headers 
# libogc the library for interfacing with the Wii hardware
INCLUDE_DIRS+=-I$(DEVKITPRO)/libogc/include
# GRRLib the graphics assitance library
INCLUDE_DIRS+=-Iexternal/GRRLIB/GRRLIB/GRRLIB

# Link directories for external libraries
# libogc
LINK_DIRS+=-L$(DEVKITPRO)/libogc/lib/wii
# Porlibs, the ported standard libraries 
LINK_DIRS+=-L$(DEVKITPRO)/portlibs/wii/lib
LINK_DIRS+=-L$(DEVKITPRO)/portlibs/ppc/lib
# Grrlib
INCLUDE_DIRS+=-Lexternal/GRRLIB/GRRLIB/GRRLIB


#
# Static rules, these will very rarely change
#

# For each of the source files we need to automatically generate the 
# dependencies files and the object files. 
# Dependency files track the files that each source file depeneds on
# The object files are compiled source files 
# Automatically build a list of these files from the list of source files
DEP_FILES = $(patsubst %.c, $(BUILD_DIR)/%.d, $(CFILES))
OBJ_FILES = $(patsubst %.c, $(BUILD_DIR)/%.o, $(CFILES))

# Define a build rule that will compile all the source files into an object file
# Create one object file for each source file and the auto generated make dependencies
# $< = name of dependency : $@ = name of target
$(BUILD_DIR)/%.o: %.c
	@echo -e "\t $<"
	@$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@
	
# Define the build rule that generates the .d file for each .c file
# the .d file automatically tracks the dependencies of each .c file
# by reading the #include directives 
# That way we know when a .o file needs rebuildings
$(BUILD_DIR)/%.d: %.c
# Build the dependencies files (MMD MP buid deps) (MF output file) (c compile without linking)
	@$(CC) -MMD -MP -MF $@ $(INCLUDE_DIRS) -c $< -o build/intermediate.o
		
# The dependencies files generated are just plain text that list them like a makefile
# so we can just include them
include $(DEP_FILES)

# Make a phony target that informs the user that compilation has started 
.PHONY: compile_informer
compile_informer:
	@echo "Compiling object files ..."