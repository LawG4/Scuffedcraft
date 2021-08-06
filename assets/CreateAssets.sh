#!/bin/bash

#
# Standard blocks
#

# Gen the texture atlas and indices, need to be in the working directory or else we can't find the json file
cd standard-blocks
python3 Main.py
cd ..

# Turn the outputted bitmap into a C file resource
"${DEVKITPRO}/tools/bin/raw2c" "standard-blocks/StandardBlocksTextureAtlas.bmp"

# Copy the atlas to the standard blocks folder 
cp "StandardBlocksTextureAtlas.c" "../source/Assets/StandardBlocks"
cp "StandardBlocksTextureAtlas.h" "../source/Assets/StandardBlocks"

# Remove the converted atlas files
rm "StandardBlocksTextureAtlas.c"
rm "StandardBlocksTextureAtlas.h"