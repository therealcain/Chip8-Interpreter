#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <string>
#include <vector>
#include <memory>
#include <array>

#define PRINT_DIRECTIVE(x) puts(#x);

// the C++ way of Defines
namespace Constants
{
    constexpr auto RESOLUTION_WIDTH  = 64;
    constexpr auto RESOLUTION_HEIGHT = 32;

    constexpr auto START_LOCATION     = 0x200;
    constexpr auto MAXIMUM_BYTES      = 0xFFF;
    constexpr auto INSTRUCTION_BYTES  = 2; 
    constexpr auto TIMER_HERTZ        = 60;

    // OPCODES - There are 16 different first four bits
    constexpr auto ZERO_OPCODE       = 0x0000;
    constexpr auto ONE_OPCODE        = 0x1000;
    constexpr auto TWO_OPCODE        = 0x2000;
    constexpr auto THREE_OPCODE      = 0x3000;
    constexpr auto FOUR_OPCODE       = 0x4000;
    constexpr auto FIVE_OPCODE       = 0x5000;
    constexpr auto SIX_OPCODE        = 0x6000;
    constexpr auto SEVEN_OPCODE      = 0x7000;
    constexpr auto EIGHT_OPCODE      = 0x8000;
    constexpr auto NINE_OPCODE       = 0x9000;
    constexpr auto TEN_OPCODE        = 0xA000;
    constexpr auto ELEVEN_OPCODE     = 0xB000;
    constexpr auto TWELVE_OPCODE     = 0xC000;
    constexpr auto THIRTEEN_OPCODE   = 0xD000;
    constexpr auto FOURTEEN_OPCODE   = 0xE000;
    constexpr auto FIFTEEN_OPCODE    = 0xF000;

    // OPCODES
    // zero opcodes:
    constexpr auto OPCODE_CLS     = 0x00E0; // Clear the display.
    constexpr auto OPCODE_RET     = 0x00EE; // Return from a subroutine.
}

class Chip8
{
private:
    [[maybe_unused]] std::string cast_hex(const std::vector<uint8_t>& vec) const noexcept;
    
    void loading_buffer_to_memory(const std::vector<uint8_t>& buffer) noexcept;
    void load_fontset() noexcept;

    void disassemble_loop() noexcept;
    void disassemble_pc() noexcept;

    void fetch_opcode() noexcept;
    
public:
    Chip8(const std::string& path);
    ~Chip8() = default;

private:
    std::vector<uint8_t> memory;

    // pseudo registers
    uint16_t program_counter = Constants::START_LOCATION;         // PC
    uint16_t stack_pointer;                                       // SP
    
    
    std::array<uint8_t, 16> registers; // VO
    std::array<uint16_t, 24> stack;

    uint16_t opcode;
    uint16_t index; // I

    // timers - rate 60hz
    uint16_t delay_timer = Constants::TIMER_HERTZ;
    uint16_t sound_timer = Constants::TIMER_HERTZ;
};

#endif
