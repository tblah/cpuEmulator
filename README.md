# RISC CPU
Project for ELEC2204 at Southampton University, ECS.

Written by Tom Eccles tde1g14 AT soton DOT ac DOT uk.

C++ Emulator for simple processor of my own design.

This work is licensed under the GPLv3 or any later version of the GPL as published by the Free Software Foundation (https://fsf.org).
A copy of the GPLv3 is provided in the LICENCE file.

## Building
Use the Makefile provided. You may need to create the ./objects directory. 

This was tested on 64-bit x86 Trisquel GNU+Linux 7 (GCC 4.8.4, GNU Make 3.81). 

## Directory Structure
test - Unit tests for things

emulator - Things for emulating digital electronics. Not specific to the CPU design

cpu - Implementation of the CPU

objects - compiled but unlinked objects from the build

assembler - a *very* simple assembler for generating memory images for the cpu to execute. I decided that making something able to parse files was overkill and so (for now) this will just use instances of Instruction.

doc - Source files for the report on this coursework

