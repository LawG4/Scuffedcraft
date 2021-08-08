#!/bin/bash

#
# The whole point of this script is to ensure that the user has all of the dependencies required
# Cross compiling is hard enough, working with all of the toolchains is hard enough.
# I want scuffedCraft to be a project literally anyone could pick up
# To this end I want to automate installing all the dependencies
# I will at least assume the user has bash 
#

# Colour terminal codes
TERM_BLK='\033[0;30m'
TERM_RED='\033[0;31m'
TERM_GRN='\033[0;32m'
TERM_YLW='\033[0;33m'
TERM_BLU='\033[0;34m'
TERM_NOC='\033[0;0m'

echo -e "========================================================="
echo -e "= Checking and installing all ScuffedCraft dependencies ="
echo -e "========================================================="

LOCAL=$(dirname "$0")"/"


# Ensure we are running as root for installing things
if [ "$EUID" -ne 0 ]
  then echo -e "${TERM_RED}Run this script as root, install sudo and run : \nsudo -E bash install.sh${TERM_NOC}"
  exit
fi

# Check if the user has any of the devkit environement variables set.
# if they don't, set them to a default value and write them to a config
if [[ -z "${DEVKITPRO}" ]]; then
	# No DEVKITPRO Variable
	DEVKITPRO=/opt/devkitpro
fi
if [[ -z "${DEVKITARM}" ]]; then
	# No devkitARM Variable
	DEVKITARM=/opt/devkitpro/devkitARM
fi
if [[ -z "${DEVKITPPC}" ]]; then
	# No DEVKITPPC Variable
	DEVKITPPC=/opt/devkitpro/devkitPPC
fi

# Write the config to a shell script we can source from here on out.
echo "Devkit Environment variables :"
echo -e "DEVKITPRO = ${TERM_YLW}${DEVKITPRO}${TERM_NOC}"
echo -e "DEVKITARM = ${TERM_YLW}${DEVKITARM}${TERM_NOC}"
echo -e "DEVKITPPC = ${TERM_YLW}${DEVKITPPC}${TERM_NOC}\n"

echo -e "export DEVKITPRO=${DEVKITPRO}\nexport DEVKITARM=${DEVKITARM}\nexport DEVKITPPC=${DEVKITPPC}" > ${LOCAL}sc_env.sh
echo -e "export PATH=\"${DEVKITPRO}:${DEVKITARM}:${DEVKITPPC}:\$PATH\"" >> ${LOCAL}sc_env.sh
echo -e "export PATH=\${DEVKITPRO}/tools/bin:\$PATH" >> ${LOCAL}sc_env.sh
source ${LOCAL}sc_env.sh

#==================================================================================================
# First Determine which type of system we are on
# This is used to determine the primary package manager, and the install command to use that pkg manager
# PKG_INS PKG_ARG are the variables we're looking to define 
#==================================================================================================

case "$OSTYPE" in
	# Found Linux the ideal set up
	linux*)
		echo -e "${TERM_GRN}Detected Linux!${TERM_NOC}"
		# Detect if we are on debian, in that case we can use the debian binary
		if [ "$(grep -Ei 'debian|buntu|mint' /etc/*release)" ]; then
			# Detected that we are on debian
			SC_HOST_SYSTEM="Debian"
			PKG_INS="apt-get"
			PKG_ARG="-y install"
			echo -e "${TERM_GRN}Detected Debian based system!${TERM_NOC}"
		elif [ "$(grep -Ei 'fedora|redhat|red' /etc/*release)" ]; then
			# Detected a redhat system
			SC_HOST_SYSTEM="Redhat"
			PKG_INS="dnf"
			PKG_ARG="install"
			echo -e "${TERM_GRN}Detected Redhat based system!${TERM_NOC}"
		elif [ "$(grep -Ei 'arch' /etc/*release)" ]; then
			# Detected arch
			SC_HOST_SYSTEM="Arch"
			PKG_INS="pacman"
			PKG_ARG="-S --noconfirm"
			echo -e "${TERM_GRN}Detected Arch based system!${TERM_NOC}"
		else
			# Unknown Linux Flavour
			SC_HOST_SYSTEM="Unkown"
			echo -e "${TERM_YLW}Could not determine your linux flavour!\nIf you have pacman we'll attempt to install${TERM_NOC}"
		fi
		;;
	darwin*)
		SC_HOST_SYSTEM="OSX"
		echo -e "${TERM_GRN}Detected OSX!${TERM_NOC}"
		echo -e "${TERM_YLW}I don't have a mac, this may break. Sorry!${TERM_NOC}"
		if ! type "brew" > /dev/null; then
			echo -e "${TERM_RED}Homebrew not detected, I can't install that for you. install Homebrew.${TERM_NOC}"
			exit
		fi
		PKG_INS="brew"
		PKG_ARG="install"
		;;
	msys*)
		echo -e "${TERM_RED}Detected Windows through msys2 compatability layer, not supported${TERM_NOC}"
		exit ;;
	bsd*)
		echo -e "${TERM_RED}Detected BSD, I'm sure you can figure out installing dependencies${TERM_NOC}"
		exit ;;
	solaris*)
		echo -e "${TERM_RED}Detected Solaris, I'm sure you can figure out installing dependencies${TERM_NOC}"
		exit ;;
	*) 
		echo -e "${TERM_RED}Unknown Operating System : ${OSTYPE}${TERM_NOC}"
		exit ;;
