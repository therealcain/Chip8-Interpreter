#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <string>
#include <vector>
#include <memory>

// #define PRINT_DIRECTIVE(x) puts(#x);

// the C++ way of Defines
namespace MemoryAddr
{
    constexpr auto START_LOCATION    = 0x200;
    constexpr auto MAXIMUM_BYTES     = 0xFFF;
    constexpr auto INSTRUCTION_BYTES = 2;
}

class Chip8
{
private:
    std::string cast_hex(const std::vector<uint8_t>& vec) const noexcept;
    
    void loading_buffer_to_memory(const std::vector<uint8_t>& buffer) noexcept;
    void load_fontset() noexcept;

    void disassemble_loop() noexcept;
    void disassemble_pc() noexcept;

    void fetch_opcodes() noexcept;
    
public:
    Chip8(const std::string& path);
    ~Chip8() = default;

private:
    std::vector<uint8_t> memory;

    // pseudo registers
    uint16_t program_counter = MemoryAddr::START_LOCATION; // pc
    uint16_t stack_pointer;                                // sp

    uint16_t opcode;
    uint16_t index;

    // timers - rate 60hz
    uint16_t delay_timer;
    uint16_t sound_timer;
};

#endif