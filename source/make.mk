# Define all of the .c files used starting with the main file
CFILES = Main.c 
DEP_FILES = $(BUILD_DIR)/Main.d
# Define the static libraries that we will link against 

# Define the include directories for the static libraries

# Define the build rule that generates the .d file for each .c file
# the .d file automatically tracks the dependencies of each .c file
# by reading the #include directives 
# That way we know when a .o file needs rebuildings
VPATH = source
$(DEP_FILES) : $(BUILD_DIR)/%.d : %.c
	@echo $< > $@