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
#   along with picomips-cpu.  If not, see http://www.gnu.org/licenses/.

CPPOPTS=-Wpedantic -O2 -DDEBUG
OUTNAME=cpuEmulator
DEFAULT_TARGET=test
CPP=g++

.PHONY: default
default: $(DEFAULT_TARGET)
	@echo Defaulted to $(DEFAULT_TARGET) target

test: registerTest
	./registerTest

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

