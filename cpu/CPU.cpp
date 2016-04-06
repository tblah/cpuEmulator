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
#include "aluOps.h"

// to do
// control unit combinational logic
inline void CPU::fetch( void ) {
    debug( "fetch" );
    // read the next instruction from the RAM into the instruction register
    ram->setReadingThisCycle( true );
    ram->setAddress( programCounter.getOutput() );
    
    controlUnitState.changeDriveSignal( ControlUnitStateEnum::Decode );   

    // reset registers which shouldn't be remembering anything from the previous cycle
    PCplus4.reset();
    resultArg.reset();
    immediate.reset();
    aluResult.reset();
    currentOpcode.reset();
}

inline void CPU::decode( void ) {
    debug( "decode" );
    // decode the instruction we just read from RAM and read those registers
    decoder.setMemoryWord( ram->getOutput() );
    currentOpcode.changeDriveSignal( decoder.getOpcode() );

    // only get what we want so we don't read any undefined signals from decoder
    // in a real control unit all the logic would happen 
    switch (decoder.getOpcode()) {
        case ( Opcode::add ):
        case ( Opcode::sub ):
        case ( Opcode::nand ):
        case ( Opcode::lshift ):
            // reads from register file
            registers.setReadThisCycle( true );
            registers.setReadSelect1( decoder.getA() );
            registers.setReadSelect2( decoder.getB() );

            resultArg.changeDriveSignal( decoder.getResult() );
            break;

        case ( Opcode::addImmediate ):
        case ( Opcode::subImmediate ):
            registers.setReadThisCycle( true );
            registers.setReadSelect1( decoder.getA() );
            immediate.changeDriveSignal( decoder.getImmediate() );
            break;

        case ( Opcode::jumpToReg ):
        case ( Opcode::branchIfZero ):
        case ( Opcode::branchIfPositive ):
            registers.setReadThisCycle( true );
            registers.setReadSelect1( decoder.getA() );
            break;

        case ( Opcode::load ):
            registers.setReadThisCycle( true );
            registers.setReadSelect1( decoder.getA() );

            resultArg.changeDriveSignal( decoder.getResult() );
            break;

        case ( Opcode::store ):
            registers.setReadThisCycle( true );
            registers.setReadSelect1( decoder.getA() );
            registers.setReadSelect2( decoder.getB() );
            break;

        case ( Opcode::nop ):
        case ( Opcode::halt ):
            // no arguements
            break;

        default:
            errExit( "In cpu/decode, invalid opcode" );
    }

    // calculate what the next value of the program counter probably will be
    alu.setControl( AluOps::add );
    alu.setA( programCounter.getOutput() );
    alu.setB( sizeof(int32_t) );
    PCplus4.changeDriveSignal( alu.getResult() );
    controlUnitState.changeDriveSignal( ControlUnitStateEnum::Execute );
}

