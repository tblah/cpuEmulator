// Template Class representing an arbitary length register containing an arbitary data type
// Basically a wrapper around Signal to make changes only take effect on a clock tick and to undefine anything read from an unspecified input

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

#ifndef  REGISTER_H
#define REGISTER_H

#include "Signal.h"

template <typename Type> class Register {
    public:
        void changeDriveSignal( Type signalValue ) {
            QNext.setValue( signalValue );
        }

        void clockTick( void ) {
            Q = QNext;

            QNext.undefine(); // we don't want QNext to remember anything
        }

        Type getOutput( void ) {
            return Q.getValue();
        }

    private:
        Signal<Type> Q;
        Signal<Type> QNext;
};

#endif
