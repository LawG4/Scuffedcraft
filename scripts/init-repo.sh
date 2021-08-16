#!/bin/bash 

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