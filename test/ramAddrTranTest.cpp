// test for RamAddrTran. Based on tests for ram (ramTest.cpp).

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

#include "../cpu/RamAddrTranslator.h"
#include "../emulator/debug.h"
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include "../assembler/Instruction.h"
#include "../cpu/Opcodes.h"
#include <string.h>

#include <endian.h>

int main( void ) {
    debug( "Starting RamAddrTran test" );

    // list of starting data for main memory
    Instruction I1 = Instruction( Opcode::add, 1, 2, 3 );
    uint32_t data = I1.getObjectCode();

    std::vector<int32_t> StartingData;
    StartingData.push_back( data );
    
    RamAddrTran<uint32_t, 10240> DUT( StartingData ); // approx 10kB RAM with 32-bit addresses and data bus width

    // test that the data was pre-loaded to main memory
    DUT.setAddress( 0 );
    DUT.setReadingThisCycle( true );
    DUT.clockTick();

    if ( DUT.getOutput() != htobe32(data) )
        errExit( "Data were not preloaded into RamAddrTran correctly" );
    
    // write to main memory
    DUT.setAddress( 0 );
    DUT.setDataIn( 100 );
    DUT.setReadingThisCycle( false );
    DUT.clockTick();

    // read back from main memory
    DUT.setAddress( 0 );
    DUT.setReadingThisCycle( true );
    DUT.clockTick();
    int32_t result = DUT.getOutput();

    // test that we got back what we wrote
    if ( result != htobe32(100) )
        errExit( "RamAddrTran test failed. We did not read back what we wrote to main memory" );

    // write to video memory
    DUT.setAddress( 6144 ); // 0th address of video memory (10240-4096=6144)
    char testData[] = "ABC";
    DUT.setDataIn( *((int32_t*) testData) ); 
    DUT.setReadingThisCycle( false );
    DUT.clockTick();

    //DUT.printBuffer(); // this cannot be easily tested automatically. It has been visually inspected. 

    // read back from video memory
    DUT.setAddress( 6144 ); // 0th address of video memory
    DUT.setReadingThisCycle( true );
    DUT.clockTick();
    result = DUT.getOutput();
    char* resultStr = (char*) &result;

    // test that we got back what we wrote
    if ( strncmp(resultStr, "ABC", 4 ) != 0 ) {
        errExit( "RamAddrTran test failed. We did not read back what we wrote to video memory" );
    }


    debug( "All test passed for RamAddrTran" );
    return EXIT_SUCCESS;
}
