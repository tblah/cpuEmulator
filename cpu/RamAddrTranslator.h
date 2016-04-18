// ram address translator

/* The main memory address space is split into the actual RAM and a video frame buffer. 
 * The frame buffer is just ASCI: 64x64 characters. Therefore the video memory takes the top 4096 addresses
 * All other addresses belong to the main memory.
 * This module encapsulates the main memory and the video memory. And provides the printing to the stdout.
 * Most of this is just passing things through to the correct RAM object (main memory or video memory)
 *
 * IO
 *    Inputs:
 *       -Address
 *       -Reading (yes/no)
 *       -clock
 *       -print to console
 *
 *       Input/Output
 *       -data
 */

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

#ifndef RAM_ADDR_TRANS
#define RAM_ADDR_TRANS

#include "ram.h" // this also includes most of the other headers we will need
#include <iostream>

// the types are assumed to be numeric or atleast have those sorts of operators working
// generally just keep the types as integers. In the context of the cpu, nothing else really makes sense
// TODO: template magic to force the types to be integral
template <typename AddressType, unsigned int numBytes> class RamAddrTran {
    private:
        RAM<AddressType, numBytes-4096> *mainMemory;
        RAM<AddressType, 4096> *videoMemory;
        Signal<bool> videoMemorySelected; // we are using the video memory this cycle
        Signal<bool> oldVideoMemorySelected; // from the previous clock cycle so getData knows which one to return from
                                             // this would not need to exit in a hardware implementation because the correct
                                             // ram object would be writing to the memory data bus and the other would be 
                                             // resenting a high impedance output. 

        // combinational logic translating an address to whatever it should be
        // the target memory is returned by refference becasue we need to return two arguments
        AddressType translateAddress( AddressType input, bool& videoAddr ) {
            if ( input > (numBytes-1) ) {
                errExit( "Invalid memory address given to ramAddrTran" );
            } else if ( input > (numBytes-4097) ) { // top 4096 addresses
                videoAddr = true;
                return input - (numBytes - 4096); // pretty substantial combinational logic is needed to make this happen
                                     // if the cpu were being optimised to have a smaller footprint
                                     // then a system requiring less logic such as checking the most
                                     // significant bit might be more appropiate. This was just chosen to 
                                     // minimise the loss of main memory address space
            } 

            // else lower addresses
            videoAddr = false;
            return input;
        }

    public:
        // constructor with initial data for the main memory
        RamAddrTran( const std::vector<int32_t> &InitialData ) {
            if (numBytes < 4097)
                errExit( "Your RAM can't fit the fixed-size frame buffer" );

            mainMemory = new RAM<AddressType, numBytes-4096>( InitialData );

            std::vector<int32_t> videoInitial;

            for ( unsigned int i = 0; i <= 4096-sizeof(int32_t); i+= sizeof(int32_t) )
                videoInitial.push_back( 0x23232323 ); // '#' = 0x23

            videoMemory = new RAM<AddressType, 4096>( videoInitial );
        }

        // destructor to unallocate the RAM objects
        ~RamAddrTran( void ) {
            delete mainMemory;
            delete videoMemory;
        }

        // pass through to the appropiate RAM object
        int32_t debugRead( AddressType addr ) {
            bool videoAddr; // are we talking about the video memory or not?
            int32_t ret; // thing we are returning

            AddressType fixedAddr = translateAddress( addr, videoAddr );

            if ( videoAddr ) {
                ret = videoMemory->debugRead( fixedAddr );
            } else {
                ret = mainMemory->debugRead( fixedAddr );
            }

            return ret;
        }

        // send the video buffer to stdout
        void printBuffer( void ) {
            // we don't know for sure that stdout is tied to a console and there are not any cross platform mechanisms that I know of
            // in the standard library to clear the console so just print some newlines.
            //for ( unsigned int i = 0; i < 100; i++ )
                //std::cout << std::endl;

            for ( unsigned int y = 0; y < 64; y++ ) {
                for ( unsigned int x = 0; x < 60; x+= 4 ) {
                    // we are going to use debug reads so that it happens instantly. 
                    // In an actual computer, actually sending the data to the console output or video adapter etc
                    // For example, a PCIE video card might have dma access to the video buffer and after receiving this signal
                    // could print the buffer in it's own time without causing the main processor to stall and wait
                    // (unless it next wants to write to the video buffer)
                    // all in all this is just to keep this simple. It is only a few % of a 2nd year module after all 
                    int32_t fourChars = videoMemory->debugRead( 64*y + x );

                    for ( unsigned int i = 0; i < 4; i++ )
                        std::cout << ((char*) &fourChars)[i];
                }

                std::cout << std::endl;
              }  
        }

        // passthrough to appropiate ram object
        void setAddress( AddressType address ) {
            bool videoAddr;

            AddressType addr = translateAddress( address, videoAddr );
            videoMemorySelected.setValue( videoAddr );

            if ( videoAddr ) {
                debugSignal( "video memory address bus", addr );
                videoMemory->setAddress( addr );
            } else
                debugSignal( "main memory address bus", addr );
                mainMemory->setAddress( addr );
        }

        // passthrough
        void setReadingThisCycle( bool rwControl ) {
            if ( videoMemorySelected.isUndefined() )
                errExit( "videoMemorySelected in RamAddrTran is undefined. You should specify the address first" );

            if ( videoMemorySelected.getValue() ) { 
                videoMemory->setReadingThisCycle( rwControl );
                debugSignal("video memory -> reading this cycle", rwControl);
            } else {
                mainMemory->setReadingThisCycle( rwControl );
                debugSignal("main memory -> reading this cycle", rwControl);
            }
        }

        // passthrough
        void setDataIn( int32_t inData ) {
            if ( videoMemorySelected.isUndefined() )
                errExit( "videoMemorySelected in RamAddrTran is undefined. You should specify the address first" );

            if ( videoMemorySelected.getValue() )
                videoMemory->setDataIn( inData );
            else
                mainMemory->setDataIn( inData );
        }

        // passthrough
        int32_t getOutput( void ) {
            if ( oldVideoMemorySelected.getValue() ) {
                debugSignal( "video memory read", (int) videoMemory->getOutput() );
                return videoMemory->getOutput();
            }
            
            debugSignal( "main memory read", mainMemory->getOutput() );
            return mainMemory->getOutput();
        }

        // passthrough
        void clockTick( void ) {
            if ( videoMemorySelected.isDefined() )
                oldVideoMemorySelected.setValue( videoMemorySelected.getValue() );
            else
                oldVideoMemorySelected.undefine();

            videoMemorySelected.undefine();

            videoMemory->clockTick();
            mainMemory->clockTick();
        }
};

#endif
