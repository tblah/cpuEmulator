// tests for RegisterFile

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
    along with cpuEmulator.  If not, see http://www.gnu.org/licenses/. */

#include "../emulator/RegisterFile.h"
#include "../emulator/debug.h"
#include <stdlib.h>
#include <stdint.h>

int main( void ) {
    debug( "Begginning RegisterFile tests" );
    
    RegisterFile<int32_t, uint8_t, 16> DUT;

    // try a write
    DUT.setReadThisCycle( false );
    DUT.setWriteSelect( 1 );
    DUT.setWriteData( 5 );
    DUT.clockTick();

    // read it back again on channel 1
    // and read register 0 on channel 2
    DUT.setReadThisCycle( true );
    DUT.setReadSelect1( 1 );
    DUT.setReadSelect2( 0 );
    DUT.clockTick();

    // test channel 1
    if ( DUT.getOut1() != 5 )
        errExit( "The value read out of register 1 is not the one we put in there" );

    // test channel 2
    if ( DUT.getOut2() != 0 )
        errExit( "The value read out of register 0 is not 0" );

    debug("All RegisterFile tests passed" );

    return EXIT_SUCCESS;
}
