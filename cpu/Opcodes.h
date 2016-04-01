// cpu opcodes enum class

/*  This file is part of cpuEmulator.
    cpuEmulator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    cpuEmulator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with cpuEmulator.  If not, see http://www.gnu.org/licenses/.*/

#ifndef OPCODES_H
#define OPCODES_H

enum class Opcode {
    nop = 0x00,

    // arithmetic operations
    addImmediate = 0x01,
    subImmediate = 0x02,
    add = 0x03,
    sub = 0x04,
    nand = 0x05,
    lshift = 0x06,

    // control operations
    jumpToReg = 0x07,
    jumpRelative = 0x08,
    branchIfEqual = 0x09,
    branchIfLessThan = 0x0A,
    branchRelativeEqual = 0x0B,
    branchRelativeLessThan = 0x0C,
    
    // memory operations
    loadImmediate = 0x0D,
    loadFull = 0x0E,
    store = 0x0F,

    // system
    serialWrite = 0x10,
    halt = 0x11 
};

#endif
