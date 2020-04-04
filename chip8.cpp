#include "chip8.hpp"

#include <fstream>
#include <exception>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <initializer_list>

Chip8::Chip8(const std::string& file_path, Window& window)
    : window_ref(window)
{
    std::ifstream file(file_path, std::ifstream::ate    | // start from the end
                                  std::ifstream::binary | // file is binary
                                  std::ifstream::in);     // planning on only input

    if(file.is_open())
    {
        // get file size in bytes
        const std::streampos size = file.tellg();

        // start reading in the beginning
        file.seekg(0, std::ios::beg);

        // File read expecting char*
        file.read(reinterpret_cast<char*>(memory.data() + LOCATION_START), size);
   
        copy_fonts_to_memory();

        window_ref.texture = SDL_CreateTexture(window_ref.renderer, SDL_PIXELFORMAT_ARGB8888, 
                                               SDL_TEXTUREACCESS_STREAMING, CHIP8_WIDTH, CHIP8_HEIGHT);

        // Starting position of CPU
        pc = LOCATION_START;

        // setting up the opcode table
        opcode_table[OPCODE_0NNN] = [this](){ OPCODE_0NNN_Impl(); };
        opcode_table[OPCODE_00E0] = [this](){ OPCODE_00E0_Impl(); };
        opcode_table[OPCODE_00EE] = [this](){ OPCODE_00EE_Impl(); };
        opcode_table[OPCODE_1NNN] = [this](){ OPCODE_1NNN_Impl(); };
        opcode_table[OPCODE_2NNN] = [this](){ OPCODE_2NNN_Impl(); };
        opcode_table[OPCODE_3XKK] = [this](){ OPCODE_3XKK_Impl(); };
        opcode_table[OPCODE_4XKK] = [this](){ OPCODE_4XKK_Impl(); };
        opcode_table[OPCODE_5XY0] = [this](){ OPCODE_5XY0_Impl(); };
        opcode_table[OPCODE_6XKK] = [this](){ OPCODE_6XKK_Impl(); };
        opcode_table[OPCODE_7XKK] = [this](){ OPCODE_7XKK_Impl(); };
        opcode_table[OPCODE_8XY0] = [this](){ OPCODE_8XY0_Impl(); };
        opcode_table[OPCODE_8XY1] = [this](){ OPCODE_8XY1_Impl(); };
        opcode_table[OPCODE_8XY2] = [this](){ OPCODE_8XY2_Impl(); };
        opcode_table[OPCODE_8XY3] = [this](){ OPCODE_8XY3_Impl(); };
        opcode_table[OPCODE_8XY4] = [this](){ OPCODE_8XY4_Impl(); };
        opcode_table[OPCODE_8XY5] = [this](){ OPCODE_8XY5_Impl(); };
        opcode_table[OPCODE_8XY6] = [this](){ OPCODE_8XY6_Impl(); };
        opcode_table[OPCODE_8XY7] = [this](){ OPCODE_8XY7_Impl(); };
        opcode_table[OPCODE_8XYE] = [this](){ OPCODE_8XYE_Impl(); };
        opcode_table[OPCODE_9XY0] = [this](){ OPCODE_9XY0_Impl(); };
        opcode_table[OPCODE_ANNN] = [this](){ OPCODE_ANNN_Impl(); };
        opcode_table[OPCODE_BNNN] = [this](){ OPCODE_BNNN_Impl(); };
        opcode_table[OPCODE_CXKK] = [this](){ OPCODE_CXKK_Impl(); };
        opcode_table[OPCODE_DXYN] = [this](){ OPCODE_DXYN_Impl(); };
        opcode_table[OPCODE_EX9E] = [this](){ OPCODE_EX9E_Impl(); };
        opcode_table[OPCODE_EXA1] = [this](){ OPCODE_EXA1_Impl(); };
        opcode_table[OPCODE_FX07] = [this](){ OPCODE_FX07_Impl(); };
        opcode_table[OPCODE_FX0A] = [this](){ OPCODE_FX0A_Impl(); };
        opcode_table[OPCODE_FX15] = [this](){ OPCODE_FX15_Impl(); };
        opcode_table[OPCODE_FX18] = [this](){ OPCODE_FX18_Impl(); };
        opcode_table[OPCODE_FX1E] = [this](){ OPCODE_FX1E_Impl(); };
        opcode_table[OPCODE_FX29] = [this](){ OPCODE_FX29_Impl(); };
        opcode_table[OPCODE_FX33] = [this](){ OPCODE_FX33_Impl(); };
        opcode_table[OPCODE_FX55] = [this](){ OPCODE_FX55_Impl(); };
        opcode_table[OPCODE_FX65] = [this](){ OPCODE_FX65_Impl(); };
    }
    else
    {
        throw std::invalid_argument("Path is invalid!");
    }
}

