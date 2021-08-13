# First ensure that the user has correctly created a sc_env.mk file by running the init-repo script
$(info Checking for ScuffedCraft environment configuration file)
ifeq (,$(wildcard ./build/sc_env.mk))
$(error Could not find environment configuration file, ensure one exists in build/sc_env.mk)
else
$(info Found!)
endif

# The user has an environment config, so souce it for its variables
include build/sc_env.mk

# Include the file that defines all of the source files used, or at least all the ones I want to add
# I don't want to use globbing, so that I can have explicit control
BUILD_DIR=build
include source/make.mk

# Check that the user has the compiler 
$(info Using DevkitPro path : $(DEVKITPRO))
ifeq (, $(wildcard $(DEVKITPPC)/bin/powerpc-eabi-gcc))
$(error Could not find compiler at the right path. Check build/sc_env.mk)
else
$(info Compiler path : $(DEVKITPPC)/bin/powerpc-eabi-gcc)
CC = $(DEVKITPPC)/bin/powerpc-eabi-gcc
endif

# Define the default target which is to build the Scuffedcraft release, which depends on the Scuffedcraft.elf
ScuffedCraft.dol: ScuffedCraft.elf
	@echo -e "\e[1m\e[32mCompiling release configuration\e[39m\e[0m" # Green and bold
	@echo $(SC_DOLPHIN_CMD)


# The scuffedcraft.elf target is dependent on all the object files. 
# it is the binary after all the code has been compiled and linked into one file 
# The list of source files are described in source/make.mk 
ScuffedCraft.elf: FORCE $(DEP_FILES)
	@echo -e "\e[1m\e[32mBuilding ScuffedCraft ...\e[39m\e[0m" # Green and bold
	@echo "Temp" > ScuffedCraft.elf
	@echo "$(DEP_FILES)"

# A force target, which just always runs so that ScuffeCraft.elf always runs
# For debug purposes
FORCE: ;

