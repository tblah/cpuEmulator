// ram. This is currently the same speed as the registers
/* IO
    Inputs:
    -Address
    -reading
    -clock

    Input/Output:
    -data
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

#ifndef RAM_H
#define RAM_H

#include "../emulator/Register.h"
#include "../emulator/Signal.h"
#include "../emulator/debug.h"
#include <stdint.h>
#include <vector>

#include <endian.h>

template <typename AddressType, unsigned int numBytes>
class RAM {
    private:
        Register<int8_t> data[numBytes]; // the ram will behave as a lot faster than real ram 
        Signal<AddressType> addr;
        Signal<bool> readingThisCycle;
        Signal<int32_t> inoutData;

    public:
        // constructor to start the ram with some initial data
        RAM( const std::vector<int32_t> &InitialData ) {
            if ( InitialData.size() > (numBytes*sizeof(int32_t)) )
                errExit( "Initial RAM data does not fit" );

            unsigned int nextRamAddr = 0;

            for ( unsigned i = 0; i != InitialData.size(); ++i) {
                // for each instruction
                int32_t fullObj = InitialData[i]; 

                // copy data
                for ( unsigned int j = 0; j < sizeof(int32_t); ++j ) 
                    data[nextRamAddr+j].changeDriveSignal( ( (int8_t*) &fullObj )[j] );
                
                nextRamAddr += sizeof(int32_t);
            }
            
            // we need a clock tick so that the data can be written
            clockTick();
        }

        // not to be used in hardware modeling. This does a read in a C++ way
        int32_t debugRead( AddressType addr ) {
            // read the data one byte at a time
            int8_t readData[ sizeof( int32_t ) ];

            for ( size_t i = 0; i < sizeof( int32_t ); i++ ) {
                readData[i] = data[ addr + i ].getOutput();
            }

            debugSignal( "debugRead from RAM returning", *((int32_t*) &readData) );

            return  *((int32_t*) &readData) ;
        }

        void setAddress( AddressType address ) {
            if ( address > (numBytes-1) ) {
                errExit( "RAM: specified address does not exist" );
            }

            addr.setValue( address );
        }

        void setReadingThisCycle( bool rwControl ) {
            readingThisCycle.setValue( rwControl );
        }

        void setDataIn( int32_t inData ) {
            inoutData.setValue( inData );
        }

        int32_t getOutput( void ) {
            return inoutData.getValue();
        }

        void clockTick( void ) {
            if ( readingThisCycle.isDefined() ) { // if we are doing anything
                if ( addr.isDefined() ) { // if we got all of our inputs for reading
                    if ( readingThisCycle.getValue() ) { // we are reading
                        // nobody can drive inoutData while we are
                        if ( inoutData.isDefined() )
                            errExit( "RAM and RAM input driving inOutData at the same time!" );

                        // read the data one byte at a time
                        int8_t readData[ sizeof( int32_t ) ];
                        for ( size_t i = 0; i < sizeof( int32_t ); i++ ) {
                            readData[i] = data[ addr.getValue() + i ].getOutput();
                        }
                        // interpret readData's bits as a int32_t
                        // this breaks strict aliasing so this file must have
                        // -fstrict-aliasing as a compiler option
                        // an alternative would be to mess about with unions 
                        inoutData.setValue( be32toh( *( (int32_t*) &readData ) ) );

                    } else { // writing
                        if ( inoutData.isDefined() ) { // we have all inputs for write
                            // write one byte at a time
                            // interpret the input's bits as an array of bytes
                            int32_t inData = inoutData.getValue();
                            int8_t* dataToWrite = (int8_t*) &inData; // treat as an array of int8_t
                            for ( size_t i = 0; i < sizeof( int32_t ); i++ ) {
                                data[ addr.getValue() + i ].changeDriveSignal( dataToWrite[i] );
                            }

                            debugSignal( "ram at address " + std::to_string(addr.getValue()), inoutData.getValue());

                            // outData should not remember it's value
                            inoutData.undefine();

                        } else { // missing inOutData
                            errExit( "You are trying to write to RAM without any data" );
                        }
                    }
                } else { // missing addr (and maybe inoutData)
                    errExit( "In RAM, you set readingThisCycle but not addr. Undefined response." );
                }
            } else { // atleast one required input is missing so let's assume we are doing nothing
                inoutData.undefine();
            }
            
            // always undefine inputs for the new clock cycle (so they don't remember anything)
            addr.undefine();
            readingThisCycle.undefine();
            
            // clock tick for memory cells
            for (unsigned int i = 0; i < (numBytes-1); i++)
                data[i].clockTick();
        }
};

#endif
