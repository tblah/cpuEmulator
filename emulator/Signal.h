// Template class to represent an arbitary signal

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

#ifndef  SIGNAL_H
#define SIGNAL_H

#include "debug.h"

template <typename Type> class Signal {
    private:
        bool undefined;
        Type Value;

    public:
        Signal( void ) { // constructor for having no value
            undefined = true;
            Value = Type();
        }

        Signal( Type GivenValue ) { // constructor with a value
            setValue( GivenValue );
        }

        void undefine( void ) { // set this signal as undefined
            undefined = true;
        }

        bool isUndefined( void ) {
            return undefined;
        }

        bool isDefined( void ) {
            return !undefined;
        }

        void setValue( Type GivenValue ) {
            undefined = false;
            Value = GivenValue;
        }

        Type getValue( void ) {
            if ( undefined ) {
                errExit( "getting the value of an undefined signal" );
            }

            return Value;
        }
};

#endif
