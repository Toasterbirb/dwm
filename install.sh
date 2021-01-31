#!/bin/bash
cp -f ./config.def.h ./config.h
doas make install
make clean
rm ./config.h
