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
# Define the static libraries that we will link against 


# Define the build rule that generates the .d file for each .c file
# the .d file automatically tracks the dependencies of each .c file
# by reading the #include directives 
# That way we know when a .o file needs rebuildings

$(BUILD_DIR)/%.d: %.c
	@echo $< > $@
	@echo "< : $<"
	@echo "@ : $@"
	@echo "$(DEP_FILES)"