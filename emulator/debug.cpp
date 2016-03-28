// utility functions relating to debug messages

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

#include "debug.h"
#include <stdlib.h>
#include <iostream>

// prints message and then exits: returning to indicate something bad hapened
void errExit( std::string message ) {
    std::cerr << "FATAL: " << message << std::endl;

    exit( EXIT_FAILURE );
}

// if debugging is enabled, print a message to stderr
void debug( std::string message ) {
    #ifdef DEBUG
        std::cerr << "DEBUG: " << message << std::endl;
    #else
        // do something with message so the compiler does not complain that it is unused
        message.length();
    #endif
}

