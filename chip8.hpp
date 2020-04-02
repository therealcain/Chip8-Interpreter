#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <string>
#include <vector>
#include <memory>
#include <array>

#define PRINT_DIRECTIVE(x) puts(#x);

// the C++ way of defines
namespace C // shortcut of Constants
{
    constexpr auto RESOLUTION_WIDTH  = 64;
    constexpr auto RESOLUTION_HEIGHT = 32;

    constexpr auto START_LOCATION     = 0x200;
    constexpr auto MAXIMUM_BYTES      = 0xFFF;
    constexpr auto INSTRUCTION_BYTES  = 2; 
    constexpr auto TIMER_HERTZ        = 60;

    // OPCODES - There are 16 different first four bits
    constexpr auto /*      */ ZERO_OPCODE       = 0x0000;
    constexpr auto /* 1NNN */ ONE_OPCODE        = 0x1000; // Jumps to address NNN. 
    constexpr auto /* 2NNN */ TWO_OPCODE        = 0x2000; // Calls subroutine at NNN. 
    constexpr auto /* 3XNN */ THREE_OPCODE      = 0x3000; // Skips the next instruction if VX equals NN.
    constexpr auto /* 4XNN */ FOUR_OPCODE       = 0x4000; // Skips the next instruction if VX doesn't equal NN.
    constexpr auto /* 5XY0 */ FIVE_OPCODE       = 0x5000; // Skips the next instruction if VX equals VY. 
    constexpr auto /* 6XNN */ SIX_OPCODE        = 0x6000; // Sets VX to NN. 
    constexpr auto /* 7XNN */ SEVEN_OPCODE      = 0x7000; // Adds NN to VX.
    constexpr auto /*      */ EIGHT_OPCODE      = 0x8000;
    constexpr auto /* 9XY0 */ NINE_OPCODE       = 0x9000; // Skips the next instruction if VX doesn't equal VY. 
    constexpr auto /* ANNN */ TEN_OPCODE        = 0xA000; // Sets I to the address NNN. 
    constexpr auto /* BNNN */ ELEVEN_OPCODE     = 0xB000; // Jumps to the address NNN plus V0. 
    constexpr auto /* CXNN */ TWELVE_OPCODE     = 0xC000; // Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN. 
    constexpr auto /* DXYN */ THIRTEEN_OPCODE   = 0xD000; // Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. 
                                                          // Each row of 8 pixels is read as bit-coded starting from memory location I; 
                                                          // I value doesn’t change after the execution of this instruction. 
                                                          // As described above, VF is set to 1 if any screen pixels are flipped from set to unset when 
                                                          // the sprite is drawn, and to 0 if that doesn’t happen
    constexpr auto /*      */ FOURTEEN_OPCODE   = 0xE000;
    constexpr auto /*      */ FIFTEEN_OPCODE    = 0xF000;

    // ZERO OPCODES:
    constexpr auto OPCODE_00E0 = 0x0000; // Clear the display.
    constexpr auto OPCODE_00EE = 0x000E; // Return from a subroutine.

    // EIGHT OPCODES:
    constexpr auto OPCODE_8XY0 = 0x0000; // Sets VX to the value of VY. 
    constexpr auto OPCODE_8XY1 = 0x0001; // Sets VX to VX or VY. (Bitwise OR operation) 
    constexpr auto OPCODE_8XY2 = 0x0002; // Sets VX to VX and VY. (Bitwise AND operation) 
    constexpr auto OPCODE_8XY3 = 0x0003; // Sets VX to VX xor VY. 
    constexpr auto OPCODE_8XY4 = 0x0004; // Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't. 
    constexpr auto OPCODE_8XY5 = 0x0005; // VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't. 
    constexpr auto OPCODE_8XY6 = 0x0006; // Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
    constexpr auto OPCODE_8XY7 = 0x0007; // Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't. 
    constexpr auto OPCODE_8XYE = 0x000E; // Stores the most significant bit of VX in VF and then shifts VX to the left by 1.

    // FOURTEEN OPCODES:
    constexpr auto OPCODE_EX9E = 0x000E; // Skips the next instruction if the key stored in VX is pressed.
    constexpr auto OPCODE_EXA1 = 0x0001; // Skips the next instruction if the key stored in VX isn't pressed.

    // FIFTEEN OPCODES:
    constexpr auto OPCODE_FX07 = 0x0007; // Sets VX to the value of the delay timer. 
    constexpr auto OPCODE_FX0A = 0x000A; // A key press is awaited, and then stored in VX. 
    constexpr auto OPCODE_FX15 = 0x0015; // Sets the delay timer to VX. 
    constexpr auto OPCODE_FX18 = 0x0018; // Sets the sound timer to VX. 
    constexpr auto OPCODE_FX1E = 0x001E; // Adds VX to I. VF is set to 1 when there is a range overflow (I+VX>0xFFF), and to 0 when there isn't.
    constexpr auto OPCODE_FX29 = 0x0029; // Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) 
                                         // are represented by a 4x5 font. 
    constexpr auto OPCODE_FX33 = 0x0033; // Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I,
                                         // the middle digit at I plus 1, and the least significant digit at I plus 2. 
    constexpr auto OPCODE_FX55 = 0x0055; // Stores V0 to VX (including VX) in memory starting at address I. 
                                         // The offset from I is increased by 1 for each value written, but I itself is left unmodified.
    constexpr auto OPCODE_FX65 = 0x0065; // Fills V0 to VX (including VX) with values from memory starting at address I. 
                                         // The offset from I is increased by 1 for each value written, but I itself is left unmodified.

    constexpr char const* MEMORY_OPCODE_ERROR_MSG = "Memory OPCODE is wrong!";
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
    uint16_t program_counter = C::START_LOCATION; // PC
    uint16_t* stack_pointer  = nullptr;           // SP
    
    std::array<uint8_t, 16> registers; // VO
    std::array<uint16_t, 24> stack;

    uint16_t opcode;
    uint16_t index; // I

    // timers - rate 60hz
    uint16_t delay_timer = C::TIMER_HERTZ;
    uint16_t sound_timer = C::TIMER_HERTZ;
};

#endif