esac

# There was a lot of branches in the above so just ensure that we have the required variables set
if [[ -z "${PKG_INS}" ]]; then
	echo -e "${TERM_RED}Something went wrong finding the package manager${TERM_NOC}"
	exit
fi

if [[ -z "${SC_HOST_SYSTEM}" ]]; then
	echo -e "${TERM_RED}Something went wrong determining the system${TERM_NOC}"
	exit
fi

#==================================================================================================
# Install the method of getting devkit packages
# On debian and macOS that involves getting a special program called dkp-pacman
# On Redhat install regular pacman
# Redhat, arch and linux undetermined add the sources to pacman
#
# We want to define the variable
# PAC_CMD to define the pacman command
#==================================================================================================

#==================================================================================================
# Install dkp-pacman on systems that need it
#==================================================================================================
if [[ "${SC_HOST_SYSTEM}" == "Debian" ]] || [[ "${SC_HOST_SYSTEM}" == "OSX" ]]; then
	# If we don't have dkp-pacman we're gonna have to install that from the github
	echo -n "* Detecting dkp-pacman : "
	if type "dkp-pacman" > /dev/null; then
		echo -e "${TERM_GRN}Found!${TERM_NOC}"
	else
		echo -e "${TERM_YLW}Not Found!${TERM_NOC}"
		# To install dkp-pacman we're gonna need Curl to pull the binaries.
		if ! type "wget" > /dev/null; then
			"${PKG_INS} wget"
		fi

		# Set the name for the correct binary to pull
		if [[ "${SC_HOST_SYSTEM}" == "OSX" ]]; then
			DKP_PKG_NAME="devkitpro-pacman-installer.pkg"
			echo "${SC_HOST_SYSTEM}"
		else
			# Determine the package name based on the architecture
			case $(uname -m) in
				amd64 | x86_64) 
					DKP_PKG_NAME="devkitpro-pacman.amd64.deb";;
				armv8 | aarch64)
					DKP_PKG_NAME="devkitpro-pacman.arm64.deb";;
				*) 
					echo -e "${TERM_RED}Architecture not supported, what are you running???${TERM_NOC}"
					exit ;;
			esac
		fi

		# Pull the binary for devkit via wget
		echo -e -n "\tPulling ${DKP_PKG_NAME} : "
		wget -q "https://github.com/devkitPro/pacman/releases/download/v1.0.2/${DKP_PKG_NAME}" -O "${LOCAL}${DKP_PKG_NAME}" > /dev/null


		# Ensure that the binary was downloaded 
		if [ ! -f "${LOCAL}${DKP_PKG_NAME}" ]; then
			echo -e "${TERM_RED}Failed!${TERM_NOC}"
			exit
		else
			echo -e "${TERM_GRN}Done!${TERM_NOC}"
		fi

		# Install the binary 
		echo -e -n "\tInstalling dkp-pacman : "
		if [[ "${SC_HOST_SYSTEM}" == "OSX" ]]; then
			installer -pkg devkitpro-pacman-installer.pkg -target /
		else 
			# on debian so check for gdebi 
			if ! type "gdebi" > /dev/null; then
				"${PKG_INS} gdebi-core"
			fi

			# Check on WSL, because symnlink might be needed 
			if grep -q Microsoft /proc/version; then
  			ln -s /proc/self/mounts /etc/mtab
			fi

			# Run gdebi to install
			gdebi -n "${DKP_PKG_NAME}" > /dev/null
		fi

		# Check if dkp-pacman was installed or else exit the script
		if type "dkp-pacman" > /dev/null; then
			echo -e "${TERM_GRN}Done!${TERM_NOC}"
			PAC_CMD="dkp-pacman"
		else
			echo -e "${TERM_RED}Fail!!!${TERM_NOC}\nCould not verify that devkit was installed"
			exit
		fi

	fi
