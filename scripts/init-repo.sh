#!/bin/bash 

# Do another root access check
if [ "$EUID" -ne 0 ]
  then echo -e "${TERM_RED}Run this script as root, install sudo and run : \nsudo -E bash scripts/init-repo.sh${TERM_NOC}"
  exit
fi

# Run the install dependencies script
LOCAL=$(dirname "$0")"/"
REPO_ROOT="${LOCAL}../"
source ${LOCAL}install.sh

# Now continue with the repo set up

echo -e "\n========================================================="
echo "=          Initialising the ScuffedCraft Repo           ="
echo "========================================================="
source ${LOCAL}common.sh
#========================================================
# Build grrlib
#========================================================
echo -n "* Building GRRLIB : "
cd "${REPO_ROOT}external/GRRLIB/GRRLIB"
make clean all install &> /dev/null

if [ $? -eq 0 ]; then
	echo -e "${TERM_GRN}Done!${TERM_NOC}"
else
	echo -e "${TERM_RED}Fail!${TERM_NOC}"
 exit
fi
cd - &> /dev/null

# Create CMakeConfig
CMAKE_DIR="${REPO_ROOT}cmake/"
echo -e "set(DEVKITPRO ${DEVKITPRO}) \nset(DEVKITARM ${DEVKITARM}) \nset(DEVKITPPC ${DEVKITPPC}) \nset(SC_DOLPHIN_CMD) ${SC_DOLPHIN_CMD})" > ${CMAKE_DIR}sc_env.cmake