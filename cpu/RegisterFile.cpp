// Register File Class. See headder for documentation

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

#include "RegisterFile.h"
#include "../emulator/debug.h"

// private utility function for checking that input register indexes are sane
// not actually part of the hardware. This is to get error messages for debugging
void RegisterFile::validateRegisterIndex( uint8_t regIndex ) {
    if ( regIndex > 31 )
        errExit( "invalid register index" );
}

// constructor
RegisterFile::RegisterFile( void ) {
    // each Register object will have already done this for it's data
    // each Signal starts undefined too :-)
    
    // register 0 is always = 0. Write this now
    registers[0].changeDriveSignal( 0 );
}

void RegisterFile::clockTick( void ) {
    if ( readThisCycle.isDefined() ) { // if we are doing anything this cycle
        if ( readThisCycle.getValue() ) { // we are reading
            // did we get all the inputs we need?

            // channel 1
            if ( readSelect1.isDefined() ) {
                // do read
                out1.setValue( registers[ readSelect1.getValue() ].getOutput() );
            } else {
                out1.undefine();
                debug( "incomplete input to RegistersFile on channel 1 read" );
            }

            // channel 2
            if ( readSelect2.isDefined() ) {
                // do read
                out2.setValue( registers[ readSelect2.getValue() ].getOutput() );
            } else {
                out2.undefine();
                debug( "incomplete input to RegistersFile on channel 2 read" );
            }

        } else { // we are writing
            // our read outputs have no meaning when we write
                out1.undefine();
                out2.undefine();

            // did we get all the inputs we need?
            if ( writeSelect.isDefined() && writeData.isDefined() ) { 
                // do the write
                registers[ writeSelect.getValue() ].changeDriveSignal( 
                    writeData.getValue() );
            } else { // something is wrong
                debug( "incomplete input to RegistersFile on write" );
            }
        }
    } else {
        out1.undefine();
        out2.undefine();
    }

    // Make everything undefined so the combinational inputs are not remembering their values from the last clock cycle
    readSelect1.undefine();
    readSelect2.undefine();
    readThisCycle.undefine();
    writeSelect.undefine();
    writeData.undefine();

    // registers all need a clock
    for (int i = 0; i < 31; i++)
        registers[i].clockTick();
}

void RegisterFile::setReadSelect1( uint8_t regIndex ) {
    validateRegisterIndex( regIndex );
    readSelect1.setValue( regIndex );
}

void RegisterFile::setReadSelect2( uint8_t regIndex ) {
    validateRegisterIndex( regIndex );
    readSelect2.setValue( regIndex );
}

void RegisterFile::setReadThisCycle( bool rwControl ) {
    readThisCycle.setValue( rwControl );
}

void RegisterFile::setWriteSelect( uint8_t regIndex ) {
    validateRegisterIndex( regIndex );

    if ( regIndex == 0 ) {
        debug( "Illegal attempt to write to register 0. Ignoring." );
    } else { // valid request so actually set the variable
        writeSelect.setValue( regIndex );
    }
}

void RegisterFile::setWriteData( int32_t data ) {
    writeData.setValue( data );
}

int32_t RegisterFile::getOut1( void ) {
    return out1.getValue(); // checking done in Signal
}

int32_t RegisterFile::getOut2( void ) {
    return out2.getValue();
}

