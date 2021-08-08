#!/bin/bash 

LOCAL=$(dirname "$0")"/"
REPO_ROOT="${LOCAL}../"
source ${LOCAL}common.sh

# Run the dolphin command set by the configuration file
"${SC_DOLPHIN_CMD}" "--batch" "--exec=${REPO_ROOT}Scuffedcraft.dol"