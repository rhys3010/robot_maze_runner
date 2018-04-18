#!/bin/sh

# Utility Script to compile the maze_runner application for the formula
# allcode buggy using the xc16 compiler
# @author Rhys Evans (rhe24@aber.ac.uk)
# @version 0.1

# Compile to .bin
./bin/xc16/xc16 -o ./out/maze_runner/main.bin ./src/maze_runner/main.c ./lib/allcode_api/allcode_api.o
echo "Generated .bin file in /out/maze_runner/"

# Generate .hex file
/opt/microchip/xc16/v1.34/bin/xc16-bin2hex -a ./out/maze_runner/main.bin 
echo "Generated .hex file in /out/maze_runner"
