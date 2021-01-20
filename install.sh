#!/bin/bash
cp -f ./config.def.h ./config.h
sudo make install
make clean
rm ./config.h
