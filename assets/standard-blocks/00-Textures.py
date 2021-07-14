import json
import math
from PIL import Image #Requires pip install Pillow
import os
# So here's how the structure of this program works. We want to read in the data inside the json file and use that to build one main texutre atlas for the standard blocks.
# for each standard block I will produce a file containing the texture coordinates of each block, decided apon by the json format file.

# Then there will be a standard block header which will define the model space coordinates for the standard block, and include the texture atlas and also all the other texture coordinates

# Read in the json config file
file = open('00-Textures.json',)
blockData = json.load(file)
file.close()

# Loop through the json list and get all of the texture names mentioned
textureNames = []
for x in blockData['standard-blocks']:
	textureNames.append(x['block-texture-n'])
	textureNames.append(x['block-texture-e'])
	textureNames.append(x['block-texture-s'])
	textureNames.append(x['block-texture-w'])
	textureNames.append(x['block-texture-t'])
	textureNames.append(x['block-texture-b'])

# Now get only the unique texture names
uniqueTextureNames = list(set(textureNames))

# Find the first square number larger than the number of unique textures
atlasWidth = math.ceil(math.sqrt(len(uniqueTextureNames)))
# This might need rounding up to the next power of two as well

# Set the size of the face in pixels 
faceSizeInPixels = 16

# Padding for the textures
texturePad = 1

# Calculate the size of the texture atlas 
atlasSize = (atlasWidth * (faceSizeInPixels + 2 * texturePad), atlasWidth * (faceSizeInPixels + 2 * texturePad))
atlasImage = Image.new( mode="RGB", size=atlasSize )

print("Placing ", len(uniqueTextureNames), " textures into a texture atlas of size : (", atlasSize[0], ", ", atlasSize[1], ")")

# Go by the left top corner of the texture
x = texturePad
y = texturePad

# Go through each unique texutre 
for textureName in uniqueTextureNames:
	# Test if the texture from the json file exists
	if not os.path.isfile(textureName):
		textureName = "99-UnknownTexture.png"
	
	# Copy the image to the texture atlas
	newTexture = Image.open(textureName)
	atlasImage.paste(newTexture, (x,y), newTexture)
	newTexture.close()

	# Move the texture coordinate for the next image
	x = x + faceSizeInPixels + texturePad
	if x + faceSizeInPixels + texturePad > atlasSize[0]:
		x = texturePad
		y = y + faceSizeInPixels + texturePad




print("Saving output image")
atlasImage.save('out.bmp')