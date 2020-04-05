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

        // Configuring texture
        window_ref.texture = SDL_CreateTexture(window_ref.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, CHIP8_WIDTH, CHIP8_HEIGHT);

        // Starting position of CPU
        pc = LOCATION_START;

        // setting up the opcode table
        // opcode_table[OPCODE_0NNN] = [this](){ OPCODE_0NNN_Impl(); };
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
    opcode = (memory[pc] << 8) | memory[pc + 1];
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

void Chip8::key_handling() noexcept
{
    static constexpr std::initializer_list<SDL_Keycode> KEYS = {
        SDLK_x, SDLK_1, SDLK_2, SDLK_3,
        SDLK_q, SDLK_w, SDLK_e, SDLK_a,
        SDLK_s, SDLK_d, SDLK_z, SDLK_c,
        SDLK_4, SDLK_r, SDLK_f, SDLK_v
    };

    while(SDL_PollEvent(&window_ref.event))
    {
        switch(window_ref.event.type)
        {
            case SDL_KEYDOWN:
            for(auto key : KEYS)
            {
                if(window_ref.event.key.keysym.sym == key)
                {
                    keypads[key] = 1;
                }
            }
            break;

            case SDL_KEYUP:
            for(auto key : KEYS)
            {
                if(window_ref.event.key.keysym.sym == key)
                {
                    keypads[key] = 0;
                }
            }
            break;
        }
    }
}

void Chip8::cycle() noexcept
{
    // printing out all of the memory
    #if ENABLE_DEBUG_MODE
    clear_terminal();
    std::cout << std::hex << get_memory_as_string(LOCATION_START, 1000) << std::endl;
    #endif // ENABLE_DEBUG_MODE

    // All instructions are 2 bytes long.
    pc += INSTRUCTION_LONG;

    fetch_opcode();
    fetch_instruction_variables();
    call_opcodes();
    render();

    key_handling();

    // decrease delay timer
    if(dt > 0)
    {
        dt--;
    }
}

void Chip8::call_opcodes() noexcept
{
    // get the first nibble and check what type the opcode is
    switch(opcode & 0xF000)
    {
        case 0x1000: case 0x2000: case 0x3000: case 0x4000:
        case 0x5000: case 0x6000: case 0x7000: case 0x9000:
        case 0xA000: case 0xB000: case 0xC000: case 0xD000:
        {
            const uint16_t masked_opcode = opcode & 0xF000;

            // just to make sure memory will not go out of bounds
            if(opcode_table.find(masked_opcode) != opcode_table.end())
            {
                opcode_table[masked_opcode]();
            }
        }
        break;

        case 0xF000:
        {
            const uint16_t masked_opcode = opcode & 0xF0FF;

            // just to make sure memory will not go out of bounds
            if(opcode_table.find(masked_opcode) != opcode_table.end())
            {
                opcode_table[masked_opcode]();
            }
        }
        break;

        default:
        {
            const uint16_t masked_opcode = opcode & 0xF00F;

            // just to make sure memory will not go out of bounds
            if(opcode_table.find(masked_opcode) != opcode_table.end())
            {
                opcode_table[masked_opcode]();
            }
        }
        break;
    }
}

void Chip8::render() noexcept
{
    SDL_UpdateTexture(window_ref.texture, nullptr, display.data(), sizeof(decltype(display[0])) * CHIP8_WIDTH);
    SDL_RenderClear(window_ref.renderer);
    SDL_RenderCopy(window_ref.renderer, window_ref.texture, nullptr, nullptr);
    SDL_RenderPresent(window_ref.renderer);
}

// This is not implemented because i'm not trying to 
// emulate the RCA 1802 CPU
// This instruction is only used on the old computers on which Chip-8 was originally implemented. It is ignored by modern interpreters.
// void Chip8::OPCODE_0NNN_Impl() {}

// Clear the display.
void Chip8::OPCODE_00E0_Impl()
{
    std::fill(display.begin(), display.end(), 0);
}

// The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
void Chip8::OPCODE_00EE_Impl()
{
    sp--;
    pc = stack[sp];
}

// The interpreter sets the program counter to nnn.
void Chip8::OPCODE_1NNN_Impl()
{
    pc = inst_var.nnn;
}  

// The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
void Chip8::OPCODE_2NNN_Impl()
{
    stack[sp] = pc;
    sp++;
    pc = inst_var.nnn;
}

// The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
void Chip8::OPCODE_3XKK_Impl()
{
    if(registers[inst_var.x] == inst_var.kk)
    {
        pc += INSTRUCTION_LONG;
    }
}

// The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
void Chip8::OPCODE_4XKK_Impl()
{
    if(registers[inst_var.x] != inst_var.kk)
    {
        pc += INSTRUCTION_LONG;
    }
}

// The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
void Chip8::OPCODE_5XY0_Impl()
{
    if(registers[inst_var.x] == registers[inst_var.y] )
    {
        pc += INSTRUCTION_LONG;
    }
}

// The interpreter puts the value kk into register Vx.
void Chip8::OPCODE_6XKK_Impl()
{
    registers[inst_var.x] = inst_var.kk;
}

// Adds the value kk to the value of register Vx, then stores the result in Vx. 
void Chip8::OPCODE_7XKK_Impl()
{
    registers[inst_var.x] += inst_var.kk;
}

// Stores the value of register Vy in register Vx.
void Chip8::OPCODE_8XY0_Impl()
{
    registers[inst_var.x] = registers[inst_var.y];
}

// Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. 
// A bitwise OR compares the corrseponding bits from two values, and if either bit is 1, then the same bit in the result is also 1. Otherwise, it is 0. 
void Chip8::OPCODE_8XY1_Impl()
{
    registers[inst_var.x] |= registers[inst_var.y];
}   

// Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. 
// A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, then the same bit in the result is also 1. Otherwise, it is 0. 
void Chip8::OPCODE_8XY2_Impl()
{
    registers[inst_var.x] &= registers[inst_var.y];
}

// Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. 
// An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, then the corresponding bit in the result is set to 1. Otherwise, it is 0.
void Chip8::OPCODE_8XY3_Impl()
{
    registers[inst_var.x] ^= registers[inst_var.y];
}

// The values of Vx and Vy are added together. 
// If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
void Chip8::OPCODE_8XY4_Impl()
{
    const auto sum = registers[inst_var.x] + registers[inst_var.y];
    // checking a carry when sum bigger than a byte
    registers[REGISTER_SIZE - 1] = sum > 255 
        ? 1 : 0; 

    registers[inst_var.x] = sum & 0xFF;
}

// If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
void Chip8::OPCODE_8XY5_Impl()
{
    registers[REGISTER_SIZE - 1] = registers[inst_var.x] > registers[inst_var.y] 
        ? 1 : 0;

    registers[inst_var.x] -= registers[inst_var.y];
}

// If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
void Chip8::OPCODE_8XY6_Impl()
{
    registers[REGISTER_SIZE - 1] = registers[inst_var.x] & 0x1;
    registers[inst_var.x] >>= 1;
}

// If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
void Chip8::OPCODE_8XY7_Impl()
{
    registers[REGISTER_SIZE - 1] = registers[inst_var.y] > registers[inst_var.x] 
        ? 1 : 0;

    registers[inst_var.x] = registers[inst_var.y] - registers[inst_var.x];
}

// If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
void Chip8::OPCODE_8XYE_Impl()
{
    registers[REGISTER_SIZE - 1] = (registers[inst_var.x] & 0x80) >> 7;
    registers[inst_var.x] <<= 1;
}

// The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
void Chip8::OPCODE_9XY0_Impl()
{
    if(registers[inst_var.x] != registers[inst_var.y])
    {
        pc += INSTRUCTION_LONG;
    }
}

// The value of register I is set to nnn.
void Chip8::OPCODE_ANNN_Impl()
{
    I = inst_var.nnn;
}

// The program counter is set to nnn plus the value of V0.
void Chip8::OPCODE_BNNN_Impl()
{
    pc = registers[0] + inst_var.nnn;
}

// The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. The results are stored in Vx.
void Chip8::OPCODE_CXKK_Impl()
{
    registers[inst_var.x] = random_byte() & inst_var.kk;
}

// The interpreter reads n bytes from memory, starting at the address stored in I. 
// These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). 
// Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. 
// If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen.
void Chip8::OPCODE_DXYN_Impl()
{
    constexpr auto VIDEO_PIXEL = 0xFFFFFFFF;
    registers[REGISTER_SIZE - 1] = 0;

    uint8_t x = registers[inst_var.x];
	uint8_t y = registers[inst_var.y];

    for (unsigned int row = 0; row < inst_var.n; row++)
	{
		uint8_t sprite = memory[I + row];

		for (unsigned int col = 0; col < 8; col++)
		{
			uint8_t pixel = sprite & (0x80 >> col);
            uint32_t array_index = (y + row) * CHIP8_WIDTH + (x + col);

			if (pixel != 0)
			{
				if (display[array_index] == VIDEO_PIXEL)
				{
					registers[0xF] = 1;
				}

				display[array_index] ^= VIDEO_PIXEL;
			}
		}
	}
}

// Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
void Chip8::OPCODE_EX9E_Impl()
{
    if(keypads[registers[inst_var.x]])
    {
        pc += INSTRUCTION_LONG;
    }
}

// Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
void Chip8::OPCODE_EXA1_Impl()
{
    if(not keypads[registers[inst_var.x]])
    {
        pc += INSTRUCTION_LONG;
    }
}

// The value of DT is placed into Vx.
void Chip8::OPCODE_FX07_Impl()
{
    registers[inst_var.x] = dt;
}

// All execution stops until a key is pressed, then the value of that key is stored in Vx.
void Chip8::OPCODE_FX0A_Impl()
{
    bool key_pressed = false;
    for(int i = 0; i < KEYPADS_SIZE; i++)
    {
        if(keypads[i])
        {
            registers[inst_var.x] = keypads[i];
            key_pressed == true;
        }
    }

    if(not key_pressed)
    {
        pc -= INSTRUCTION_LONG;
    }
}

// DT is set equal to the value of Vx.
void Chip8::OPCODE_FX15_Impl()
{
    dt = registers[inst_var.x];
}

// sound not implemented
// ST is set equal to the value of Vx.
void Chip8::OPCODE_FX18_Impl() {}

// The values of I and Vx are added, and the results are stored in I.
void Chip8::OPCODE_FX1E_Impl()
{
    I += registers[inst_var.x];
}

// The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx.
void Chip8::OPCODE_FX29_Impl()
{
    I = 5 * registers[inst_var.x];
}   

// The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.
void Chip8::OPCODE_FX33_Impl()
{
    uint8_t value = registers[inst_var.x];

    memory[I + 2] = value % 10; 
    value /= 10;

    memory[I + 1] = value % 10;
    value /= 10;

    memory[I] = value % 10;
}

// The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
void Chip8::OPCODE_FX55_Impl()
{
    for(uint8_t i = 0; i <= inst_var.x; i++) 
    {
        memory[I + i] = registers[i];
    }
}

// The interpreter reads values from memory starting at location I into registers V0 through Vx.
void Chip8::OPCODE_FX65_Impl()
{
    for(uint8_t i = 0; i <= inst_var.x; i++)
    {
        registers[i] = memory[I + i];
    }
}