// Headerfile for the class containing the RegistersFile. The Registers file is all of the general purpouse registers accessable to the programmer.

/* IO:
    ReadSelect1 - select which register to read from on line 1
    Out1 - output from reading the first register
    ReadSelect2
    Out2
    (bool) reading - choose if we are reading or writing this cycle
    WriteData - Data to write
    WriteSelect - choose which register to write to
*/

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

#ifndef REGISTERSFILE_H
#define REGISTERSFILE_H

#include "../emulator/Register.h"
#include "../emulator/Signal.h"
#include "../emulator/debug.h"

#include <iostream>
using namespace std;

template <typename DataType, typename IndexType, unsigned int numRegisters> 
class RegisterFile {
    private:
        // representing values on the IO to the Register File on the clock edge
        // everything is a Signal so that undefined in will mean undefined out
        Signal<IndexType> readSelect1;
        Signal<IndexType> readSelect2;

        Signal<DataType> out1;
        Signal<DataType> out2;

        Signal<bool> readThisCycle;

        Signal<IndexType> writeSelect;
        Signal<DataType> writeData;

        // private utility function for checking that input register indexes are sane
        // not actually part of the hardware. This is to get error messages for debugging
        void validateRegisterIndex( IndexType regIndex ) {
            if ( regIndex > numRegisters-1 )
                errExit( "invalid register index" );
        }

        // The registers
        Register<DataType> registers[numRegisters]; 

    public:
        RegisterFile( void ) {
            // each Register object will have already done this for it's data
            // each Signal starts undefined too :-)
    
            // register 0 is always = 0. Write this now
            registers[0].changeDriveSignal( 0 );
        }

        void clockTick( void ){
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
                        // many instructions only read from channel 1. Don't panic
                        //debug( "incomplete input to RegistersFile on channel 2 read" );
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
                        cout << "wrote " << writeData.getValue() << " to " <<
                            (int) writeSelect.getValue() << endl;
                    } else { // something is wrong
                        debug( "incomplete input to RegistersFile on write" );
                    }
                }
            } else { // not doing anything this cycle
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
            for (unsigned int i = 0; i < numRegisters-1; i++)
                registers[i].clockTick();

        } // end of clockTick()

        // inputs
        void setReadSelect1( IndexType regIndex ) {
            validateRegisterIndex( regIndex );
            readSelect1.setValue( regIndex );
        }

        void setReadSelect2( IndexType regIndex ) {
            validateRegisterIndex( regIndex );
            readSelect2.setValue( regIndex );
        }

        void setReadThisCycle( bool rwControl ) {
            readThisCycle.setValue( rwControl );
        }

        void setWriteSelect( IndexType regIndex ) {
            validateRegisterIndex( regIndex );

            // we cannot write to register 0
            if ( regIndex == 0 ) {
                debug( "Illegal attempt to write to register 0. Ignoring." );
            } else {
                writeSelect.setValue( regIndex );
            }
        }

        void setWriteData( DataType data ) {
            writeData.setValue( data );
        }

        // outputs
        DataType getOut1( void ) {
            return out1.getValue(); // checking done in Signal
        }

        DataType getOut2( void ) { 
            return out2.getValue();
        }
};

#endif