#==================================================================================================
# Install and configure regular pacman
#==================================================================================================
else
	# If we're on redhat then install pacman 
	if [[ "${SC_HOST_SYSTEM}" ==  Redhat ]]; then
		dnf install pacman
		pacman-key --init
	fi

	# Now ensure that the system has pacman 
	if ! type "pacman" > /dev/null; then
		echo -e "${TERM_RED}ERROR\n{TERM_NOC}Somehow pacman is not installed"
		exit
	else 
		PAC_CMD="pacman"
	fi

	# Install wget
	echo -e -n "* Checking for wget : "
	if ! type "wget" > /dev/null; then
		echo -e "${TERM_YLW}Not Found!${TERM_NOC}"
		echo -e "\tInstalling wget : "
		\"yes | pacman -S "wget"\" > /dev/null 
		# one final check 
		if ! type "wget" > /dev/null; then
			echo -e "${TERM_RED}\n\tFailed to install wget${TERM_NOC}"
			exit
		else
			echo -e "${TERM_GRN}Success!${TERM_NOC}"
		fi
	else
		echo -e "${TERM_GRN}Found!${TERM_NOC}"
	fi

	# Download the pacman keys
	echo -e -n "* Downloading pacman keys : "
	wget -q https://pkg.devkitpro.org/devkitpro-keyring.pkg.tar.xz -O ${LOCAL}devkitpro-keyring.pkg.tar.xz
	if [ ! -f "${LOCAL}devkitpro-keyring.pkg.tar.xz" ]; then
		echo -e "${TERM_RED}Failed!${TERM_NOC}"
		exit
	else
		echo -e "${TERM_GRN}Done!${TERM_NOC}"
	fi

	# Pacman has been installed so lets configure it.
	# Apparently there's a chance these keys will fail
	echo -e -n "* Installing the pacman keys : "
	pacman-key --recv BC26F752D25B92CE272E0F44F7FD5492264BB9D0 --keyserver keyserver.ubuntu.com &> /dev/null
	pacman-key --lsign BC26F752D25B92CE272E0F44F7FD5492264BB9D0  &> /dev/null
	yes | pacman -U devkitpro-keyring.pkg.tar.xz &> /dev/null
	echo -e "${TERM_GRN}Done!${TERM_NOC}"

	# Update the confs
	echo -e -n "* Updating pacman confs : "
	PAC_CONF="[dkp-libs]\nServer = https://pkg.devkitpro.org/packages"
	if ! grep -qxF "Server = https://pkg.devkitpro.org/packages" /etc/pacman.conf; then
		echo -e "${PAC_CONF}" >> /etc/pacman.conf
	fi
	PAC_CONF="[dkp-linux]\nServer = https://pkg.devkitpro.org/packages/linux/\$arch/"
	if ! grep -qxF "Server = https://pkg.devkitpro.org/packages/linux/\$arch/" /etc/pacman.conf; then
		echo -e "${PAC_CONF}" >> /etc/pacman.conf
	fi
	echo -e "${TERM_GRN}Done!${TERM_NOC}"

	# Sync the pacman database 
	echo -e -n "* Syncing the pacman database (Might take some time): "
	yes | pacman -Syu &> /dev/null
	# Just ensure that everything has worked so far
	if [ $? -eq 0 ]; then
    echo -e "${TERM_GRN}Done!${TERM_NOC}"
	else
    echo -e "${TERM_RED}Fail!${TERM_NOC}"
    exit
	fi
fi

# Now let's ensure that this version of pacman has been configured correctly!
echo -e -n "* Ensuring devkit packages : "
if ${PAC_CMD} -Sl dkp-libs | grep -q "libogc"; then
	echo -e "${TERM_GRN}Done!${TERM_NOC}"
else
	echo -e "${TERM_RED}Fail!${TERM_NOC}"
  exit
fi

#==================================================================================================
# Pacman has now been installed either on dkp-pacman, or using pacman and configuring it 
# PAC_CMD and PKG_INS and PKG_ARG should have been writen to by now
# We now have all the tools to install everything we need. 
#==================================================================================================

# Install devkit for wii dev
echo -e -n "* Installing wii-dev : "
${PAC_CMD} -S --noconfirm "wii-dev" &> /dev/null 
echo -e "${TERM_GRN}Done!${TERM_NOC}"

