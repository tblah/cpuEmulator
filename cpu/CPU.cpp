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
    debugSignal( "cpu state", "fetch" );
    // read the next instruction from the RAM into the instruction register
    ram->setAddress( programCounter.getOutput() );
    ram->setReadingThisCycle( true );
    
    controlUnitState.changeDriveSignal( ControlUnitStateEnum::Decode );   

    // reset registers which shouldn't be remembering anything from the previous cycle
    PCplus4.reset();
    resultArg.reset();
    immediate.reset();
    aluResult.reset();
    currentOpcode.reset();
}

inline void CPU::decode( void ) {
    debugSignal( "cpu state", "decode" );
    // decode the instruction we just read from RAM and read those registers
    decoder.setMemoryWord( ram->getOutput() );
    currentOpcode.changeDriveSignal( decoder.getOpcode() );

    controlUnitState.changeDriveSignal( ControlUnitStateEnum::Execute );

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
        case ( Opcode::printBuffer):
            // we can't skip back to fetch yet because PC has not got it's new value
            break;

        default:
            errExit( "In cpu/decode, invalid opcode" );
    }

    // calculate what the next value of the program counter probably will be
    alu.setControl( AluOps::add );
    alu.setA( programCounter.getOutput() );
    alu.setB( sizeof(int32_t) );
    PCplus4.changeDriveSignal( alu.getResult() );
}

