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
    along with cpuEmulator.  If not, see http://www.gnu.org/licenses/.*/

#include "alu.h"
#include "../emulator/debug.h"

// endian-ness stuff
//#define __BSD_SOURCE // already defined by gcc. Other compilers may vary??
#include <endian.h>

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
            case (AluOps::add):
                result.setValue( A.getValue() + B.getValue() );
                break;
        
            case (AluOps::sub):
                result.setValue( A.getValue() - B.getValue() );
                break;
    
            case (AluOps::nand):
                result.setValue( ~(A.getValue() & B.getValue()) );
                break;

            case (AluOps::lshift):
                result.setValue( A.getValue() << B.getValue() );
                break;

            case (AluOps::nop):
                undefine();
                return;
        }

        setFlags();
    }

    upToDate = true;
}

int32_t ALU::fixNumber( int32_t x ) {
    // the cpu (and machine code assembled for it) is big endian
    // and it uses two's compliment for all numbers
    // this may not be true for the machine doing the emulating. This could break adding and subtracting so we will fix numbers here
    
    // the endian functions operate on unsigned numbers. This doesn't matter because we don't know which bits are which yet
    int32_t result = abs(x);
    uint32_t unsignedX = be32toh( *((uint32_t*) &result) );

    // this won't overflow because we started with a signed number
    result = (int32_t) unsignedX;

    // assume the host machine uses two's compliment because there do not seem to be anything in standard libraries for this conversion. Most machines use two's compliment.
    // source: http://www.cs.umd.edu/class/sum2003/cmsc311/Notes/Data/twoscomp.html
    
    // negative if the most significant bit is 1, otherwise positive
    if (x >> 31) // = 1 for negative, = 0 for positive
        result = -result;

    return result;
}

ALU::ALU( void ) {
    upToDate = false;
}

void ALU::setControl( AluOps controlIn ) {
    upToDate = false;
    control.setValue( controlIn );
}

void ALU::setA( int32_t Aval ) {
    upToDate = false;
    A.setValue( Aval );
}

void ALU::nativeSetA( int32_t Aval ) {
    upToDate = false;
    A.setValue( fixNumber(Aval) );
}

void ALU::setB( int32_t Bval ) {
    upToDate = false;
    B.setValue( fixNumber(Bval) );
}

void ALU::nativeSetB( int32_t Bval ) {
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
    
    // convert result back to a number the cpu is supposed to understand
    uint32_t ret = abs( result.getValue());
    
    if ( result.getValue() < 0 ) {
        ret = ~ret;
        ret += 1;
    }
    
    return htobe32( ret );
}

int32_t ALU::getNativeResult( void ) {
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
