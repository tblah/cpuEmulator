// Decoder module for the cpu

/* IO
    Input:
        memory word

    Output: (not all valid at any one time)
        opcode
        A
        B
        result
        imediate
*/

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

#ifndef DECODER_H
#define DECODER_H

#include "../emulator/Signal.h"
#include "Opcodes.h"
#include <stdint.h>

class Decoder {
    private:
        Signal<uint32_t> memoryWord;
        Signal<Opcode> Op;
        Signal<uint8_t> A;
        Signal<uint8_t> B;
        Signal<uint8_t> result;
        Signal<int32_t> immediate;

        // decoders for different instruction formats
        // add, sub, nand, lshift
        void decodeArgs3Reg( void );
        // addImmediate, subImmediate
        void decodeArgsImmediate( void );
        // jumpToReg, branchIfZero, branchIfPositive
        void decodeArgs1Reg( void );
        // load and store are both unique so they will be written straight into setMemoryWord
        // only the opcode has to be decoded for nop and halt

        // for reading registers and opcodes out of input words
        uint8_t get5BitsAtOffset( uint8_t offset );

    public:
        void setMemoryWord( uint32_t inMemoryWord );

        Opcode getOpcode( void );
        uint8_t getA( void );
        uint8_t getB( void );
        uint8_t getResult( void );
        int32_t getImmediate( void );

        void undefine( void );
};

#endif
