// class to represent an instruction to be translated from assembly to machine code

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

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "../cpu/Opcodes.h"
#include <stdint.h>

class Instruction {
    private:
        uint32_t objectCode; 

        uint8_t validateRegister( uint8_t reg );
        uint32_t validateImmediate( int32_t Immediate );

        public:
            // constructor for instructions with 3 registers as their arguements
            Instruction( Opcode Op, uint8_t A, uint8_t B, uint8_t dest);

            // constructor for instructions with 1 register and an immediate
            Instruction( Opcode Op, uint8_t A, int32_t immediate );

            // constructor for instructions with just 1 register as an arg
            Instruction( Opcode Op, uint8_t A );

            // constructor for instructions with just an opCode
            Instruction( Opcode Op );

            uint32_t getObjectCode( void );

};

#endif