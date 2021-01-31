#!/bin/bash
while true
do
	cmus=$(./cmus)
	xsetroot -name " $cmus $(date "+%H:%M | %d.%m.%Y")"
	sleep 2
done
