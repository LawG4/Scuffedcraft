#!/bin/bash 

# Overridable configurable options. 
# Used for testing for toolchain support, getting include paths, and running dolphin.
# Uncomment what you need

# If using this variable the location to call dolphin will be overriden
# export SC_FORCE_DOLPHIN_PATH=

# If using this variable, the location of the binary that'll be run by dolphin
# export SC_FORCE_BINARY=

# If the user doesn't want to require dolphin and just wants to build the game
# export SC_NO_DOLPHIN=true

# IP address used by Wii load that'll 
# export WIILOAD=tcp:192.168.0.1

# If the user doesn't want to enable logging, because it requires mounting a file produced by dolphin as if it was an external drive 
# export SC_NO_LOG_FILE=true

# The file path to mount the Wii file system as
export SC_LOG_MOUNT_POINT=/mnt/wii/ # The mount point that 

# Editable variables finished.......................................................................................................

# Detect if we are in WSL
if [[ -n "$IS_WSL" || -n "$WSL_DISTRO_NAME" ]]; then
    export SC_WSL=true
fi

# Set the path to dolphin as that migt change across platforms 
if [[ -z "${SC_FORCE_DOLPHIN_PATH}" ]];
then
		if [ "${SC_WSL}"=true ];
	then
		export SC_DOLPHIN="/mnt/c/Program Files/Dolphin/Dolphin.exe"
	else
		export SC_DOLPHIN="dolphin"
	fi
else
	#Force environment variable set so override
	export SC_DOLPHIN=${SC_FORCE_DOLPHIN_PATH}
fi