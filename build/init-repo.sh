#!/bin/bash 
source common.sh

# This script is used to set up the users computer, so that all the make commands can run if all the dependencies have been installed
if [[ -z "${SC_NO_LOG_FILE}" ]];
then 
	# Check if the user has set a mount point file name 
	if [[ -z "${SC_LOG_MOUNT_POINT}" ]];
	then 
		echo -e "${TERM_RED}No mount point path variable set.${TERM_NOC}"
		echo "set a value for SC_LOG_MOUNT_POINT, or disable logging"
		exit 1
	fi

	# Check if the mount directory already exists
	if [[ -d "${SC_LOG_MOUNT_POINT}" ]]; then
		echo -e "${TERM_YLW}[Warning] Logging mount point already exists${TERM_NOC}"
		echo "SC_LOG_MOUNT_POINT = ${SC_LOG_MOUNT_POINT} "
	else
		sudo mkdir "${SC_LOG_MOUNT_POINT}"
		# Check if the mount directory was made successfully 
		if [ -d "${SC_LOG_MOUNT_POINT}" ]; then
			echo -e "${TERM_GRN}Successfully made a logging mount directory${TERM_NOC}"
			echo "Logging mount directory : ${SC_LOG_MOUNT_POINT}"
		else
			echo -e "${TERM_RED}Could not create logging directory${TERM_NOC}"
			echo "Using SC_LOG_MOUNT_POINT = ${SC_LOG_MOUNT_POINT}"
			echo "Either make it manually or disable logging"
			exit 1
		fi
	fi
else
	echo -e "${TERM_YLW}[Warning] Logging Disabled.${TERM_NOC}"
fi

# Now check for toolchain support 
source test-for-toolchain.sh

# if we made it this far then assume repo was initialised propperly
echo -e "${TERM_GRN}Repo init successfully ${TERM_NOC}"