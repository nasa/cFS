#!/bin/bash
#   Desc: This Script will copy or link example files to appropriate locations.
#   Author: Guy de Carufel, Allen Brown (Odyssey Space Research)
#   Date: May 11, 2016

usage ()
{
    echo -e "\nHELP TEXT\n"
    echo -e "\tScript to cp (or link) example files to appropriate locations."
    echo -e "\tNOTE: Target will be overwritten if it already exists.\n"
    echo -e "\tOption: -h         This help text."
    echo -e "\tOption: -l         Create symbolic link instead of a copy."
    echo -e "\tOption: -m {NAME}  Mission name. (default: MISSION_NAME env var)"
    echo -e "\t                   (This controls the *_to_types.h name.)"
    echo -e "\tArg: example (dir) name."
    echo -e "\n\tNote: Expects either a MISSION_HOME or APP_DIR env variable "
    echo -e "\t      to locate apps/inc (which must exist)."
    echo -e "\tNote: Either the MISSION_NAME env var or the -m option must be used."
    
    echo -e "\n\tExample:  ./setup.sh -l -m MY_MISSION udp"
    echo -e "\n\tWill create following symbolic links:"
    echo -e "\tln -s udp/to_custom.c ../src/"
    echo -e "\tln -s udp/to_platform_cfg.h ../platform_inc/"
    echo -e "\tln -s udp/MISSION_to_types.h MISSION_HOME/apps/inc/MY_MISION_to_types.h"
    echo -e "\t(cp called instead of ln -s if -l option not used.)\n"
}

link=0

while [ "$1" != "" ]; do

    echo "===DEBUG In while loop, args: $@ ==="

    case $1 in
        -h | --help )       usage
                            exit
                            ;;
        -l | --link )       link=1
                            ;;
        -m | --mission )    mission=$2
                            shift
                            ;;
        * )                 break
        
    esac
    shift
done

if [ $# -ne 1 ]; then
    echo -e "\tA single argument required. eg. setup.sh udp"
    usage
    exit
fi

# Check the CFS_TST way
if [ -z "$MISSION_HOME" ]; then
	# Check the gsfc_build/"classic build" way (setvars.sh)
	if [ -z "$APP_DIR" ]; then
		echo -e "Either the env var MISSION_HOME (CFS_TST build)"
		echo -e "  or APP_DIR (gsfc ""classic"" build) must be set."
		exit 1
	fi
else
	APP_DIR="${MISSION_HOME}/apps"
fi

if [ ! -d "$APP_DIR" ]; then
    echo -e "\t$APP_DIR does not point to an existing directory."
    exit
fi

if [ ! -d "$APP_DIR/inc" ]; then
    echo -e "\t$APP_DIR/inc does not exist. Create directory first."
    exit
fi

if [ -z "$mission" ]; then
	if [ -z "$MISSION_NAME" ]; then
		echo -e "Need a mission name from the -m option via the MISSION_NAME env var."
		exit 1
	else
		mission=$MISSION_NAME
	fi
fi

mission="${mission^^}"

if [ ! -d "$PWD/$1" ]; then
    echo -e "\tBad example name. ARG should be the name of the example directory."
    exit
fi

echo -e "\tArguments supplied:"

if [ "$link" -eq 1 ]; then
    echo -e "\tOption: -l (create symbolic links)"
fi
echo -e "\tMission name: $mission"
echo -e "\tExample name: $1\n"

rm -f $PWD/../src/to_custom.c
rm -f $PWD/../platform_inc/to_platform_cfg.h
rm -f ${APP_DIR}/inc/${mission}_to_types.h 
echo -e "\tRemoved files:"
echo -e "\t$PWD/../src/to_custom.c"
echo -e "\t$PWD/../platform_inc/to_platform_cfg.h"
echo -e "\t${APP_DIR}/inc/${mission}_to_types.h\n"

if [ "$link" -eq 1 ]; then
    ln -sf $PWD/$1/to_custom.c $PWD/../src/to_custom.c
    ln -sf $PWD/$1/to_platform_cfg.h $PWD/../platform_inc/to_platform_cfg.h
    ln -sf $PWD/$1/MISSION_to_types.h ${APP_DIR}/inc/${mission}_to_types.h 
    echo -e "\tCreating Symbolic links:"
else
    cp -f $PWD/$1/to_custom.c $PWD/../src/to_custom.c
    cp -f $PWD/$1/to_platform_cfg.h $PWD/../platform_inc/to_platform_cfg.h
    cp -f $PWD/$1/MISSION_to_types.h ${APP_DIR}/inc/${mission}_to_types.h
    echo -e "\tFiles copied:"
fi

echo -e "\t$PWD/$1/to_custom.c -> $PWD/../src/to_custom.c"
echo -e "\t$PWD/$1/to_platform_cfg.h -> $PWD/../platform_inc/to_platform_cfg.h"
echo -e "\t$PWD/$1/MISSION_to_types.h -> ${APP_DIR}/inc/${mission}_to_types.h\n"
