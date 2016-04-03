// tests for cpu/alu

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

#include "../cpu/alu.h"
#include "../emulator/debug.h"
#include "../cpu/aluOps.h"
#include <stdlib.h>
#include <endian.h>

int main( void ) {
    debug( "Beginning ALU tests" );
    
    ALU DUT;

    DUT.setA( (int32_t) htobe32(1) );
    DUT.setB( (int32_t) htobe32(2) );

    // add
    DUT.setControl( AluOps::add );

    if ( (DUT.getNativeResult() != 3) || DUT.getZeroFlag() || !DUT.getPositiveFlag() )
        errExit( "failed test(s) for add" );

    // sub
    DUT.setControl( AluOps::sub );

    if ( (DUT.getNativeResult() != -1) || DUT.getZeroFlag() || DUT.getPositiveFlag() )
        errExit( "failed test(s) for sub" );
    
    // nand
    DUT.setControl( AluOps::nand );

    // different architechtures might do the sign differently so let's interpret the bits as unsigned for this test
    int32_t signedResult = DUT.getNativeResult();
    uint32_t testResult = *( (uint32_t*) &signedResult);
    if ( (testResult != 0xFFFFFFFF) || DUT.getZeroFlag() )
        errExit( "failed test(s) for nand" );

    // lshift
    DUT.setControl( AluOps::lshift );
    
    if ( (DUT.getNativeResult() != 4 ) || DUT.getZeroFlag() || !DUT.getPositiveFlag() )
        errExit( "failed test(s) for lshift" );

    // nop
    // no outputs to test here. Just run it to see if anything errExit's
    DUT.setControl( AluOps::nop );

    // zero flag
    DUT.setA( - ( (int32_t) htobe32(1024)) );
    DUT.setB( (int32_t) htobe32(1024) );
    DUT.setControl( AluOps::add );

    if ( (DUT.getNativeResult() != 0) || !DUT.getZeroFlag() || !DUT.getPositiveFlag() )
        errExit( "failed test(s) for zero flag" );

    // native output
    DUT.setA( htobe32(3) );
    DUT.setB( htobe32(3) );
    DUT.setControl( AluOps::add );

    int32_t expectedResult =  ((int32_t) htobe32(6));
    if ( DUT.getResult() != expectedResult )
        errExit( "failed test for alternate output" );

    debug( "All ALU tests passed" );    

    return EXIT_SUCCESS;
}
