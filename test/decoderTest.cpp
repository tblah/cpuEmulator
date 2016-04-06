// tests for Decoder

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

#include "../cpu/Decoder.h"
#include "../cpu/Opcodes.h"
#include "../emulator/debug.h"
#include "../assembler/Instruction.h"
#include <stdlib.h>
#include <time.h>
#include <endian.h>

int main( void ) {
    debug( "Beginning Decoder tests" );

    Decoder DUT;
    
    // test each instruction type with some dummy values

    // add
    Instruction addI = Instruction( Opcode::add, 2, 1, 3 );
    DUT.setMemoryWord( htobe32( addI.getObjectCode() ) );
    
    if ( (DUT.getOpcode() != Opcode::add) ||
            (DUT.getA() != 2) || (DUT.getB() != 1) || (DUT.getResult() != 3) )
        errExit( "add instruction" );
    
    // load
    Instruction loadI = Instruction( Opcode::load, (uint8_t) 1, (uint8_t) 2 );
    DUT.setMemoryWord( htobe32( loadI.getObjectCode() ) );

    if ( (DUT.getOpcode() != Opcode::load) || (DUT.getA() != 1) || 
            (DUT.getResult() != 2 ) )
       errExit( "load instruction" ); 

    // store
    Instruction storeI = Instruction( Opcode::store, (uint8_t) 2, (uint8_t) 1 );
    DUT.setMemoryWord( htobe32( storeI.getObjectCode() ) );

    if ( (DUT.getOpcode() != Opcode::store) || (DUT.getA() != 2) || (DUT.getB() != 1) )
        errExit( "store" );

    // addImmediate
    // do this test a few times with different random numbers so I can feel happier about the immediate arguement
    srand( time(NULL ) );
    for (int i = 0; i < 1000; i++ ) {
        // random accross 22 bit signed integers
        int32_t testNum = ( rand() % 4194304 ) - 2097152;

        Instruction addImmI = Instruction( Opcode::addImmediate, 1, testNum );
        DUT.setMemoryWord( htobe32( addImmI.getObjectCode() ) );
    
        if ( (DUT.getOpcode() != Opcode::addImmediate)  || (DUT.getA() != 1) ||
                (DUT.getImmediate() != testNum) )
            errExit( "addImmediate" );
    }
    
    // branchIfZero
    Instruction bneI = Instruction( Opcode::branchIfZero, 30 );
    DUT.setMemoryWord( htobe32( bneI.getObjectCode() ) );

    if ( (DUT.getOpcode() != Opcode::branchIfZero) || (DUT.getA() != 30 ) )
        errExit( "branchIfZero" );

    // nop
    Instruction nopI = Instruction( Opcode::nop );
    DUT.setMemoryWord( htobe32( nopI.getObjectCode() ) );

    if ( DUT.getOpcode() != Opcode::nop )
        errExit( "nop" );

    debug( "All Decoder tests passed" );
    return EXIT_SUCCESS;
}

