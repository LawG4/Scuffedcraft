# *ScuffedCraft*
Ever wanted to play a Minecraft clone on the Nintendo Wii? Well here is a scuffed version, everything is scuffed from the textures, to the font and to the code. This is why the project is affectionally called *ScuffedCraft*. 

ScuffedCraft is a totally open source voxel game written in C, minor updates are tested primarily via Dolphin, although releases are tested on a homebrewed Nintendo Wii. *note, this project is not affiliated with Nintendo.*

![ScuffedCraft Works](https://media.giphy.com/media/RcW9HlcwmSmcjsIqlR/giphy.gif)

> ***IT WORKS ON REAL HARDWARE***

### Build instructions

```shell
# Pull the repo
git clone https://github.com/LawG4/Scuffedcraft.git
cd Scuffedcraft
# Install and build all dependencies
sudo -E bash scripts/init-repo.sh
# Build ScuffedCraft
bash scripts/build.sh
# Run ScuffedCraft on Dolphin. On WSL make sure to edit scripts/sc_env.sh first
bash scripts/run.sh
```

If you want to install everything yourself, or you're using an unsupported platform, I am working on detailed manual instructions.

### Gameplay features

- Big plane loaded in, that you can fly around.
- Infinitely loads flat terrain
- Works on real hardware!!!! 
- That is it at the moment.

### Development Goals

Do you want to help contribute to the code base? Here's a list of features I'm planning to add, and it'd be lovely if you'd like to help! Also feel free to go absolutely wild and implement whatever you want! 

- Switch mesh from a linked list of faces to automatically reallocated array of faces
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

- Supplying bash scripts to init the repo
- Automatically generate a texture atlas for the standard blocks

#### **0.0.1**

- Major refactor of the codebase to a more expandable and modular structure
- Added chunk memory management - Allocate a fixed size for the currently rendered chunks and then dynamically switch the chunks in and out of memory (Currently we're not actually changing the chunk but just the world matrix since all the terrain looks the same)
- Added an extraordinarily simple logging and retrieval system
- Removed Collision code because it was not inline for the expandability I wanted
- Abstracted block based functionality out of the rendering system
- Removed Cock.png (I had a testing image which was to make sure I could map textures and I forgot to remove the test image)



