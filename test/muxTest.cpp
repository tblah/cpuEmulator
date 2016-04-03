// test for mux

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

#include "../emulator/mux.h"
#include "../emulator/debug.h"
#include <stdlib.h>

// a key type for the mux
enum class Key {
    one,
    two,
    three
};

int main( void ) {
    debug( "Beginning mux tests" );

    // try special version for enums
    Mux<Key, int> DUT;
    DUT.setInput( Key::one, 1 );
    DUT.setInput( Key::two, 2 );
    DUT.setInput( Key::three, 3 );

    DUT.setSelect( Key::one );
    if ( DUT.getOutput() != 1 )
        errExit( "1" );

    DUT.setSelect( Key::two );
    if ( DUT.getOutput() != 2 )
        errExit( "2" );

    DUT.setSelect( Key::three );
    if ( DUT.getOutput() != 3 )
        errExit( "3" );

    DUT.setInput( Key::three, -3 );
    if ( DUT.getOutput() != -3 )
        errExit( "-3" );

    // now try with integer keys
    Mux<int, int> DUT2;

    DUT2.setInput(1, 1);
    DUT2.setSelect(1);

    if (DUT2.getOutput() != 1 )
        errExit( "int 1" );
    
    debug( "All mux tests passed" );
    return EXIT_SUCCESS;
}