void Chip8::copy_fonts_to_memory() noexcept
{
    static constexpr std::initializer_list<int> fontset {
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

    // copy all fontset to memory
    std::copy(fontset.begin(), fontset.end(), memory.begin());
}

#if ENABLE_DEBUG_MODE
std::string Chip8::get_memory_as_string(size_t min, size_t max) const noexcept
{
    std::stringstream ss;

    // telling the stream that the characters are going to be hexadecimals
    // and if there is no character put 0 instead
    ss << std::hex << std::setfill('0');

    for(size_t i = 0; i < memory.size(); i++)
    {
        // make sure i is between min and max
        if(i >= min && i <= max)
        {
            // pushing the character into the stream and make sure it's 2 letter;
            ss << " " << std::setw(2) << +memory[i] << " " << opcode << "  ";
        }
    }

    return ss.str();
}

void Chip8::clear_terminal() const noexcept
{
    // UGHHH std::system is being used
    #if __linux__
    std::system("clear");
    #elif _WIN32
    std::system("cls");
    #endif
}
#endif // ENABLE_DEBUG_MODE

void Chip8::fetch_opcode() noexcept
{
    // shifting to left the memory[pc] by 8 bits
    // and then assign the next memory by using bitwise OR operation
    // example: a2 bc turns to 0xA2BC
    opcode = memory[pc] << 8 | memory[pc + 1];
}

void Chip8::fetch_instruction_variables() noexcept
{
    // A 12-bit value, the lowest 12 bits of the instruction
    inst_var.nnn = opcode & 0xFFF;      // addr
    // A 4-bit value, the lowest 4 bits of the instruction
    inst_var.n   = opcode & 0xF;        // nibble
    // A 4-bit value, the lower 4 bits of the high byte of the instruction
    inst_var.x   = (opcode >> 8) & 0xF; // x-axis
    // A 4-bit value, the upper 4 bits of the low byte of the instruction
    inst_var.y   = (opcode >> 4) & 0xF; // y-axis
    // An 8-bit value, the lowest 8 bits of the instruction
    inst_var.kk  = opcode & 0xFF;       // byte
}

// generating a random byte
uint8_t Chip8::random_byte() const noexcept
{
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(0, 255);
    return distr(eng);
}

void Chip8::cycle() noexcept
{
    // priting out all of the memory
    #if ENABLE_DEBUG_MODE
    clear_terminal();
    std::cout << std::hex << get_memory_as_string(LOCATION_START, 1000) << std::endl;
    #endif // ENABLE_DEBUG_MODE
    
    fetch_opcode();

    // Mask to remove the least important bit in the first nibble
    const auto masked_opcode =  opcode & 0xF0FF;
    if(opcode_table.count(masked_opcode) >= 1)
    {
        fetch_instruction_variables();
        opcode_table[masked_opcode]();
    }

    // Display graphics
    std::array<uint8_t, CHIP8_WIDTH * CHIP8_HEIGHT> display_buffer;
    for(size_t y = 0; y < display.size(); y++)
    {
        for(size_t x = 0; x < display[y].size(); x++)
        {
            display_buffer[y + x] = display[y][x];
        }
    }

    // Display the pixels
    SDL_UpdateTexture(window_ref.texture, nullptr, display_buffer.data(), CHIP8_WIDTH * sizeof(uint32_t));
    window_ref.clear();
    SDL_RenderCopy(window_ref.renderer, window_ref.texture, nullptr, nullptr);
    window_ref.render();

    // All instructions are 2 bytes long.
    pc += INSTRUCTION_LONG;
}

// This is not implemented because i'm not trying to 
// emulate the RCA 1802 CPU
void Chip8::OPCODE_0NNN_Impl() {}

void Chip8::OPCODE_00E0_Impl()
{
    // resetting the display
    std::fill(&display[0][0], &display[0][0] + sizeof(display), 0);
}

void Chip8::OPCODE_00EE_Impl()
{
    pc = stack.back();
    sp--;
}

void Chip8::OPCODE_1NNN_Impl()
{
    pc = inst_var.nnn;
}  

void Chip8::OPCODE_2NNN_Impl()
{
    sp++;
    stack[STACK_SIZE - 1] = pc;
    pc = inst_var.nnn;
}

void Chip8::OPCODE_3XKK_Impl()
{
    pc = registers[inst_var.x] == inst_var.kk 
        ? pc + INSTRUCTION_LONG : pc;
}

void Chip8::OPCODE_4XKK_Impl()
{
    pc = registers[inst_var.x] != inst_var.kk 
        ? pc + INSTRUCTION_LONG : pc;
}

void Chip8::OPCODE_5XY0_Impl()
{
    pc = registers[inst_var.x] == registers[inst_var.y] 
        ? pc + INSTRUCTION_LONG : pc;
}

void Chip8::OPCODE_6XKK_Impl()
{
    registers[inst_var.x] = inst_var.kk;
}

void Chip8::OPCODE_7XKK_Impl()
{
    registers[inst_var.x] += inst_var.kk;
}

void Chip8::OPCODE_8XY0_Impl()
{
    registers[inst_var.x] = registers[inst_var.y];
}

void Chip8::OPCODE_8XY1_Impl()
{
    registers[inst_var.x] |= registers[inst_var.y];
}   

void Chip8::OPCODE_8XY2_Impl()
{
    registers[inst_var.x] &= registers[inst_var.y];
}

void Chip8::OPCODE_8XY3_Impl()
{
    registers[inst_var.x] ^= registers[inst_var.y];
}

void Chip8::OPCODE_8XY4_Impl()
{
    const auto sum = registers[inst_var.x] + registers[inst_var.y];
    // checking a carry when sum bigger than a byte
    registers[REGISTER_SIZE - 1] = sum > 255 
        ? 1 : 0; 
}

void Chip8::OPCODE_8XY5_Impl()
{
    registers[REGISTER_SIZE - 1] = registers[inst_var.x] > registers[inst_var.y] 
        ? 1 : 0;

    registers[inst_var.x] -= registers[inst_var.y];
}

void Chip8::OPCODE_8XY6_Impl()
{
    registers[REGISTER_SIZE - 1] = registers[inst_var.x] & 0x1;
    registers[inst_var.x] /= 2;
}

void Chip8::OPCODE_8XY7_Impl()
{
    registers[REGISTER_SIZE - 1] = registers[inst_var.y] > registers[inst_var.x] 
        ? 1 : 0;

    registers[inst_var.x] = registers[inst_var.y] - registers[inst_var.x];
}

void Chip8::OPCODE_8XYE_Impl()
{
    registers[REGISTER_SIZE - 1] = (registers[inst_var.x] & 0x80) >> 7;
    registers[inst_var.x] *= 2;
}

void Chip8::OPCODE_9XY0_Impl()
{
    pc = registers[inst_var.x] != registers[inst_var.y] 
        ? pc + 2 : pc;
}

void Chip8::OPCODE_ANNN_Impl()
{
    I = inst_var.nnn;
}

void Chip8::OPCODE_BNNN_Impl()
{
    pc = registers[0] + inst_var.nnn;
}

void Chip8::OPCODE_CXKK_Impl()
{
    registers[inst_var.x] = random_byte() & inst_var.kk;
}

void Chip8::OPCODE_DXYN_Impl()
{
    registers[REGISTER_SIZE - 1] = 0;

    uint16_t pixel;
    for(int y = 0; y < inst_var.n; y++)
    {
        pixel = memory[I + y];

        for(int x = 0; x < 8; x++)
        {
            if((pixel & (0x80 >> x)) != 0)
            {
                registers[REGISTER_SIZE - 1] = display[(inst_var.x + x)][((inst_var.y + y) * CHIP8_WIDTH)] == 1 
                    ? 1 : registers[REGISTER_SIZE - 1];

                display[(inst_var.x + x)][((inst_var.y + y) * CHIP8_WIDTH)] ^= 1;
            }
        }
    }
}

void Chip8::OPCODE_EX9E_Impl()
{
    pc = keypads[registers[inst_var.x]] != 0 
        ? pc + INSTRUCTION_LONG : pc;
}

void Chip8::OPCODE_EXA1_Impl()
{
    pc = keypads[registers[inst_var.x]] == 0 
        ? pc + INSTRUCTION_LONG : pc;
}

void Chip8::OPCODE_FX07_Impl()
{
    registers[inst_var.x] = dt;
}

void Chip8::OPCODE_FX0A_Impl()
{
    for(int i = 0; i < KEYPADS_SIZE; i++)
    {
        if(keypads[i] != 0)
        {
            registers[inst_var.x] = keypads[i];
        }
    }
}

void Chip8::OPCODE_FX15_Impl()
{
    dt = registers[inst_var.x];
}

// sound not implemented
void Chip8::OPCODE_FX18_Impl() {}

void Chip8::OPCODE_FX1E_Impl()
{
    I += registers[REGISTER_SIZE - 1];
}

void Chip8::OPCODE_FX29_Impl()
{
    I = registers[inst_var.x] * 5;
}   

void Chip8::OPCODE_FX33_Impl()
{
    uint8_t value = registers[inst_var.x];

    memory[I + 2] = value % 10; 
    value /= 10;

    memory[I + 1] = value % 10;
    value /= 10;

    memory[I] = value % 10;
}

void Chip8::OPCODE_FX55_Impl()
{
    for(uint8_t i = 0; i <= inst_var.x; i++) 
    {
        memory[I + i] = registers[i];
    }
}

void Chip8::OPCODE_FX65_Impl()
{
    for(uint8_t i = 0; i <= inst_var.x; i++)
    {
        registers[i] = memory[I + i];
    }
}