// ALU class
// see headerfile for more documentation

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

#include "alu.h"
#include "../emulator/debug.h"

void ALU::setFlags( void ) {
    int32_t val = result.getValue();
    
    // defining 0 as positive so that "branch less than" is a less than and not a less than or equal to
    if (val >= 0)
        positive.setValue( true );
    else
        positive.setValue( false );

    if (val == 0)
        zeroFlag.setValue( true );
    else
        zeroFlag.setValue( false );
}

void ALU::updateOutputs( void ) {
    if ( !upToDate ) {
        if ( A.isUndefined() || B.isUndefined() || control.isUndefined() ) {
            // not all inputs set
            return;
        }

        // calculate result
        switch ( control.getValue() ) {
            case (add):
                result.setValue( A.getValue() + B.getValue() );
                break;
        
            case (sub):
                result.setValue( A.getValue() - B.getValue() );
                break;
    
            case (nand):
                result.setValue( ~(A.getValue() & B.getValue()) );
                break;

            case (lshift):
                result.setValue( A.getValue() << B.getValue() );
                break;

            case (nop):
                undefine();
                return;
        }

        setFlags();
    }

    upToDate = true;
}

ALU::ALU( void ) {
    upToDate = false;
}

void ALU::setControl( aluOps controlIn ) {
    upToDate = false;
    control.setValue( controlIn );
}

void ALU::setA( int32_t Aval ) {
    upToDate = false;
    A.setValue( Aval );
}

void ALU::setB( int32_t Bval ) {
    upToDate = false;
    B.setValue( Bval );
}

void ALU::undefine( void ) {
    upToDate = false;
    control.undefine();
    A.undefine();
    B.undefine();
    result.undefine();
    zeroFlag.undefine();
    positive.undefine();
}

int32_t ALU::getResult( void ) {
    updateOutputs();
    return result.getValue();
}

bool ALU::getZeroFlag( void ) {
    updateOutputs();
    return zeroFlag.getValue();
}

bool ALU::getPositiveFlag( void ) {
    updateOutputs();
    return positive.getValue();
}
