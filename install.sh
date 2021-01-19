#!/bin/bash
cp -f ./config.def.h ./config.h
sudo make clean install
rm ./config.h