inline void CPU::execute( void ) {
    debug( "execute" );
    // actually execure the instructions
    // to keep the code simple we are not using aluBMux explicitly
    switch ( currentOpcode.getOutput() ) {
        case ( Opcode::add ):
            debug( "add exec" );
            alu.setA( registers.getOut1() );
            alu.setB( registers.getOut2() );
            alu.setControl( AluOps::add );

            aluResult.changeDriveSignal( alu.getResult() );
            zero.changeDriveSignal( alu.getZeroFlag() );
            positive.changeDriveSignal( alu.getPositiveFlag() );

            programCounter.changeDriveSignal( PCplus4.getOutput() );
            break;

        case ( Opcode::sub ):
            debug( "sub exec" );
            alu.setA( registers.getOut1() );
            alu.setB( registers.getOut2() );
            alu.setControl( AluOps::sub );

            aluResult.changeDriveSignal( alu.getResult() );
            zero.changeDriveSignal( alu.getZeroFlag() );
            positive.changeDriveSignal( alu.getPositiveFlag() );
 
            programCounter.changeDriveSignal( PCplus4.getOutput() );
            break;

        case ( Opcode::nand ):
            debug( "nand exec" );
            alu.setA( registers.getOut1() );
            alu.setB( registers.getOut2() );
            alu.setControl( AluOps::nand );

            aluResult.changeDriveSignal( alu.getResult() );
            zero.changeDriveSignal( alu.getZeroFlag() );
            positive.changeDriveSignal( alu.getPositiveFlag() );
 
            programCounter.changeDriveSignal( PCplus4.getOutput() );
            break;

        case ( Opcode::lshift ):
            debug( "lshift exec" );
            alu.setA( registers.getOut1() );
            alu.setB( registers.getOut2() );
            alu.setControl( AluOps::lshift );
 
            aluResult.changeDriveSignal( alu.getResult() );
            zero.changeDriveSignal( alu.getZeroFlag() );
            positive.changeDriveSignal( alu.getPositiveFlag() );
 
            programCounter.changeDriveSignal( PCplus4.getOutput() );
            break;

        case ( Opcode::addImmediate ):
            debug( "addImmediate exec" );
            alu.setA( registers.getOut1() );
            alu.setB( immediate.getOutput() );
            alu.setControl( AluOps::add );
 
            aluResult.changeDriveSignal( alu.getResult() );
            zero.changeDriveSignal( alu.getZeroFlag() );
            positive.changeDriveSignal( alu.getPositiveFlag() );
 
            programCounter.changeDriveSignal( PCplus4.getOutput() );
            break;

        case ( Opcode::subImmediate ):
            debug( "subImmediate exec" );
            alu.setA( registers.getOut1() );
            alu.setB( immediate.getOutput() );
            alu.setControl( AluOps::sub );
 
            aluResult.changeDriveSignal( alu.getResult() );
            zero.changeDriveSignal( alu.getZeroFlag() );
            positive.changeDriveSignal( alu.getPositiveFlag() );
 
            programCounter.changeDriveSignal( PCplus4.getOutput() );
            break;

        case ( Opcode::jumpToReg ):
            debug( "J2R exec" );
            programCounter.changeDriveSignal( registers.getOut1() );
            break;

        case ( Opcode::branchIfZero ):
            debug( "BIZ exec" );
            // using ifZeroMux as an if statement
            // this is using the value of zero from whenever the ALU last did
            //          something in the execute stage
            ifZeroMux.setInput( true, registers.getOut1() );
            ifZeroMux.setInput( false, PCplus4.getOutput() );
            ifZeroMux.setSelect( zero.getOutput() );

            programCounter.changeDriveSignal( ifZeroMux.getOutput() );
            break;

        case ( Opcode::branchIfPositive ):
            debug( "BIP exec" );
            // see notes for branchIfZero
            ifPositiveMux.setInput( true, registers.getOut1() );
            ifPositiveMux.setInput( false, PCplus4.getOutput() );
            debug("nhj");
            ifPositiveMux.setSelect( positive.getOutput() );
            debug("wat");
            programCounter.changeDriveSignal( ifPositiveMux.getOutput() );
            break;

        case ( Opcode::load ):
            debug( "load exec" );
            ram->setReadingThisCycle( true );
            ram->setAddress( registers.getOut1() );
 
            programCounter.changeDriveSignal( PCplus4.getOutput() );
            break;

        case ( Opcode::store ):
            debug( "store exec" );
            ram->setReadingThisCycle( false ); // write
            ram->setAddress( registers.getOut1() );
            ram->setDataIn( registers.getOut2() );
 
            programCounter.changeDriveSignal( PCplus4.getOutput() );
            break;

        case ( Opcode::nop ):
            // nothing needs doing
            debug( "nop exec" );
            programCounter.changeDriveSignal( PCplus4.getOutput() );
            break;

        case ( Opcode::halt ):
            debug( "halt exec" );
            halted.changeDriveSignal( true );
            programCounter.reset(); // errExit if we don't actually halt
            break;

        default:
            errExit( "In cpu/execute, invalid opcode" );
    }

    controlUnitState.changeDriveSignal( ControlUnitStateEnum::Write );
}

inline void CPU::write( void ) {
    debug( "write" );
    switch ( currentOpcode.getOutput() ) {
        case ( Opcode::addImmediate ):
        case ( Opcode::subImmediate ):
            // write the result back to register 1
            registers.setReadThisCycle( false ); // write
            registers.setWriteSelect( 1 );
            registers.setWriteData( aluResult.getOutput() );
            break;

        case ( Opcode::add ):
        case ( Opcode::sub ):
        case ( Opcode::nand ):
        case ( Opcode::lshift ):
            // write back to the specified register
            registers.setReadThisCycle( false ); // write
            registers.setWriteSelect( resultArg.getOutput() );
            registers.setWriteData( aluResult.getOutput() );
            break;

        case ( Opcode::load ):
            registers.setReadThisCycle( false ); // write
            registers.setWriteSelect( resultArg.getOutput() );
            registers.setWriteData( ram->getOutput() );
            break;
        
        case ( Opcode::nop ):
        case ( Opcode::jumpToReg ):
        case ( Opcode::branchIfZero ):
        case ( Opcode::branchIfPositive ):
        case ( Opcode::store ):
        case ( Opcode::halt ):
            // TODO: make other instructions skip this step so they execute a bit faster
            break;

        default:
            errExit( "cpu/write invalid opcode" );
    }

    controlUnitState.changeDriveSignal( ControlUnitStateEnum::Fetch );
}

CPU::CPU( const std::vector<int32_t> &InitialRamData ) {
    ram = new RAM<int32_t, int32_t, 1024> ( InitialRamData );

    halted.changeDriveSignal( false );
    halted.clockTick();

    controlUnitState.changeDriveSignal( ControlUnitStateEnum::Fetch );
    controlUnitState.clockTick();

    programCounter.changeDriveSignal( 0 );
    programCounter.clockTick();
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
    //aluBMx.undefine();
    //PCMux.undefine();
    ifZeroMux.undefine();
    ifPositiveMux.undefine();
    //regWriteSelectMux.undefine();
    //regWriteDataMux.undefine();

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
    PCplus4.clockTick();
    resultArg.clockTick();
    immediate.clockTick();
    aluResult.clockTick();
    zero.clockTick();
    positive.clockTick();
    controlUnitState.clockTick();
    currentOpcode.clockTick();
    halted.clockTick();
    ram->clockTick();
    
    return halted.getOutput();   
}

int32_t CPU::debugRamRead( int32_t addr ) {
    return ram->debugRead( addr );
}
