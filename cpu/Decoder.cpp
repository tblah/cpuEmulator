// combinational logic which decodes instructions
// the decoder does not worry about details of number representation. That is in the ALU.

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

#include "Decoder.h"
#include "../emulator/debug.h"
#include <endian.h>

// decoders for different instruction formats
// add sub nand lshift
void Decoder::decodeArgs3Reg( void ) {
    // arguements are A, B and result
    A.setValue( get5BitsAtOffset( 5 ) );
    B.setValue( get5BitsAtOffset( 10 ) );
    result.setValue( get5BitsAtOffset( 15 ) );
}

// addImmediate, subImmediate
void Decoder::decodeArgsImmediate( void ) {
    // arguements are A and result
    A.setValue( get5BitsAtOffset( 5 ) );

    // immediate starts at offset 10 and is 22 bits long
    int32_t word = memoryWord.getValue();
    word = word >> 10; // the bits shifted into the left will match the sign bit
    immediate.setValue( word );
}

// jumpToReg, branchIfZero, branchIfPositive
void Decoder::decodeArgs1Reg( void ) {
    // A is the only arguement
    A.setValue( get5BitsAtOffset( 5 ) );
}

uint8_t Decoder::get5BitsAtOffset( uint8_t offset ) {
    uint32_t word = memoryWord.getValue();
    word = word >> offset;
    
    // we want the least significant byte. This should be big-endian
    uint8_t ret = ((uint8_t*) &word )[0];

    // we only want the least significant 5 bits
    ret = ret & 0b00011111; // binary constants are a GCC extension
    // they are also part of C++1y so a modern compiler should be okay

    return ret;
}

void Decoder::setMemoryWord( uint32_t inMemoryWord ) {
    // invalidate old outputs
    undefine();

    // set the new memoryWord
    memoryWord.setValue( /*be32toh(inMemoryWord)*/ inMemoryWord );

    // calculate new outputs
    Op.setValue( static_cast<Opcode>(get5BitsAtOffset( 0 )) );

    switch ( Op.getValue() ) {
        case ( Opcode::add ):
        case ( Opcode::sub ):
        case ( Opcode::nand ):
        case ( Opcode::lshift ):
            decodeArgs3Reg();
            break;

        case ( Opcode::addImmediate ):
        case ( Opcode::subImmediate ):
            decodeArgsImmediate();
            break;

        case ( Opcode::jumpToReg ):
        case ( Opcode::branchIfZero ):
        case ( Opcode::branchIfPositive ):
            decodeArgs1Reg();
            break;

        case ( Opcode::load ):
            A.setValue( get5BitsAtOffset( 5 ) );
            result.setValue( get5BitsAtOffset( 15 ) );            
            break;

        case ( Opcode::store ):
            A.setValue( get5BitsAtOffset( 5 ) );
            B.setValue( get5BitsAtOffset( 10 ) );
            break;

        case ( Opcode::nop ):
        case ( Opcode::halt ):
        case ( Opcode::printBuffer ):
            // no arguements need to be processed
            break;

        default:
            errExit( "decoding invalid opcode (cpu/Decoder.cpp)" );
    }
}

Opcode Decoder::getOpcode( void ) {
    return Op.getValue();
}

uint8_t Decoder::getA( void ) {
    return A.getValue();
}

uint8_t Decoder::getB( void ) {
    return B.getValue();
}

uint8_t Decoder::getResult( void ) {
    return result.getValue();
}

int32_t Decoder::getImmediate( void ) {
    return immediate.getValue();
}

void Decoder::undefine( void ) {
    memoryWord.undefine();
    Op.undefine();
    A.undefine();
    B.undefine();
    result.undefine();
    immediate.undefine();
}
    
