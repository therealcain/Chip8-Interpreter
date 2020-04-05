#ifndef CPU_HPP
#define CPU_HPP

#include <array>
#include <cstdint>

template<size_t STACK, 
         size_t MEMORY, 
         size_t REGISTER, 
         size_t KEYS, 
         size_t WIDTH, 
         size_t HEIGHT>
struct CPU
{
    std::array<uint32_t, WIDTH * HEIGHT>   display   {};
    std::array<uint16_t, STACK>            stack     {};
    std::array<uint8_t,  MEMORY>           memory    {};
    std::array<uint8_t,  REGISTER>         registers {};
    std::array<uint8_t,  KEYS>             keypads   {};

    uint16_t pc;     // used to store the currently executing address
    uint16_t I;      // this register is generally used to store memory addresses
    uint16_t opcode; // the current operation code
    uint8_t  sp;     // used to point to the topmost level of the stack

    // these are automatically decremented at a rate of 60Hz.
    uint8_t  dt;     // delay of the program
    // uint8_t  st;   // delay of the sounds
};

#endif // CPU_HPP