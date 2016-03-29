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
    along with picomips-cpu.  If not, see http://www.gnu.org/licenses/.*/

#ifndef REGISTERSFILE_H
#define REGISTERSFILE_H

#include "../emulator/Register.h"
#include <stdint.h>
#include "../emulator/Signal.h"

class RegisterFile {
    private:
        // representing values on the IO to the Register File on the clock edge
        // everything is a Signal so that undefined in will mean undefined out
        Signal<uint8_t> readSelect1;
        Signal<uint8_t> readSelect2;

        Signal<int32_t> out1;
        Signal<int32_t> out2;

        Signal<bool> readThisCycle;

        Signal<uint8_t> writeSelect;
        Signal<int32_t> writeData;

        void validateRegisterIndex( uint8_t regIndex );

        // The registers
        Register<int32_t> registers[32]; 

    public:
        RegisterFile( void );

        void clockTick( void );

        // inputs
        void setReadSelect1( uint8_t regIndex );
        void setReadSelect2( uint8_t regIndex );
        void setReadThisCycle( bool rwControl );
        void setWriteSelect( uint8_t regIndex );
        void setWriteData( int32_t data );

        // outputs
        int32_t getOut1( void );
        int32_t getOut2( void );
};


#endif
