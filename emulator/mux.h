// n-way multiplexer with arbitary types
// this is pretty much just a wrapper for std::unordered_map and Signal

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

#ifndef MUX_H
#define MUX_H

#include <unordered_map>
#include "Signal.h"
#include <stdexcept>
#include "debug.h"
#include <type_traits>

// by default, gcc does not know how to make a hash table of enum classes.
// this tells it how
struct EnumClassHash {
    template <typename Type>
    unsigned int operator()(Type EnumClass) const {
        return static_cast<unsigned int>(EnumClass);
    }
};

template <typename KeyType, typename DataType > class Mux {
    private:
        /* the third template parameter to this is the hashing function to use
            when building the hash table. std::hash does not know how to deal
            with enums and so we need to specify EnumClassHash *only* when KeyType
            is an enum
        */
 
        std::unordered_map<KeyType, DataType, typename
            std::conditional< std::is_enum<KeyType>::value, EnumClassHash,
            std::hash<KeyType> >::type> Data;

        Signal<KeyType> Select;

    public:
        void setInput( KeyType InputSelect, DataType inData ) {
               Data[InputSelect] = inData;
        }

        void setSelect( KeyType InputSelect ) {
            Select.setValue( InputSelect );
        }

        DataType getOutput( void ) {
            DataType Output;

            try {
                Output = Data.at( Select.getValue() ); // checks done in Signal
            } catch (const std::out_of_range& err) {
                errExit( "Mux selecting an unrecognised input" );
            }

            // gcc will complain that Output might be uninitialised here. This is because it does not know that errExit can never return. Ignore this error.
            return Output; 
        }

        void undefine( void ) {
            Select.undefine();
            Data.clear();
        }
};
    
#endif
