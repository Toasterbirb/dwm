#!/bin/bash

# This script is meant for fast and simple testing during development

Help()
{
   # Display Help
   echo "Runs dwm in Xephyr."
   echo
   echo "Syntax: $(basename $0) [-h|d]"
   echo "options:"
   echo "h              Print this help."
   echo "d              Change display for test server."
   echo "t [theme_name] Use a custom colorscheme"
   echo
   echo "example: $(basename $0) -d :2"
   echo
}

# Default display
XEPHYR_DISPLAY=":1"

while getopts ':hdt:' OPTION; do
  case "$OPTION" in
    d)
		XEPHYR_DISPLAY="$OPTARG"
		echo "\$XEPHYR_DISPLAY configured to $OPTARG"
		;;
    h)
		Help
		exit;;
	t)
		THEME="./themes/$OPTARG"
		echo "Setting theme to $OPTARG"
		;;
    ?)
      Help
      exit 1;;
  esac
done
shift "$(($OPTIND -1))"

function _relaunch()
{
	[ -f $oldPID ] && pkill -9 -F ./testpid
	./dwm &
	echo "$!" > ./testpid
}


pgrep Xephyr || Xephyr -br -ac -noreset -screen 1600x900 $XEPHYR_DISPLAY &>/dev/null &

export DISPLAY=$XEPHYR_DISPLAY

make clean
rm -f config.h theme.h

if [ -n "$THEME" ]
then
	ln -sr $THEME theme.h
fi

make && _relaunch
