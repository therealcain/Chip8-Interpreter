#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <array>
#include <functional>
#include <map>

#include "cpu.hpp"
#include "window.hpp"

constexpr auto MEMORY_SIZE   = 0xFFF;
constexpr auto STACK_SIZE    = 16;
constexpr auto REGISTER_SIZE = 16;
constexpr auto KEYPADS_SIZE  = 16;
constexpr auto CHIP8_WIDTH   = 64;
constexpr auto CHIP8_HEIGHT  = 32;

class Chip8 : public CPU<STACK_SIZE,
                         MEMORY_SIZE,
                         REGISTER_SIZE,
                         KEYPADS_SIZE,
                         CHIP8_WIDTH,
                         CHIP8_HEIGHT> 
{
public:
    Chip8(const std::string& file_path, Window& window);
    ~Chip8() = default;

    void cycle() noexcept;

private:
    #if ENABLE_DEBUG_MODE
    std::string get_memory_as_string(size_t min, size_t max) const noexcept;
    void clear_terminal() const noexcept;
    #endif // ENABLE_DEBUG_MODE

    void copy_fonts_to_memory() noexcept;
    void fetch_opcode() noexcept;
    void fetch_instruction_variables() noexcept;
    void call_opcodes() noexcept;
    uint8_t random_byte() const noexcept;

    // Opcode implemenations
    // void OPCODE_0NNN_Impl();
    void OPCODE_00E0_Impl();
    void OPCODE_00EE_Impl();
    void OPCODE_1NNN_Impl();
    void OPCODE_2NNN_Impl();
    void OPCODE_3XKK_Impl();
    void OPCODE_4XKK_Impl();
    void OPCODE_5XY0_Impl();
    void OPCODE_6XKK_Impl();
    void OPCODE_7XKK_Impl();
    void OPCODE_8XY0_Impl();
    void OPCODE_8XY1_Impl();
    void OPCODE_8XY2_Impl();
    void OPCODE_8XY3_Impl();
    void OPCODE_8XY4_Impl();
    void OPCODE_8XY5_Impl();
    void OPCODE_8XY6_Impl();
    void OPCODE_8XY7_Impl();
    void OPCODE_8XYE_Impl();
    void OPCODE_9XY0_Impl();
    void OPCODE_ANNN_Impl();
    void OPCODE_BNNN_Impl();
    void OPCODE_CXKK_Impl();
    void OPCODE_DXYN_Impl();
    void OPCODE_EX9E_Impl();
    void OPCODE_EXA1_Impl();
    void OPCODE_FX07_Impl();
    void OPCODE_FX0A_Impl();
    void OPCODE_FX15_Impl();
    void OPCODE_FX18_Impl();
    void OPCODE_FX1E_Impl();
    void OPCODE_FX29_Impl();
    void OPCODE_FX33_Impl();
    void OPCODE_FX55_Impl();
    void OPCODE_FX65_Impl();

private:
    // Rendering
    Window& window_ref;

    std::map<uint16_t              /*opcode*/, 
             std::function<void()> /*opcode impl*/> opcode_table;

    // Contains all of the values of the instruction variables 
    // of the current opcode
    struct Instruction_variables
    {
        uint16_t nnn;
        uint8_t n;
        uint8_t x;
        uint8_t y;
        uint8_t kk;
    }; Instruction_variables inst_var;

// Constants
private:
    static constexpr auto LOCATION_START   = 0x200;
    static constexpr auto INSTRUCTION_LONG = 2;
    
    // static constexpr auto OPCODE_0NNN = 0x0000; // Jump to a machine code routine at nnn
    static constexpr auto OPCODE_00E0 = 0x0000; // Clear the display
    static constexpr auto OPCODE_00EE = 0x000E; // Return from a subroutine
    static constexpr auto OPCODE_1NNN = 0x1000; // Jump to location nnn
    static constexpr auto OPCODE_2NNN = 0x2000; // Call subroutine at nnn
    static constexpr auto OPCODE_3XKK = 0x3000; // Skip next instruction if Vx = kk
    static constexpr auto OPCODE_4XKK = 0x4000; // Skip next instruction if Vx != kk
    static constexpr auto OPCODE_5XY0 = 0x5000; // Skip next instruction if Vx = Vy
    static constexpr auto OPCODE_6XKK = 0x6000; // Set Vx = kk
    static constexpr auto OPCODE_7XKK = 0x7000; // Set Vx = Vx + kk
    static constexpr auto OPCODE_8XY0 = 0x8000; // Set Vx = Vy
    static constexpr auto OPCODE_8XY1 = 0x8001; // Set Vx = Vx OR Vy
    static constexpr auto OPCODE_8XY2 = 0x8002; // Set Vx = Vx AND Vy
    static constexpr auto OPCODE_8XY3 = 0x8003; // Set Vx = Vx XOR Vy
    static constexpr auto OPCODE_8XY4 = 0x8004; // Set Vx = Vx + Vy, set VF = carry
    static constexpr auto OPCODE_8XY5 = 0x8005; // Set Vx = Vx - Vy, set VF = NOT borrow
    static constexpr auto OPCODE_8XY6 = 0x8006; // Set Vx = Vx SHR 1
    static constexpr auto OPCODE_8XY7 = 0x8007; // Set Vx = Vy - Vx, set VF = NOT borrow
    static constexpr auto OPCODE_8XYE = 0x800E; // Set Vx = Vx SHL 1
    static constexpr auto OPCODE_9XY0 = 0x9000; // Skip next instruction if Vx != Vy
    static constexpr auto OPCODE_ANNN = 0xA000; // Set I = nnn
    static constexpr auto OPCODE_BNNN = 0xB000; // Jump to location nnn + V0
    static constexpr auto OPCODE_CXKK = 0xC000; // Set Vx = random byte AND kk
    static constexpr auto OPCODE_DXYN = 0xD000; // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
    static constexpr auto OPCODE_EX9E = 0xE09E; // Skip next instruction if key with the value of Vx is pressed
    static constexpr auto OPCODE_EXA1 = 0xE0A1; // Skip next instruction if key with the value of Vx is not pressed
    static constexpr auto OPCODE_FX07 = 0xF007; // Set Vx = delay timer value
    static constexpr auto OPCODE_FX0A = 0xF00A; // Wait for a key press, store the value of the key in Vx
    static constexpr auto OPCODE_FX15 = 0xF015; // Set delay timer = Vx
    static constexpr auto OPCODE_FX18 = 0xF018; // Set sound timer = Vx
    static constexpr auto OPCODE_FX1E = 0xF01E; // Set I = I + Vx
    static constexpr auto OPCODE_FX29 = 0xF029; // Set I = location of sprite for digit Vx
    static constexpr auto OPCODE_FX33 = 0xF033; // Store BCD representation of Vx in memory locations I, I+1, and I+2
    static constexpr auto OPCODE_FX55 = 0xF055; // Store registers V0 through Vx in memory starting at location I
    static constexpr auto OPCODE_FX65 = 0xF065; // Read registers V0 through Vx from memory starting at location I
};

#endif // CHIP8_HPP