// Class representing a bidirectional bus which could be driven by anything connected to it. This wraps Signal, making sure only one thing can drive the bus at any time. 

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

#ifndef BUS_H
#define BUS_H

#include "Signal.h"
#include "debug.h"
#include <limits.h>

template <typename Type> class Bus {
    private:
        Signal<Type> BusSignal;
        long owner; // ID of the object driving the bus
        long nextID; // next unallocated ID

    public:
        Bus( void ) {
            owner = -1; // -1 signifies that there is no owner
            nextID = 0;          
        }

        // allows objects to register a unique ID for bus ownership
        long registerID( void ) { // don't multithread this
            if (nextID == LONG_MAX) { // don't overflow nextID
                errExit( "Bus ran out of ID space. nextID would overflow" );
            }
            
            return nextID++; // postincrement: returns old value of nextID
        }

        // allows an object to claim exclusive ability to drive the bus
        void claimOwnership( long ID ) {
            if ( owner != -1 ) {
                errExit( "Two objects tried to claim ownership of a Bus \
                            at the same time" );
            } else {
                owner = ID;
            }
        }

        void surrenderOwnership( long ID ) {
            if ( ID != owner ) {
                errExit( "An object which is not the owner of a Bus tried to \
                            surrender ownerhip");
            } else {
                owner = -1; // signifies no owner
            }
        }

        void driveValue( long ID, Type DrivenValue ) {
            if ( ID != owner ) {
                errExit( "An object which is not the owner of a Bus tried to \
                            drive a value");
            } else {
                BusSignal = DrivenValue;
            }
        }

        Type readValue( void ) {
            return BusSignal.getValue(); // Signal checks if it is undefined
        }
};

#endif