echo -e -n "* Installing libogc : "
${PAC_CMD} -S --noconfirm "libogc" &> /dev/null 
if [ $? -eq 0 ]; then
  echo -e "${TERM_GRN}Done!${TERM_NOC}"
else
  echo -e "${TERM_RED}Fail!${TERM_NOC}"
  exit
fi

# Ensure that the user has git
echo -e -n "* Checking for Git : "
if ! type "git" &> /dev/null; then
	echo -e "${TERM_YLW}Not Found!${TERM_NOC}"
	echo -e -n "\tInstalling git : "
	${PKG_INS} ${PKG_ARG} "git" &> /dev/null 
	# one final check 
	if ! type "git" &> /dev/null; then
		echo -e "${TERM_RED}\n\tFailed to install git${TERM_NOC}"
		exit
	else
		echo -e "${TERM_GRN}Success!${TERM_NOC}"
	fi
else
	echo -e "${TERM_GRN}Found!${TERM_NOC}"
fi

# Use git to doenload the submodules
echo -e -n "* Downloading submodules : "
git submodule update --init --recursive &> /dev/null
echo -e "${TERM_GRN}Done${TERM_NOC}"

# GRRLib dependencies install
echo -e -n "* Installing grrlib dependencies : "
${PAC_CMD} --sync --needed --noconfirm libfat-ogc ppc-libpng ppc-freetype ppc-libjpeg-turbo &> /dev/null 
if [ $? -eq 0 ]; then
  echo -e "${TERM_GRN}Done!${TERM_NOC}"
else
  echo -e "${TERM_RED}Fail!${TERM_NOC}"
  exit
fi

# Ensure that the user has make
echo -e -n "* Checking for make : "
if ! type "make" &> /dev/null; then
	echo -e "${TERM_YLW}Not Found!${TERM_NOC}"
	echo -e -n "\tInstalling make : "
	${PKG_INS} ${PKG_ARG} "make" &> /dev/null 
	# one final check 
	if ! type "make" &> /dev/null; then
		echo -e "${TERM_RED}\n\tFailed to install make${TERM_NOC}"
		exit
	else
		echo -e "${TERM_GRN}Success!${TERM_NOC}"
	fi
else
	echo -e "${TERM_GRN}Found!${TERM_NOC}"
fi

#==================================================================================================
# Install dolphin emulator so that the user can run their builds
#==================================================================================================

# If on WSL we can't install that for you.
if [[ $(grep microsoft /proc/version) ]] || [[ $(grep Microsoft /proc/version) ]]; then
	echo -e "${TERM_YLW}* Sorry, I can't install Dolphin for you since you're on WSL${TERM_NOC}"
	echo -e "\tInstall dolphin on windows and edit the auto generated file at scripts/sc_env.sh"
	echo -e "\tSC_DOLPHIN_CMD=/mnt/c/<PATH TO DOLPHIN>"
	echo -e "\tExcept replace <Path To Dolphin> to be the path to dolphin executable"
	echo -e "\te.g. SC_DOLPHIN_CMD=\"/mnt/c/Program Files/Dolphin/dolphin.exe\""
	echo -e "SC_DOLPHIN_CMD=\"/mnt/c/<PATH_TO_DOLPHIN>\"" >> ${LOCAL}sc_env.sh
else
	# On debian, we need to add the PPA
	if [[ "${SC_HOST_SYSTEM}" == "Debian" ]]; then
		apt-add-repository ppa:dolphin-emu/ppa
	fi

	# Install Dolphin
	echo -e -n "* Checking for Dolphin emulator : "
	if ! type "dolphin-emu" &> /dev/null; then
		echo -e "${TERM_YLW}Not Found!${TERM_NOC}"
		echo -e -n "\tInstalling Dolphin Emulator (Will be a long time) :  "
		${PKG_INS} ${PKG_ARG} "dolphin-emu" &> /dev/null 
		# one final check 
		if ! type "dolphin-emu" &> /dev/null; then
			echo -e "${TERM_RED}\n\tFailed to install dolphin emu${TERM_NOC}"
			exit
		else
			echo -e "${TERM_GRN}Success!${TERM_NOC}"
		fi
	else
		echo -e "${TERM_GRN}Found!${TERM_NOC}"
	fi
	echo -e "SC_DOLPHIN_CMD=dolphin-emu" >> ${LOCAL}sc_env.sh
fi
