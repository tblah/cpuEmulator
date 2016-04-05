// encapsulating class represinting the whole CPU
// control unit logic is in here because it needs access to everything anyway

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

#include "CPU.h"
#include "../emulator/debug.h"

// to do
// control unit logic
inline void CPU::fetch( void ) {

}

inline void CPU::decode( void ) {

}

inline void CPU::execute( void ) {

}

inline void CPU::write( void ) {

}

CPU::CPU( const std::vector<int32_t> &InitialRamData ) {
    ram = new RAM<int32_t, int32_t, 1024> ( InitialRamData );

    halted.changeDriveSignal( false );
    halted.clockTick();

    controlUnitState.changeDriveSignal( ControlUnitStateEnum::Fetch );
    controlUnitState.clockTick();
}

CPU::~CPU( void ) {
    delete ram;
}

bool CPU::clockTick( void ) {
    // don't bother doing anything if we are halted
    if ( halted.getOutput() )
        return halted.getOutput();

    // all of the combinational logic must not remember stuff from the previous cycle
    alu.undefine();
    decoder.undefine();
    aluBMux.undefine();
    PCMux.undefine();
    ifZeroMux.undefine();
    ifPositiveMux.undefine();
    regWriteSelectMux.undefine();
    regWriteDataMux.undefine();

    // do all combinational logic
    switch( controlUnitState.getOutput() ) {
        case (ControlUnitStateEnum::Fetch):
            fetch();
            break;
        
        case (ControlUnitStateEnum::Decode):
            decode();
            break;
    
        case (ControlUnitStateEnum::Execute):
            execute();
            break;

        case (ControlUnitStateEnum::Write):
            write();
            break;

        default:
            errExit( "illigal value in controlUnitState register" );
    }

    // now clock tick everything sequential
    registers.clockTick();
    programCounter.clockTick();
    instructionRegister.clockTick();
    PCplus4.clockTick();
    resultArg.clockTick();
    immediate.clockTick();
    A.clockTick();
    B.clockTick();
    aluResult.clockTick();
    zero.clockTick();
    positive.clockTick();
    ramRead.clockTick();
    halted.clockTick();
    controlUnitState.clockTick();
    
    return halted.getOutput();   
}


