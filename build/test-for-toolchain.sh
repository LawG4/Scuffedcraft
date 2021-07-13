#!/bin/bash 
source common.sh

echo "Testing for toolchain support ..."

# Test which operating system we're on
SC_SYSTEM_NAME="$(uname -s)"

# Uname failed, we have no hope of continuing further 
if [ $? -ne 0 ]
then
    echo -e "${TERM_RED}Uname failed. No hope to continue. File a bug report.${TERM_NOC}"
    exit 1
fi


# first test if we're on windows. Although devkit pro exists on windows and provides a full build 
# system through msys2, which is a very useful, msys2 has some weird behaviours and occasionally 
# moving between the msys2 environement and windows causes some troubles, For example the path to 
# the devkit toolchain is local to msys2. So the compiler and the IDE would need different include 
# paths for highlighting 
SC_WINDOWS_FOUND=0
case "${SC_SYSTEM_NAME}" in 
	MSYS*)		SC_WINDOWS_FOUND=1;;
	MINGW*) 	SC_WINDOWS_FOUND=1;;
	CYGWIN*)	SC_WINDOWS_FOUND=1;;
	Windows*)	SC_WINDOWS_FOUND=1;;
	WINDOWS*)	SC_WINDOWS_FOUND=1;;
esac
if [ ${SC_WINDOWS_FOUND} -ne 0 ];
then
	echo -e "${TERM_RED}>>> FAIL! ${TERM_NOC}"
	echo "Detected a native windows shell, this is not supported, please WSL instead."
	exit 1
fi


# From this point onward we know we are either on Linux, WSL, Mac.
# We don't know yet if we have to block apple silicone yet. 

# Test for the environement that points to toolchain for compiling to wii
if [[ -z "${DEVKITPPC}" ]];
then
	echo -e "${TERM_RED}>>> FAIL! ${TERM_NOC}"
	echo "DeckitPPC environement variable not set. required for execution"
	exit 1
fi

# Test that the compiler is actually at the location we pointed to 
if ! command -v "${DEVKITPPC}/bin/powerpc-eabi-gcc" &> /dev/null
then
	echo -e "${TERM_RED}>>> FAIL! ${TERM_NOC}"
	echo "PowerPc compiler not found at DevkitPPC environement variable location, please set it correctly."
	echo "${DEVKITPPC}/bin/powerpc-eabi-gcc"
	exit 1
fi

# Test that dolphin is installed unless the user specified they don't want to install dolphin
if [[ -z "${SC_NO_DOLPHIN}" ]];
then
	# Using Dolphin so check for it
	if ! command -v "${SC_DOLPHIN}" &> /dev/null
	then
		echo -e "${TERM_RED}>>> FAIL! ${TERM_NOC}"
		echo "Could not find Dolphin, using path"
		echo "${SC_DOLPHIN}"
	fi
else
	# Not using dolphin so warn the user.
	echo -e "${TERM_YLW}[Warning]${TERM_NOC} Deciding not to use dolphin, run commands won't work"
fi

# I want to remove dependance on GRRLIB as I only use it to switch off anti aliasing.

# Haven't exited yet, so we have full toolchain support
echo -e "${TERM_GRN}Toolchain support verified${TERM_NOC}"