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

    public:
        ALU( void );
        void setControl( AluOps controlIn );
        void setA( int32_t Aval );
        void setB( int32_t Bval );
        void undefine( void ); // undefine signals (called from something sequential so that inputs can't remember their values)

        int32_t getResult( void );
        bool getZeroFlag( void );
        bool getPositiveFlag( void );
};

#endif
