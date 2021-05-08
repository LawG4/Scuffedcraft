# *ScuffedCraft*
Ever wanted to play a Minecraft clone on the Nintendo Wii? Well here is a scuffed version, everything is scuffed from the textures, to the font and to the code. This is why the project is affectionally called *ScuffedCraft*. 

ScuffedCraft is a totally open source voxel game written in C, minor updates are tested primarily via Dolphin, although releases are tested on a homebrewed Nintendo Wii. *note, this project is not affiliated with Nintendo.*

<blockquote class="twitter-tweet"><p lang="en" dir="ltr">In other news, scuffedcraft runs on real hardware!! In an incredibly on-brand move, I only have the teeniest little screen salvaged from a car reversing camera kit <a href="https://t.co/2i5kFReEw7">pic.twitter.com/2i5kFReEw7</a></p>&mdash; Lawrence (@Lawrence9800) <a href="https://twitter.com/Lawrence9800/status/1389253026389663745?ref_src=twsrc%5Etfw">May 3, 2021</a></blockquote> <script async src="https://platform.twitter.com/widgets.js" charset="utf-8"></script>

### Gameplay features

- Big plane loaded in, that you can fly around.
- Infinitely loads flat terrain
- Works on real hardware!!!! 
- That is it at the moment.

### Build instructions and environment

*`*I am slowly begging the move to CMake to simplify this process. The main problem is the devkitPro install on windows sets it's environment variables to be local to the UNIX file system within MSYS2. As a result make works, but the devkit environment variables can't be used for include paths. So for windows users set WIN_DEVKITPRO environment variable. The Linux make file has problems reaching dolphin, this is another problem I want to be able to fix at some point. Hopefully the move to CMake will facilitate this.*`*

So the general layout of the project follows my general code structures for Nintendo Wii development. There is a .vsvode folder because I use VSCode for writing my projects. This has some settings for include directories, intellisense and compilers. My VSCode settings and makefile **REQUIRE** some environment variables to be set on Windows. On Linux Autocomplete will work fine, but dolphin won't be launched.

```makefile
DOLPHIN_PATH  = The root folder where dolphin is installed to 
WIN_DEVKITPRO = The root folder where Devkit is installed to  
```

The *WIN_DEVKITPRO* environment variable is a duplicate of the one that is installed via devkit, however that one on Windows points to the MySys2 file path not the windows file path. So in order to use the devkit in windows via command line we need the duplicate variable.

- make 	   : Build the .dol and .elf files
- make run : Build and run the .elf file on dolphin emulator
- make log : Build and run on dolphin but then fetch the log files afterwards
- make hw  : Build the .dol and .elf file and launch the dol via wii_load to the wii with ip : WIILOAD env variable

### Dependencies 

- DevkitPPC
- LibOGC
- GRRLib
- ImDisk (Only for windows for reading the virtual SD Card to get the debug logs)
- Dolphin for testing

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

#### **0.0.2** *in progress*

- Started moving to CMake

#### **0.0.1**

- Major refactor of the codebase to a more expandable and modular structure
- Added chunk memory management - Allocate a fixed size for the currently rendered chunks and then dynamically switch the chunks in and out of memory (Currently we're not actually changing the chunk but just the world matrix since all the terrain looks the same)
- Added an extraordinarily simple logging and retrieval system
- Removed Collision code because it was not inline for the expandability I wanted
- Abstracted block based functionality out of the rendering system
- Removed Cock.png (I had a testing image which was to make sure I could map textures and I forgot to remove the test image)



