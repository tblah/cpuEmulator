// ALU module for the CPU
// All implemented as combinational logic

/* IO
    Inputs:
        aluControl (see aluOps.h)
        A (first arguement)
        B (second arguement)
    
    Outputs:
        result (the result of the operation)
        zeroFlag (for BNE)
        positive (for BLT)
*/

/* we have different getters and setters depending on wheather the desired format
   is appropiate for the host cpu or the cpu we are emulating. Adds for the program
   counter will be done for the host cpu as this is internal to the emulation and so
   I do not consider it important to keep the address numbers accurate. However, 
   for the execution of memory images to be accurate, the ALU has to also support 
   operating on numbers represented for the cpu being emulated. This is nessecarry
   in order to provide accurate emulation. */ 

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

#ifndef ALU_H
#define ALU_H

#include "../emulator/Signal.h"
#include <stdint.h>
#include "aluOps.h"

class ALU {
    private:
        Signal<AluOps> control;
        Signal<int32_t> A;
        Signal<int32_t> B;
        
        Signal<int32_t> result;
        Signal<bool> zeroFlag;
        Signal<bool> positive;

        // combinational logic
        void setFlags( void );
        void updateOutputs( void );

        // have we already calculated the value of the outputs since the last change in inpus
        bool upToDate;

        // handle different number representation between the machine we are doing this emulation on and the cpu design we are emulating
        int32_t fixNumber( int32_t x );

    public:
        ALU( void );
        void setControl( AluOps controlIn );
        void setA( int32_t Aval );
        void nativeSetA( int32_t Aval ); // doesn't run fixNumber() on it
        void setB( int32_t Bval );
        void nativeSetB( int32_t Bval ); // doesn't run fixNumber() on it
        void undefine( void ); // undefine signals (called from something sequential so that inputs can't remember their values)

        int32_t getResult( void ); // no conversion for host machine
        int32_t getNativeResult( void ); // conversion to host machine
        bool getZeroFlag( void );
        bool getPositiveFlag( void );
};

#endif
