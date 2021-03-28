# *ScuffedCraft*
Ever wanted to play a Minecraft clone on the Nintendo Wii? Well here is a scuffed version, everything is scuffed from the textures, to the font and to the code. This is why the project is affectionally called *ScuffedCraft*. 

ScuffedCraft is a totally open source voxel game written in C, minor updates are tested primarily via Dolphin, although I plan to test full playable releases on actual hardware, I just don't have any available to me at the moment, if you have a homebrewed Wii, why not give it a run?  

 ![Screenshot](https://media.giphy.com/media/or7Hdux4BRkV1xZ8qj/giphy.gif)

### Gameplay features

- Big plane loaded in, that you can fly around.
- Infinitely loads terrain 
- That is it at the moment.

### Build instructions and environment

So the general layout of the project follows my general code structures for Nintendo Wii development. There is a .vsvode folder because I use VSCode for writing my projects. This has some settings for include directories, intellisense and compilers. My VSCode settings and makefile **REQUIRE** some environment variables to be set.

```makefile
DOLPHIN_PATH = The root folder where dolphin is installed to 
DEVKIT_PATH  = The root folder where Devkit is installed to  
```

The *DEVKIT_PATH* environment variable is a duplicate of the one that is installed via devkit, however that one on Windows points to the MySys2 file path not the windows file path. So in order to use the devkit in windows via command line we need the duplicate variable.

- make 	   : Build the .dol and .elf files
- make run : Build and run the .elf file on dolphin emulator
- make log : Build and run on dolphin but then fetch the log files afterwards
- make hw  : Build the .dol and .elf file and launch the dol via wii_load to the wii with ip : WIILOAD env variable

### Dependencies 

- DevkitPPC
- LibOGC
- GRRLib
- ImDisk (Only for windows for reading the virtual SD Card to get the debug logs)

### Development Goals

Do you want to help contribute to the code base? Here's a list of features I'm planning to add, and it'd be lovely if you'd like to help! Also feel free to go absolutely wild and implement whatever you want! 

- Switch mesh from a linked list of faces to automatically reallocated array of faces
- Make the makefile cross platform as it currently is only built for Windows
- Test the Wii to see if there is a benefit to using quad primitives instead of triangular 
- Storing and loading world files
- Seeds that dictate how a terrain is created
- Entities
- Multiple controller support
- Controller abstraction
- Logging
- Collision
- Mesh freeing 

### Patch notes

#### **0.0.1**

- Major refactor of the codebase to a more expandable and modular structure
- Added chunk memory management - Allocate a fixed size for the currently rendered chunks and then dynamically switch the chunks in and out of memory (Currently we're not actually changing the chunk but just the world matrix since all the terrain looks the same)
- Added an extraordinarily simple logging and retrieval system
- Removed Collision code because it was not inline for the expandability I wanted
- Abstracted block based functionality out of the rendering system
- Removed Cock.png (I had a testing image which was to make sure I could map textures and I forgot to remove the test image)



