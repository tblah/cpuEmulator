// demo of the cpu where "Hello World!" moves about the screen

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

#include "../cpu/CPU.h"
#include "../emulator/debug.h"
#include "../assembler/Instruction.h"
#include "../cpu/Opcodes.h"
#include <vector>
#include <stdlib.h>
#include <stdint.h>
#include <endian.h>

using namespace std;

void runInstructions( vector<Instruction> &instructions ) {
    vector<int32_t> machineCode;
    
    for ( unsigned int i = 0; i < instructions.size(); i++ )
        machineCode.push_back( instructions.at(i).getObjectCode() );

    CPU DUT( machineCode );

    while ( !DUT.clockTick() ); // run until halt
}

int main( void ) {
    debug( "Begginning cpu demo" );
    
    // does this: (note that the strings here are not null terminated)
    /*
     * for (i=6144; i < 10227; i++) { // 6144 is the base address of the video memory
     *      ram[i] = "____";
     *      ram[i+1] = "Hell";
     *      ram[i+1+4] = "o Wo";
     *      ram[i+1+4+4] ="rld!";
     *      printBuffer();
     * }
     *
     * Once information is loaded, the registers will contain
     * r10 = "    "
     * r11 = "Hell"
     * r12 = "o Wo"
     * r13 = "rld!"
     * r14 = (no longer used)
     * r15 = 10227
     * r16 = i
     * r17 = addr of start of loop
     * r18 addr of halt
     */

    vector<Instruction> I;

    I.push_back( Instruction( Opcode::addImmediate, 0, (int32_t) 6*4 ) );
    I.push_back( Instruction( Opcode::jumpToReg, (uint8_t) 1 ) ); // jump to address 6 (over the data in interveining addresses)

    // data (starts at addr 2)
    I.push_back( Instruction( static_cast<int32_t>(          0x5F5F5F5F   ) ) ); // "____" symetric so endian-ness does not matter
    I.push_back( Instruction( static_cast<int32_t>( htobe32( 0x48656C6C ) ) ) ); // "Hell" 
    I.push_back( Instruction( static_cast<int32_t>( htobe32( 0x6F20576F ) ) ) ); // "o Wo"
    I.push_back( Instruction( static_cast<int32_t>( htobe32( 0x726C6421 ) ) ) ); // "rld!"

    // preload stuff into registers (starts at addr 6)
    I.push_back( Instruction( Opcode::addImmediate, 0, (int32_t) 2*4 ) ); // addr 1 jumped to here
    I.push_back( Instruction( Opcode::load, 1, (uint8_t) 10 ) );       // r10 = ram[2] = "    "

    I.push_back( Instruction( Opcode::addImmediate, 0, (int32_t) 3*4 ) );
    I.push_back( Instruction( Opcode::load, 1, (uint8_t) 11 ) );       // r11 = ram[3] = "Hell"

    I.push_back( Instruction( Opcode::addImmediate, 0, (int32_t) 4*4 ) );
    I.push_back( Instruction( Opcode::load, 1, (uint8_t) 12 ) );       // r12 = ram[4] = "o Wo"

    I.push_back( Instruction( Opcode::addImmediate, 0,  (int32_t) 5*4 ) );
    I.push_back( Instruction( Opcode::load, 1, (uint8_t) 13 ) );        // r13 = ram[5] = "rld!"

    I.push_back( Instruction( Opcode::addImmediate, 0, (int32_t) 6144 ) );
    I.push_back( Instruction( Opcode::add, 1, 0, 16 ) );               // i = r16 = 6114

    I.push_back( Instruction( Opcode::addImmediate, 0, (int32_t) 10224 ) );
    I.push_back( Instruction( Opcode::add, 1, 0, 15 ) );               // r15 = 10227

    I.push_back( Instruction( Opcode::addImmediate, 0, (int32_t) 22*4 ) ); // start of the loop is at address 22
    I.push_back( Instruction( Opcode::add, 1, 0, 17 ) );               // r17 = 22 = start of loop

    I.push_back( Instruction( Opcode::addImmediate, 0, (int32_t) 34*4 ) ); // halt address is 34
    I.push_back( Instruction( Opcode::add, 1, 0, 18 ) );               // r18 = 34 = halt addr

    // addr 22: start of loop
    // loop condition
    I.push_back( Instruction( Opcode::sub, 15, 16, (int8_t) 1 ) );              // r1 = 10227-i
    I.push_back( Instruction( Opcode::branchIfZero, 18 ) );            // if that subtraction made 0 then i=10227 so jump to the halt

    // loop content
    I.push_back( Instruction( Opcode::store, 16, (uint8_t) 10 ) );     // ram[i] = r10 = "____"

    I.push_back( Instruction( Opcode::addImmediate, 16, (int32_t) 1 ) );   // r1 = i+1 
    I.push_back( Instruction( Opcode::add, 1, 0, (uint8_t) 16 ) );         // (while we have i+1 calculated) i = i+1
    I.push_back( Instruction( Opcode::store, 1, (uint8_t) 11 ) );          // ram[r1] = r11 = "Hell"
    
    I.push_back( Instruction( Opcode::addImmediate, 1, (int32_t) 4 ) );    // r1 = r1+4 = i+1+4
    I.push_back( Instruction( Opcode::store, 1, (uint8_t) 12 ) );          // ram[r1] = r12 = "o Wo"

    I.push_back( Instruction( Opcode::addImmediate, 1, (int32_t) 4 ) );    // r1 = r1+4 = i+1+4+4
    I.push_back( Instruction( Opcode::store, 1, (uint8_t) 13 ) );          // ram[r1] = r13 = "rld!"

    I.push_back( Instruction( Opcode::printBuffer ) );

    // go back to the beginning of the loop
    I.push_back( Instruction( Opcode::jumpToReg, 17 ) );                   // jump back to the start of the loop

    // the previously mentioned halt instruction
    I.push_back( Instruction( Opcode::halt ) );

    // emulate the processor
    runInstructions( I );
    
    debug( "End of CPU demo" );
    return EXIT_SUCCESS;
}
