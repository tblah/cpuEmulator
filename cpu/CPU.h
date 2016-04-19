// encapsulating class representing the whole CPU
// control unit logic is in here because it needs access to everything anyway
// the RAM is included as part of the CPU to save the need for another layer

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

#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <vector>

//#include "../emulator/Bus.h"
#include "../emulator/mux.h"
#include "alu.h"
#include "Decoder.h"
#include "../emulator/Register.h"
#include "../emulator/RegisterFile.h"
#include "muxControlEnums.h"
#include "../emulator/Signal.h"
#include "ControlUnitState.h"
//#include "ram.h"
#include "RamAddrTranslator.h"
#include "Opcodes.h"

class CPU {
    private:
        // big parts
        ALU alu;
        Decoder decoder;
        RegisterFile<int32_t, uint8_t, 32> registers; // user registers
        
        // not really part of a cpu but included here for simplicity
        RamAddrTran<int32_t, 10240>* ram; // 10240 bytes of ram

        // special purpose registers
        Register<int32_t> programCounter;
        Register<int32_t> PCplus4;
        Register<uint8_t> resultArg;
        Register<int32_t> immediate;
        Register<int32_t> aluResult;
        Register<bool> zero;
        Register<bool> positive;
//        Register<int32_t> ramRead;
        Register<bool> halted;
        Register<ControlUnitStateEnum> controlUnitState;
        Register<Opcode> currentOpcode;
        
        // interface with ram
        //Bus<int32_t> ramDataBus;

        // multiplexers
        //Mux<AluBMuxControl, int32_t> aluBMux;
        //Mux<PCMuxControl, int32_t> PCMux;
        Mux<bool, int32_t> ifZeroMux;
        Mux<bool, int32_t> ifPositiveMux;
        //Mux<RegWriteSelectMuxControl, uint8_t> regWriteSelectMux;
        //Mux<RegWriteDataMuxControl, int32_t> regWriteDataMux;
    
        // control unit combinational logic for each state
        void fetch( void );
        void decode( void );
        void execute( void );
        void write( void );

    public:
        CPU( const std::vector<int32_t> &InitialRamData );
        ~CPU( void );

        bool clockTick( void ); // returns wheather or not we are halted

        // this should only be used in automated testing to check the correct values 
        // made it to RAM
        int32_t debugRamRead( int32_t addr );
};

#endif
