// test for RAM

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

#include "../cpu/ram.h"
#include "../emulator/debug.h"
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include "../assembler/Instruction.h"
#include "../cpu/Opcodes.h"

#include <endian.h>

int main( void ) {
    debug( "Starting RAM test" );

    // list of starting data
    Instruction I1 = Instruction( Opcode::add, 1, 2, 3 );
    uint32_t data = I1.getObjectCode();

    std::vector<int32_t> StartingData;
    StartingData.push_back( data );
    
    RAM<uint32_t, 1024> DUT( StartingData ); // 1kB RAM with 32-bit addresses and data bus width

    // test that the data was pre-loaded
    DUT.setReadingThisCycle( true );
    DUT.setAddress( 0 );
    DUT.clockTick();
    
    if ( DUT.getOutput() != htobe32(data) )
        errExit( "Data were not preloaded into RAM correctly" );
    
    // write
    DUT.setReadingThisCycle( false );
    DUT.setAddress( 0 );
    DUT.setDataIn( 100 );
    DUT.clockTick();

    // read back
    DUT.setReadingThisCycle( true );
    DUT.setAddress( 0 );
    DUT.clockTick();
    int32_t result = DUT.getOutput();

    // test that we got back what we wrote
    if ( result != htobe32(100) )
        errExit( " RAM test failed. We did not read back what we wrote" );

    debug( "All test passed for RAM" );
    return EXIT_SUCCESS;
}
