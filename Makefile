# First ensure that the user has correctly created a sc_env.mk file by running the init-repo script
ifeq (,$(wildcard ./build/sc_env.mk))
$(error Could not find environment configuration file, ensure one exists in build/sc_env.mk)
else
endif

# The user has an environment config, so souce it for it's variables
include build/sc_env.mk

# Check that the user has the compiler 
ifeq (, $(wildcard $(DEVKITPPC)/bin/powerpc-eabi-gcc))
$(error Could not find compiler at the right path. Check build/sc_env.mk)
else
CC = $(DEVKITPPC)/bin/powerpc-eabi-gcc
LD = $(CC)
endif


#
# Start defining targets to build, the first being scuffedcraft.dol
#

# Define the default target which is to build the Scuffedcraft release, which depends on the Scuffedcraft.elf
ScuffedCraft.dol: ScuffedCraft.elf
	@echo -e "\e[1m\e[32mCompiling release configuration\e[39m\e[0m" # Green and bold
	@$(DEVKITPRO)/tools/bin/elf2dol $< $@
	@echo "Done!"

# Include the file that defines all of the source files used, or at least all the ones I want to add
# I don't want to use globbing, so that I can have explicit control
BUILD_DIR=build
include source/make.mk

# The scuffedcraft.elf target is dependent on all the object files. 
# it is the binary after all the code has been compiled and linked into one file 
# The list of source files are described in source/make.mk 
ScuffedCraft.elf: inform $(OBJ_FILES) $(CFILES)
	@echo "Linking ..."
	@$(CC) $(OBJ_FILES) $(CFLAGS) $(LINK_DIRS) $(LINK_FLAGS) -o ScuffedCraft.elf
	
# A target that runs the build on dolphin
run: ScuffedCraft.elf
	@echo "Launching ..."
	@$(DOLPHIN_CMD) -b -d ScuffedCraft.elf

# A target to remove all the builds made  
.PHONY: clean
clean:
	@echo "Cleaning intermediates"
	@rm -f $(BUILD_DIR)/*.d
	@rm -f $(BUILD_DIR)/*.o 
	@echo "Cleaning Binaries"
	@rm -f ScuffedCraft.elf
	@rm -f ScuffedCraft.dol
	@echo "Done"

# A phony target that informs the user of the variables and environments used
.PHONY: inform
inform:
	@echo -e "\e[1m\e[32mBuilding ScuffedCraft ...\e[39m\e[0m" # Green and bold
	@echo "Using DevkitPro path : $(DEVKITPRO)"
	@echo -e "Compiler path : $(DEVKITPPC)/bin/powerpc-eabi-gcc\n"