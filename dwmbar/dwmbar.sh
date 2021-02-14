#!/bin/bash

scriptPath=$(echo $0 | sed 's/dwmbar.sh//g')
while true
do
	cmus=$($scriptPath/cmus)
	xsetroot -name " $cmus $(date "+%H:%M | %d.%m.%Y ")"
	sleep 2
done
