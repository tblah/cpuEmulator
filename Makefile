# Makefile for all c++ code

#   This file is part of cpuEmulator.
#   cpuEmulator is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#   cpuEmulator is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#   You should have received a copy of the GNU General Public License
#   along with cpuEmulator.  If not, see http://www.gnu.org/licenses/.

CPPOPTS=-Wpedantic -O2 -g -DDEBUG -std=c++11
OUTNAME=cpuEmulator
DEFAULT_TARGET=asm
CPP=g++

.PHONY: default
default: $(DEFAULT_TARGET)

asm: objects/asmmain.o objects/asmCommandLineArgs.o objects/asmInstruction.o objects/debug.o
	$(CPP) $(CPPOPTS) -o $@ objects/asmmain.o objects/asmCommandLineArgs.o objects/asmInstruction.o objects/debug.o

objects/asmInstruction.o: assembler/Instruction.cpp assembler/Instruction.h cpu/Opcodes.h
	$(CPP) $(CPPOPTS) -o $@ -c assembler/Instruction.cpp

objects/asmmain.o: assembler/main.cpp assembler/commandLineArgs.h
	$(CPP) $(CPPOPTS) -o $@ -c assembler/main.cpp

objects/asmCommandLineArgs.o: assembler/commandLineArgs.cpp
	$(CPP) $(CPPOPTS) -o $@ -c assembler/commandLineArgs.cpp

test: registerTest busTest registerFileTest aluTest ramTest
	@./registerTest
	@./busTest
	@./registerFileTest
	@./aluTest
	@./ramTest

ramTest: objects/ramTest.o objects/debug.o
	$(CPP) $(CPPOPTS) -o $@ objects/ramTest.o objects/debug.o

objects/ramTest.o: test/ramTest.cpp cpu/ram.h emulator/Signal.h emulator/Register.h emulator/debug.h
	$(CPP) $(CPPOPTS) -o $@ -c test/ramTest.cpp

aluTest: objects/aluTest.o objects/debug.o objects/alu.o
	$(CPP) $(CPPOPTS) -o $@ objects/aluTest.o objects/debug.o objects/alu.o

objects/aluTest.o: cpu/alu.h cpu/aluOps.h test/aluTest.cpp emulator/debug.h
	$(CPP) $(CPPOPTS) -o $@ -c test/aluTest.cpp

objects/alu.o: cpu/alu* emulator/debug.h emulator/Signal.h
	$(CPP) $(CPPOPTS) -o $@ -c cpu/alu.cpp  

registerFileTest: objects/debug.o objects/registerFileTest.o
	$(CPP) $(CPPOPTS) -o $@ objects/registerFileTest.o objects/debug.o 

objects/registerFileTest.o: emulator/RegisterFile.h emulator/debug.h
	$(CPP) $(CPPOPTS) -o $@ -c test/registerfileTest.cpp

busTest: objects/busTest.o objects/debug.o
	$(CPP) $(CPPOPTS) -o $@ objects/busTest.o objects/debug.o

objects/busTest.o: emulator/Signal.h emulator/Bus.h test/busTest.cpp
	$(CPP) $(CPPOPTS) -o $@ -c test/busTest.cpp

registerTest: objects/registerTest.o objects/debug.o
	$(CPP) $(CPPOPTS) -o $@ objects/registerTest.o objects/debug.o

objects/registerTest.o: emulator/Register.h emulator/Signal.h test/registerTest.cpp
	$(CPP) $(CPPOPTS) -o $@ -c test/registerTest.cpp

objects/debug.o: emulator/debug.cpp emulator/debug.h
	$(CPP) $(CPPOPTS) -o $@ -c emulator/debug.cpp

.PHONY: clean
clean: 
	@-2>/dev/null rm $(shell find . -name "*.o"); true
	@-2>/dev/null rm $(shell find . -executable); true

