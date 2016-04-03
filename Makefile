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

# -fno-strict-aliasing is needed for cpu/ram.h clockTick() where inoutData is set to a cast of the read data to DataType. Dissabling strict aliasing will reduce the possible optomisations for the compiler but I do not consider this application performance-critical
CPPOPTS=-Wall -Wpedantic -O2 -g -DDEBUG -std=c++11 -fno-strict-aliasing
OUTNAME=cpuEmulator
DEFAULT_TARGET=test
CPP=g++

.PHONY: default
default: $(DEFAULT_TARGET)

objects/Instruction.o: assembler/Instruction.cpp assembler/Instruction.h cpu/Opcodes.h
	$(CPP) $(CPPOPTS) -o $@ -c assembler/Instruction.cpp

test: registerTest busTest registerFileTest aluTest ramTest decoderTest
	@./registerTest
	@./busTest
	@./registerFileTest
	@./aluTest
	@./ramTest
	@./decoderTest

decoderTest: objects/decoderTest.o objects/debug.o objects/Instruction.o objects/Decoder.o objects/alu.o
	$(CPP) $(CPPOPTS) -o $@ objects/decoderTest.o objects/debug.o objects/Instruction.o objects/Decoder.o objects/alu.o

objects/Decoder.o: cpu/Opcodes.h cpu/Decoder.cpp cpu/Decoder.h emulator/Signal.h emulator/debug.h
	$(CPP) $(CPPOPTS) -o $@ -c cpu/Decoder.cpp 

objects/decoderTest.o: test/decoderTest.cpp cpu/Decoder.h cpu/Opcodes.h emulator/debug.h assembler/Instruction.h cpu/alu.h cpu/aluOps.h
	$(CPP) $(CPPOPTS) -o $@ -c test/decoderTest.cpp

ramTest: objects/ramTest.o objects/debug.o objects/Instruction.o
	$(CPP) $(CPPOPTS) -o $@ objects/ramTest.o objects/debug.o objects/Instruction.o

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