inline void CPU::execute( void ) {
    // default
    controlUnitState.changeDriveSignal( ControlUnitStateEnum::Write );

    // actually execure the instructions
    // to keep the code simple we are not using aluBMux explicitly
    switch ( currentOpcode.getOutput() ) {
        case ( Opcode::add ):
            debugSignal( "cpu state", "add execute" );
            alu.setA( registers.getOut1() );
            alu.setB( registers.getOut2() );
            alu.setControl( AluOps::add );

            aluResult.changeDriveSignal( alu.getResult() );
            zero.changeDriveSignal( alu.getZeroFlag() );
            positive.changeDriveSignal( alu.getPositiveFlag() );

            programCounter.changeDriveSignal( PCplus4.getOutput() );
            break;

        case ( Opcode::sub ):
            debugSignal( "cpu state", "sub execute" );
            alu.setA( registers.getOut1() );
            alu.setB( registers.getOut2() );
            alu.setControl( AluOps::sub );

            aluResult.changeDriveSignal( alu.getResult() );
            zero.changeDriveSignal( alu.getZeroFlag() );
            positive.changeDriveSignal( alu.getPositiveFlag() );
 
            programCounter.changeDriveSignal( PCplus4.getOutput() );
            break;

        case ( Opcode::nand ):
            debugSignal( "cpu state", "nand execute" );
            alu.setA( registers.getOut1() );
            alu.setB( registers.getOut2() );
            alu.setControl( AluOps::nand );

            aluResult.changeDriveSignal( alu.getResult() );
            zero.changeDriveSignal( alu.getZeroFlag() );
            positive.changeDriveSignal( alu.getPositiveFlag() );
 
            programCounter.changeDriveSignal( PCplus4.getOutput() );
            break;

        case ( Opcode::lshift ):
            debugSignal( "cpu state", "lshift execute" );
            alu.setA( registers.getOut1() );
            alu.setB( registers.getOut2() );
            alu.setControl( AluOps::lshift );
 
            aluResult.changeDriveSignal( alu.getResult() );
            zero.changeDriveSignal( alu.getZeroFlag() );
            positive.changeDriveSignal( alu.getPositiveFlag() );
 
            programCounter.changeDriveSignal( PCplus4.getOutput() );
            break;

        case ( Opcode::addImmediate ):
            debugSignal( "cpu state", "addImmediate execute" );
            alu.setA( registers.getOut1() );
            alu.setB( immediate.getOutput() );
            alu.setControl( AluOps::add );
 
            aluResult.changeDriveSignal( alu.getResult() );
            zero.changeDriveSignal( alu.getZeroFlag() );
            positive.changeDriveSignal( alu.getPositiveFlag() );
 
            programCounter.changeDriveSignal( PCplus4.getOutput() );
            break;

        case ( Opcode::subImmediate ):
            debugSignal( "cpu state", "subImmediate execute" );
            alu.setA( registers.getOut1() );
            alu.setB( immediate.getOutput() );
            alu.setControl( AluOps::sub );
 
            aluResult.changeDriveSignal( alu.getResult() );
            zero.changeDriveSignal( alu.getZeroFlag() );
            positive.changeDriveSignal( alu.getPositiveFlag() );
 
            programCounter.changeDriveSignal( PCplus4.getOutput() );
            break;

        case ( Opcode::jumpToReg ):
            debugSignal( "cpu state", "Jump to Register execute" );
            programCounter.changeDriveSignal( registers.getOut1() );
            
            // no need to write anything. Skip to next instruction
            controlUnitState.changeDriveSignal( ControlUnitStateEnum::Fetch );
            break;

        case ( Opcode::branchIfZero ):
            debugSignal( "cpu state", "Branch if zero execute" );
            // using ifZeroMux as an if statement
            // this is using the value of zero from whenever the ALU last did
            //          something in the execute stage
            ifZeroMux.setInput( true, registers.getOut1() );
            ifZeroMux.setInput( false, PCplus4.getOutput() );
            ifZeroMux.setSelect( zero.getOutput() );

            programCounter.changeDriveSignal( ifZeroMux.getOutput() );
             
            // no need to write anything. Skip to next instruction
            controlUnitState.changeDriveSignal( ControlUnitStateEnum::Fetch );
            break;

        case ( Opcode::branchIfPositive ):
            debugSignal( "cpu state", "Branch if positive execute" );
            // see notes for branchIfZero
            ifPositiveMux.setInput( true, registers.getOut1() );
            ifPositiveMux.setInput( false, PCplus4.getOutput() );
            ifPositiveMux.setSelect( positive.getOutput() );
            programCounter.changeDriveSignal( ifPositiveMux.getOutput() );
             
            // no need to write anything. Skip to next instruction
            controlUnitState.changeDriveSignal( ControlUnitStateEnum::Fetch );
            break;

        case ( Opcode::load ):
            debugSignal( "cpu state", "load execute" );
            ram->setAddress( registers.getOut1() );
            ram->setReadingThisCycle( true );
 
            programCounter.changeDriveSignal( PCplus4.getOutput() );
            break;

        case ( Opcode::store ):
            debugSignal( "cpu state", "store execute" );
            ram->setAddress( registers.getOut1() );
            ram->setReadingThisCycle( false ); // write
            ram->setDataIn( registers.getOut2() );
 
            programCounter.changeDriveSignal( PCplus4.getOutput() );
             
            // no need to write anything. Skip to next instruction
            controlUnitState.changeDriveSignal( ControlUnitStateEnum::Fetch );
            break;

        case ( Opcode::nop ):
            // nothing needs doing
            debugSignal( "cpu state", "nop execute" );
            programCounter.changeDriveSignal( PCplus4.getOutput() );
             
            // no need to write anything. Skip to next instruction
            controlUnitState.changeDriveSignal( ControlUnitStateEnum::Fetch );
            break;

        case ( Opcode::printBuffer):
            debugSignal( "cpu state", "serial write" );
            ram->printBuffer();
            programCounter.changeDriveSignal( PCplus4.getOutput() );

            // skip writeback
            controlUnitState.changeDriveSignal( ControlUnitStateEnum::Fetch );
            break;

        case ( Opcode::halt ):
            debugSignal( "cpu state", "halt execute" );
            halted.changeDriveSignal( true );
            programCounter.reset(); // errExit if we don't actually halt
            break;

        default:
            errExit( "In cpu/execute, invalid opcode" );
    }
}

inline void CPU::write( void ) {
    debugSignal( "cpu state", "write" );
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
            errExit( "we should have skipped write for that opcode" );
            break;

        default:
            errExit( "cpu/write invalid opcode" );
    }

    controlUnitState.changeDriveSignal( ControlUnitStateEnum::Fetch );
}

CPU::CPU( const std::vector<int32_t> &InitialRamData ) {
    ram = new RamAddrTran<int32_t, 10240> ( InitialRamData );

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
