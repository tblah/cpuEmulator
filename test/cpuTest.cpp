// tests for the whole cpu

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

using namespace std;

// yes returning a CPU is *really* slow but it is the neetest way I can see 
//      of doing this without refactoring
// and this still runs in no time at all for the ram sizes we are emulating
// this function only exists to reduce the amount of repeted code. If we were
//      using huge ram sizes then it would be better to just write the code out 
//      long-hand so that the CPU object is not copied around the stack 
CPU runInstructions( vector<Instruction> &instructions ) {
    vector<int32_t> machineCode;
    
    for ( unsigned int i = 0; i < instructions.size(); i++ )
        machineCode.push_back( instructions.at(i).getObjectCode() );

    CPU DUT( machineCode );

    while ( !DUT.clockTick() ); // run until halt

    return DUT;
}

int main( void ) {
    debug( "Begginning cpu tests" );
    debug( "" );

    // test halt
    vector<Instruction> haltTest;
    haltTest.push_back( Instruction( Opcode::halt ) );
    runInstructions( haltTest );
    debug ( "halt test passed" );
    debug( "" );




    // test nop
    vector<Instruction> nopTest;
    nopTest.push_back( Instruction( Opcode::nop ) );
    nopTest.push_back( Instruction( Opcode::halt ) );
    runInstructions( nopTest );
    debug( "nop test passed" );
    debug( "" );



    // test addImmediate, subImmediate and store
    vector<Instruction> test3;
    // add 100 to r0 (which always has a value of 0), and save to r1
    test3.push_back( Instruction( Opcode::addImmediate, (uint8_t) 0, (int32_t) 100 ) ); 
    // now subtract 50 from it, saving to r1
    test3.push_back( Instruction( Opcode::subImmediate, (uint8_t) 1, (int32_t) 50 ) );
    // store the contents of register 1 in ram location 0
    test3.push_back( Instruction( Opcode::store, (uint8_t) 0, (uint8_t) 1 ) );
    test3.push_back( Opcode::halt );
    CPU test3CPU = runInstructions( test3 );
    
    if ( test3CPU.debugRamRead( 0 ) != 50 )
        errExit( "test3" );
    else
        debug( "test 3 passed" );
    debug( "" );





    // test add, sub 
    vector<Instruction> test4;
    // put -10 into r1
    test4.push_back( Instruction( Opcode::addImmediate, (uint8_t) 0, (int32_t) -10 ) );
    // copy it to r2 (add r0 to r1 and save to r2)
    test4.push_back( Instruction( Opcode::add, 0, 1, 2 ) );
    // subtract r2 from r1 and save to r3 ( -10 - (-10) = 0 )
    test4.push_back( Instruction( Opcode::sub, 2, 1, 3 ) );
    // save this to ram[0]
    test4.push_back( Instruction( Opcode::store, (uint8_t) 0, (uint8_t) 3 ) );
    test4.push_back( Instruction( Opcode::halt ) );
    CPU test4CPU = runInstructions( test4 );
    
    if ( test4CPU.debugRamRead( 0 ) != 0 )
        errExit( "test4" );
    else
        debug( "test 4 passed" );
    debug( "" );





    // test load, jumpToReg
    vector<Instruction> testLoad;

    // put the address of the 7th word into r1
    testLoad.push_back( Instruction( Opcode::addImmediate, (uint8_t) 0, (int32_t)7*4) );
    // load number at RAM[ (r1 = 7words) ] into r10
    testLoad.push_back( Instruction( Opcode::load, (uint8_t) 1, (uint8_t) 10 ) );

    // write r10 into ram address 10 
    testLoad.push_back( Instruction( Opcode::addImmediate, (uint8_t) 0, (int32_t) 10) );
    testLoad.push_back( Instruction( Opcode::store, (uint8_t) 1, (uint8_t) 10 ) );
    testLoad.push_back( Instruction( Opcode::halt ) );

    // jump over the number that we are going to load and early halt
    testLoad.push_back( Instruction( Opcode::addImmediate, (uint8_t) 0, (int32_t)9*4));
    testLoad.push_back( Instruction( Opcode::jumpToReg, (uint8_t) 1 ) );

    // number that we load
    testLoad.push_back( Instruction( 1234 ) );

    // early halt
    testLoad.push_back( Instruction( Opcode::halt ) );

    // write r10 into ram address 10 
    testLoad.push_back( Instruction( Opcode::addImmediate, (uint8_t) 0, (int32_t) 10) );
    testLoad.push_back( Instruction( Opcode::store, (uint8_t) 1, (uint8_t) 10 ) );
    testLoad.push_back( Instruction( Opcode::halt ) );

    CPU testLoadCPU = runInstructions( testLoad );

    if ( testLoadCPU.debugRamRead( 10 ) != 1234 )
        errExit( "loadTest" );
    else
        debug( "load test passed" );
    debug( "" );




    // conditional branch test
    vector<Instruction> condBranch;

    // jump over the early halt
    condBranch.push_back( Instruction( Opcode::addImmediate, (uint8_t) 0,(int32_t)3*4));
    condBranch.push_back( Instruction( Opcode::branchIfPositive, 1 ) );

    // early halt
    condBranch.push_back( Instruction( Opcode::halt ) );

    // jump to a negative memory address (errExit) if we try to jump on a negative
    condBranch.push_back( Instruction( Opcode::subImmediate, (uint8_t) 0,(int32_t) 1) );
    condBranch.push_back( Instruction( Opcode::branchIfPositive, 1 ) );

    // copy -1 into r2 and r3
    condBranch.push_back( Instruction( Opcode::add, 0, 1, 2 ) );
    condBranch.push_back( Instruction( Opcode::add, 0, 1, 3 ) );

    // if (r2 == r3) jump over early halt
    condBranch.push_back( Instruction( Opcode::addImmediate, 0, 11*4 ) );
    condBranch.push_back( Instruction( Opcode::sub, 2, 3, 10 ) );
    condBranch.push_back( Instruction( Opcode::branchIfZero, 1 ) );

    // early halt
    condBranch.push_back( Instruction( Opcode::halt ) );

    // if (r0 == r2) jump to zero (loop forever)
    condBranch.push_back( Instruction( Opcode::addImmediate, 0, 0 ) );
    condBranch.push_back( Instruction( Opcode::sub, 0, 2, 10 ) );
    condBranch.push_back( Instruction( Opcode::branchIfZero, 1 ) );

    // store r1 to ram[100]
    condBranch.push_back( Instruction( Opcode::add, 0, 1, 2 ));
    condBranch.push_back( Instruction( Opcode::addImmediate, (uint8_t) 0,(int32_t)100));
    condBranch.push_back( Instruction( Opcode::store, (uint8_t) 1, (uint8_t) 2 ) );

    condBranch.push_back( Instruction( Opcode::halt ) );

    CPU condBranchCPU = runInstructions( condBranch );

    if ( condBranchCPU.debugRamRead( 100 ) != 0 )
        errExit( "cond branch test" );
    else
        debug( "conditional branch test passed" );

    debug( "" );

    // if add and sub work then nand and lshift definately work because the alu is 
    //      fully tested and the instructions work the same way

    // check that we don't crash when running the print buffer instruction. We cannot test it's functionality 
    // without messing about with exec's and pipes and that feels like overkill when I can just look at the console. 
    // A more full test of it will be written in a different test program.
    vector<Instruction> printBuffer;

    printBuffer.push_back( Instruction( Opcode::printBuffer ) );
    printBuffer.push_back( Instruction( Opcode::halt ) );

    runInstructions( printBuffer ); // we are just checking that this manages to return
    debug( "printBuffer test 1 passed" );


    debug( "All tests passed for CPU" );
    return EXIT_SUCCESS;
}
