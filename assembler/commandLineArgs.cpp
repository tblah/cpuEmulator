// process command line arguements

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

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>

using namespace std;

void printHelp( char* name ) {
    cout << "Usage: " << name << " [options] input_file" << endl;
    cout << "Options:" << endl;
    cout << "-o file \t\t Specify output file name. By default this is input_file.out" << endl;
    cout << "--help \t\t\t Display this notice" << endl;
}

void processCommandLineArgs( int argc, char** argv, string &inputFile, string &outputFile) {
    if ( (argc != 2) && (argc != 4) ) {
        printHelp( argv[0] );
        exit( EXIT_FAILURE );
    }

    if ( strncmp( argv[1], "--help", 6 ) == 0 ) {
        printHelp( argv[0] );
        exit( EXIT_SUCCESS );
    }

    if ( argc == 2 ) { // only an input file is specified
        inputFile.assign( argv[1] );
        outputFile.assign( inputFile );
        outputFile += ".out";
        cout << "No output file specified. Writing to " << outputFile << endl;
    }

    if ( argc == 4 ) { // output file specified as well
        if ( strncmp( argv[1], "-o", 2 ) != 0 ) {
            printHelp( argv[0] );
            exit( EXIT_FAILURE );
        }

        inputFile.assign( argv[3] );
        outputFile.assign( argv[2] );
    }
}
