// *very* minimal assembler for the cpu

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

#include <stdlib.h>
#include <iostream>
#include "commandLineArgs.h"

#include "Instruction.h"
#include "../cpu/Opcodes.h"
#include <stdio.h>

using namespace std;

inline void printFreeSoftwareMessage( void ) {
    cout << "This is free software; see the source for licence conditions.  There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << endl << endl;
}

// deleteme
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = b[i] & (1<<j);
            byte >>= j;
            printf("%u", byte);
        }
    }
    puts("");
}

int main( int argc, char** argv ) {
    string inputFile;
    string outputFile;

    printFreeSoftwareMessage();

    processCommandLineArgs( argc, argv, inputFile, outputFile );

    // test Instruction
    Instruction add( Opcode::add, 0, 1, 2 );
    uint32_t addObj = add.getObjectCode();
    printBits( sizeof(uint32_t), (void const * const) &addObj );

    Instruction addI( Opcode::addImmediate, 1, 1);
    uint32_t addIObj = addI.getObjectCode();
    printBits( sizeof(uint32_t), (void const * const) &addIObj );
    
    Instruction addI2( Opcode::addImmediate, 1, -1);
    uint32_t addI2Obj = addI2.getObjectCode();
    printBits( sizeof(uint32_t), (void const * const) &addI2Obj );

    return EXIT_SUCCESS;
}
