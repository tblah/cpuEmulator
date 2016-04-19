// stores an Instruction. See headder file

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

#include "Instruction.h"
#include "../emulator/debug.h"

// for architecture-independent endian-ness
//#define _BSD_SOURCE // already defined by gcc
#include <endian.h>

uint8_t Instruction::validateRegister( uint8_t reg ) {
    if ( reg > 31 )
        errExit( "Illegal register number specified. Cannot assemble." );

    // endian-ness of 1 byte is nothing to worry about
    return reg;
}

uint32_t Instruction::validateImmediate( int32_t immediate ) {
    // two's compliment representations of the immediate value must fit in 22 bits
    // therefore valid Immediates fit in the shown range
    
    if ( (immediate < -2097152) || (immediate > 2097151) )
        errExit( "Illigal imediate value specified. Cannot assemble." );

    // make sure it is in two's compliment
    uint32_t result = abs( immediate );

    if ( immediate < 0 ) {
        // invert the digits and add 1
        result = ~result; 
        result += 1;
    }

    return result;
}

Instruction::Instruction( Opcode Op, uint8_t A, uint8_t B, uint8_t dest) {
    //  test opcode matches this type
    if ( (Op != Opcode::add) && (Op != Opcode::sub) && (Op != Opcode::nand)
             && (Op != Opcode::lshift) )
        errExit( "Incorrect instruction type for opcode" );

    // the format is as follows
    // |31 unused 20| |19 dest 15| |14 B 10| |9 A 5| |4 opcode 0|
    objectCode = 0;
    objectCode |= validateRegister( dest ) << 15;
    objectCode |= validateRegister( B ) << 10;
    objectCode |= validateRegister( A ) << 5;
    objectCode |= static_cast<unsigned int>(Op); // top 3 bits should be zero so this won't break A
}

// for load theOtherOne=dest, store theOtherOne=B
Instruction::Instruction( Opcode Op, uint8_t A, uint8_t theOtherOne ) {
    if ( (Op != Opcode::store) && (Op != Opcode::load)  )
        errExit( "Incorrect instruction type for opcode" );

    // format is a sparser version of 'A, B, dest' type   
    objectCode = 0;

    if ( Op == Opcode::load )
        objectCode |= validateRegister( theOtherOne ) << 15;

    if ( Op == Opcode::store )
        objectCode |= validateRegister( theOtherOne ) << 10;

    objectCode |= validateRegister( A ) << 5;
    objectCode |= static_cast<unsigned int>(Op); // top 3 bits should be zero so this won't break A
}

Instruction::Instruction( Opcode Op, uint8_t A, int32_t immediate ) {
    // test opcode matches this type
    if ( (Op != Opcode::addImmediate) && (Op != Opcode::subImmediate) )
        errExit( "Incorrect instruction type for opcode" );

    // format
    // |31 immediate 10| |9 A 5| |4 opcode 0|
    objectCode = 0;
    objectCode |= validateImmediate( immediate ) << 10;
    objectCode |= validateRegister( A ) << 5;
    objectCode |= static_cast<unsigned int>(Op); // top 3 bits should be zero so this won't break A
}

Instruction::Instruction( Opcode Op, uint8_t A ) {
    // test opcode matches this type
    if ( (Op != Opcode::jumpToReg) && (Op != Opcode::branchIfZero) 
            && (Op != Opcode::branchIfPositive) )
        errExit( "Incorrect instruction type for opcode" );

    // format
    // |31 unused 10| |9 A 5| |4 opcode 0|
    objectCode = 0;
    objectCode |= validateRegister( A ) << 5;
    objectCode |= static_cast<unsigned int>(Op); // top 3 bits should be zero so this won't break A
}

Instruction::Instruction( Opcode Op ) {
    if ( (Op != Opcode::halt) && (Op != Opcode::nop) && (Op != Opcode::printBuffer) )
        errExit( "Incorrect instruction type for opcode" );

    objectCode = 0;
    objectCode |= static_cast<unsigned int>(Op);
}

Instruction::Instruction( int32_t num ) {
    objectCode = num;
}

uint32_t Instruction::getObjectCode( void ) {
    // the cpu is big endian. This assembler may not be running on a big endian machine
    return htobe32(objectCode);
}
