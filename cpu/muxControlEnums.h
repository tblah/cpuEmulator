// enums for controlling the multiplexers in the cpu
// these have been superseeded because it would make the code extremely
//      messy to include explicitly and their functionality is replicated by
//      the switch statements anyway

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

#ifndef ENUM_CONTROL_H
#define ENUM_CONTROL_H

// this mux was not included explicitly in code because it made it hard to read
/*enum class AluBMuxControl {
    RegB,
    Immediate
};*/

/*enum class PCMuxControl {
    Normal, // PC = PC+4
    JumpIfZero,
    JumpIfPositive,
    Jump
};

enum class RegWriteSelectMuxControl {
    One,
    ResultArg
};

enum class RegWriteDataMuxControl {
    aluResult,
    ramRead
};*/

#endif
