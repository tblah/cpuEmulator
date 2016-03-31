// tests for Bus

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

#include "../emulator/debug.h"
#include "../emulator/Bus.h"
#include <stdlib.h>

int main( void ) {
    debug("Beginning Bus tests");

    Bus<bool> DUT;

    long ID = DUT.registerID();

    // test that the ID is reasonable
    if ( ID < 0 )
        errExit( "BUS ID is negative" );
    
    DUT.claimOwnership( ID );
    DUT.driveValue( ID, true );

    // test that this value was given to the bus
    if ( DUT.readValue() != true )
        errExit( "Bus value is incorrect" );

    DUT.surrenderOwnership( ID ); // see if this throws an errExit

    debug("Bus tests all passed");

    return EXIT_SUCCESS;
}

