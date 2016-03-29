// test for Register class

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
    along with picomips-cpu.  If not, see http://www.gnu.org/licenses/.*/

#include "../emulator/Register.h"
#include "../emulator/debug.h"
#include <stdlib.h>

int main( void ) {
    debug( "Beginning tests of Register Class" );
    Register<bool> DUT;

    DUT.changeDriveSignal( true );
    DUT.clockTick();

    // test this value was propegated to the output
    if ( DUT.getOutput() != true ) {
        errExit( "Register input did not become output on clock tick" );
    }

    DUT.changeDriveSignal( false );

    // test this was not yet put on the output
    if ( DUT.getOutput() != true ) {
        errExit( "Register output changed too soon" );
    }

    debug( "All Register tests passed" );

    return EXIT_SUCCESS;
}
