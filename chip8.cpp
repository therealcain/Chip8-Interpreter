#include "chip8.hpp"

#include <fstream>
#include <iostream>
#include <exception>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>

[[maybe_unused]] std::string Chip8::cast_hex(const std::vector<uint8_t>& vec) const noexcept
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    for(const auto ch : vec) 
    {
        ss << std::setw(2) << +ch << " ";
    }

    return ss.str();
}

void Chip8::loading_buffer_to_memory(const std::vector<uint8_t>& buffer) noexcept
{
    std::copy(buffer.begin(), buffer.end(), memory.begin() + C::START_LOCATION);
}

void Chip8::load_fontset() noexcept
{
    static constexpr std::initializer_list<uint8_t> fontset {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    // copy all of the fontsets to memory
    std::copy(fontset.begin(), fontset.end(), memory.begin());
}

void Chip8::disassemble_pc() noexcept
{
    // --- Chip8 Instructions
    // example: a2 bc - big endian
    // opcode = a2bc
    // a2 - high byte - 8 bits
    // bc - low byte  - 8 bits

    // A 12-bit value, the lowest 12 bits of the instruction
    const uint16_t nnn = opcode & 0xFFF;              // addr
    // A 4-bit value, the lowest 4 bits of the instruction
    const uint8_t  n   = opcode & 0xF;                // nibble
    // A 4-bit value, the lower 4 bits of the high byte of the instruction
    const uint8_t  x   = (opcode >> 8) & 0xF;         // x-axis
    // A 4-bit value, the upper 4 bits of the low byte of the instruction
    const uint8_t  y   = (opcode >> 4) & 0xF;         // y-axis
    // An 8-bit value, the lowest 8 bits of the instruction
    const uint8_t  kk  = opcode & 0xFF;               // byte

    // Mask the first digit
    switch (opcode & 0xF000)
    {
        case C::ZERO_OPCODE:
            switch (opcode & 0x00FF)
            {
                // 0NNN implementation not needed because
                // i'm not trying to emulate RCA 1802 CPU
                
                case C::OPCODE_00E0:
                PRINT_DIRECTIVE(C::OPCODE_00E0);
                break;

                case C::OPCODE_00EE:
                PRINT_DIRECTIVE(C::OPCODE_00EE);
                break;

                default:
                    std::cout << C::MEMORY_OPCODE_ERROR_MSG << std::endl;
                break;
            }
        break;

        case C::ONE_OPCODE:   // 1NNN
            PRINT_DIRECTIVE(C::ONE_OPCODE);
        break;

        case C::TWO_OPCODE:   // 2NNN
            PRINT_DIRECTIVE(C::TWO_OPCODE);
        break;

        case C::THREE_OPCODE: // 3XNN
            PRINT_DIRECTIVE(C::THREE_OPCODE);
        break;

        case C::FOUR_OPCODE:  // 4XNN
            PRINT_DIRECTIVE(C::FOUR_OPCODE);
        break;

        case C::FIVE_OPCODE:  // 5XY0
            PRINT_DIRECTIVE(C::FIVE_OPCODE);
        break;

        case C::SIX_OPCODE:   // 6XNN
            PRINT_DIRECTIVE(C::SIX_OPCODE);
        break;

        case C::SEVEN_OPCODE: // 7XNN
            PRINT_DIRECTIVE(C::SEVEN_OPCODE);
        break;

        case C::EIGHT_OPCODE:
            switch (opcode & 0x000F)
            {
                case C::OPCODE_8XY0:
                    PRINT_DIRECTIVE(C::OPCODE_8XY0);
                break;

                case C::OPCODE_8XY1:
                    PRINT_DIRECTIVE(C::OPCODE_8XY1);
                break;

                case C::OPCODE_8XY2:
                    PRINT_DIRECTIVE(C::OPCODE_8XY2);
                break;

                case C::OPCODE_8XY3:
                    PRINT_DIRECTIVE(C::OPCODE_8XY3);
                break;

                case C::OPCODE_8XY4:
                    PRINT_DIRECTIVE(C::OPCODE_8XY4);
                break;

                case C::OPCODE_8XY5:
                    PRINT_DIRECTIVE(C::OPCODE_8XY5);
                break;

                case C::OPCODE_8XY6:
                    PRINT_DIRECTIVE(C::OPCODE_8XY6);
                break;

                case C::OPCODE_8XY7:
                    PRINT_DIRECTIVE(C::OPCODE_8XY7);
                break;

                case C::OPCODE_8XYE:
                    PRINT_DIRECTIVE(C::OPCODE_8XYE);
                break;

                default:
                    std::cout << C::MEMORY_OPCODE_ERROR_MSG << std::endl;
                break;
            }
        break;

        case C::NINE_OPCODE:     // 9XY0
            PRINT_DIRECTIVE(C::NINE_OPCODE);
        break;

        case C::TEN_OPCODE:      // ANNN
            PRINT_DIRECTIVE(C::TEN_OPCODE);
        break;

        case C::ELEVEN_OPCODE:   // BNNN
            PRINT_DIRECTIVE(C::ELEVEN_OPCODE);
        break;

        case C::TWELVE_OPCODE:   // CXNN
            PRINT_DIRECTIVE(C::TWELVE_OPCODE);
        break;

        case C::THIRTEEN_OPCODE: // DXYN
            PRINT_DIRECTIVE(C::THIRTEEN_OPCODE);
        break;

        case C::FOURTEEN_OPCODE:
            switch (opcode & 0x000F)
            {
                case C::OPCODE_EX9E:
                PRINT_DIRECTIVE(C::OPCODE_EX9E);
                break;

                case C::OPCODE_EXA1:
                PRINT_DIRECTIVE(C::OPCODE_EXA1);
                break;

                default:
                std::cout << C::MEMORY_OPCODE_ERROR_MSG << std::endl;
                break;
            }
        break;

        case C::FIFTEEN_OPCODE:
            switch (opcode & 0x00FF)
            {
                case C::OPCODE_FX07:
                PRINT_DIRECTIVE(C::OPCODE_FX07);
                break;

                case C::OPCODE_FX0A:
                PRINT_DIRECTIVE(C::OPCODE_FX0A);
                break;

                case C::OPCODE_FX15:
                PRINT_DIRECTIVE(C::OPCODE_FX15);
                break;

                case C::OPCODE_FX18:
                PRINT_DIRECTIVE(C::OPCODE_FX18);
                break;

                case C::OPCODE_FX1E:
                PRINT_DIRECTIVE(C::OPCODE_FX1E);
                break;

                case C::OPCODE_FX29:
                PRINT_DIRECTIVE(C::OPCODE_FX29);
                break;

                case C::OPCODE_FX33:
                PRINT_DIRECTIVE(C::OPCODE_FX33);
                break;

                case C::OPCODE_FX55:
                PRINT_DIRECTIVE(C::OPCODE_FX55);
                break;

                case C::OPCODE_FX65:
                PRINT_DIRECTIVE(C::OPCODE_FX65);
                break;

                default:
                std::cout << C::MEMORY_OPCODE_ERROR_MSG << std::endl;
                break;
            }
        break;

        default:
        std::cout << "Memory OPCODE is wrong!" << std::endl;
        break;
    }
}

void Chip8::disassemble_loop() noexcept
{
    while(program_counter < memory.size())
    {
        fetch_opcode();
        // std::cout << "Memory:" << std::hex << +memory[program_counter] << " " << std::hex << +memory[program_counter + 1];
        // std::cout << " - Opcode:" << std::hex << +opcode << "       ";

        disassemble_pc();

        // jump in the memory by 2
        // because every instruction is 2 bytes long
        program_counter += C::INSTRUCTION_BYTES;
    }
}

void Chip8::fetch_opcode() noexcept
{
    // shifting to left the memory[pc] by 8 bits
    // and then assign the next memory by using bitwise OR operation
    // example: a2 bc turns to 0xA2BC
    opcode = memory[program_counter] << 8 | memory[program_counter + 1];
}

Chip8::Chip8(const std::string& path)
    : memory(C::MAXIMUM_BYTES)
{
    std::ifstream file(path, std::ios::in     | 
                             std::ios::ate    | 
                             std::ios::binary);
    if(file.is_open())
    {
        // get the size of file in bytes
        const std::streampos size = file.tellg();

        std::vector<uint8_t> buffer(size);

        // start reading in the beginning ( index 0 )
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char*>(buffer.data()), size);

        load_fontset();        
        loading_buffer_to_memory(buffer);
        buffer.clear();

        disassemble_loop();
    }
    else 
    {
        throw std::runtime_error("File could not be found!");
    }
}