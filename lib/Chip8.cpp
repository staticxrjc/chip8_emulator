#include "Chip8.h"

#include <chrono>
#include <cstring>
#include <fstream>

namespace Emulator {
    Chip8::Chip8()
        : randGen(std::chrono::system_clock::now().time_since_epoch().count()) {
        // Initialize pc
        pc = START_ADDRESS;

        // Load Fonts into memory
        for (size_t i{}; i < FONTSET_SIZE; ++i) {
            memory[FONTSET_START_ADDRESS + i] = fontset[i];
        }

        // Initialize RNG
        randByte = std::uniform_int_distribution<uint8_t>(0, 255U);
    }

    void Chip8::LoadROM(const std::string &filename) {
        // Open the file as a stream of binary and move the file pointer to the end

        if (std::ifstream file(filename, std::ios::binary | std::ios::ate); file.is_open()) {
            // Get size of file and allocate a buffer to hold the contents
            const std::streampos size = file.tellg();
            const auto buffer = new char[size];

            // Go back to the beginning of the file and fill the buffer
            file.seekg(0, std::ios::beg);
            file.read(buffer, size);
            file.close();

            // Load the ROM contents into the Chip8's memory, starting at 0x200
            for (size_t i{}; i < size; ++i) {
                memory[START_ADDRESS + i] = buffer[i];
            }

            // Free the buffer
            delete[] buffer;
        }
    }

    void Chip8::OP_00E0() {
        memset(video, 0, sizeof(video));
    }

    void Chip8::OP_00EE() {
        --sp;
        pc = stack[sp];
    }

    void Chip8::OP_1nnn() {
        const uint16_t address = opcode & 0x0FFFu;

        pc = address;
    }

    void Chip8::OP_2nnn() {
        const uint16_t address = opcode & 0x0FFFu;

        stack[sp] = pc;
        ++sp;
        pc = address;
    }

    void Chip8::OP_3xkk() {
        const uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        const uint8_t byte = opcode & 0x00FFu;

        if (registers[Vx] == byte) {
            pc += 2;
        }
    }

    void Chip8::OP_4xkk() {
        const uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        const uint8_t byte = opcode & 0x00FFu;

        if (registers[Vx] != byte) {
            pc += 2;
        }
    }

    void Chip8::OP_5xy0() {
        const uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        const uint8_t Vy = (opcode & 0x00F0u) >> 4u;

        if (registers[Vx] == registers[Vy]) {
            pc += 2;
        }
    }

    void Chip8::OP_6xkk() {
        const uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        const uint8_t byte = opcode & 0x00FFu;

        registers[Vx] = byte;
    }

    void Chip8::OP_7xkk() {
        const uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        const uint8_t byte = opcode & 0x00FFu;

        registers[Vx] += byte;
    }

    void Chip8::OP_8xy0() {
        const uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        const uint8_t Vy = (opcode & 0x00F0u) >> 4u;

        registers[Vx] = registers[Vy];
    }

    void Chip8::OP_8xy1() {
        const uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        const uint8_t Vy = (opcode & 0x00F0u) >> 4u;

        registers[Vx] |= registers[Vy];
    }

    void Chip8::OP_8xy2() {
        const uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        const uint8_t Vy = (opcode & 0x00F0u) >> 4u;

        registers[Vx] &= registers[Vy];
    }

    void Chip8::OP_8xy3() {
        const uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        const uint8_t Vy = (opcode & 0x00F0u) >> 4u;

        registers[Vx] ^= registers[Vy];
    }

    void Chip8::OP_8xy4() {
        const uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        const uint8_t Vy = (opcode & 0x00F0u) >> 4u;

        const uint16_t sum = registers[Vx] + registers[Vy];
        if ( sum > 255U) {
            registers[0xF] = 1;
        }
        else {
            registers[0xF] = 0;
        }

        registers[Vx] = sum & 0xFFu;
    }

    void Chip8::OP_8xy5() {
        const uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        const uint8_t Vy = (opcode & 0x00F0u) >> 4u;

        if (Vx > Vy) {
            registers[0xF] = 1;
        }
        else {
            registers[0xF] = 0;
        }

        registers[Vx] -= registers[Vy];

    }

    void Chip8::OP_8xy6() {
        const uint8_t Vx = (opcode & 0x0F00u) >> 8u;

        // Save LSB in VF
        registers[0xF] = registers[Vx & 0x1u];

        registers[Vx] >>= 1;
    }

    void Chip8::OP_8xy7() {
        const uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        const uint8_t Vy = (opcode & 0x00F0u) >> 4u;

        if (Vy > Vx) {
            registers[0xF] = 1;
        }
        else {
            registers[0xF] = 0;
        }

        registers[Vy] -= registers[Vx];
    }

    void Chip8::OP_8xYE() {
        const uint8_t Vx = (opcode & 0x0FF00u) >> 8u;

        // Save MSB in VF
        registers[0xF] = (registers[Vx] & 0x80u) >> 7u;

        registers[Vx] <<= 1;
    }

    void Chip8::OP_9xy0() {
        const uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        const uint8_t Vy = (opcode & 0x00F0u) >> 4u;

        if (registers[Vx] != registers[Vy]) {
            pc += 2;
        }

    }

    void Chip8::OP_Annn() {
        const uint16_t address = opcode & 0x0FFFu;

        index = address;
    }

    void Chip8::OP_Bnnn() {
        const uint16_t address = opcode & 0x0FFFu;

        pc = address + registers[0];
    }

    void Chip8::OP_Cxkk() {
        const uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        const uint8_t byte = (opcode & 0x00FFu);

        registers[Vx] = randByte(randGen) & byte;
    }

    void Chip8::OP_Dxyn() {
        const uint8_t Vx = (opcode & 0x0F00u) >> 8u;
        const uint8_t Vy = (opcode & 0x00F0u) >> 4u;
        const uint8_t height = (opcode & 0x000Fu);

        // Wrap if going beyond screen boundaries
        const uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
        const uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

        registers[0xF] = 0;

        for (size_t row{}; row < height; ++row) {
            const uint8_t spriteByte = memory[index + row];

            for (size_t col{}; col < 8; ++col) {
                const uint8_t spritePixel = spriteByte & (0x80 >> col);
                uint32_t* screenPixel = &video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

                // Sprite pixel is on
                if (spritePixel)
                {
                    // Screen pixel also on - collision
                    if (*screenPixel == 0xFFFFFFFF)
                    {
                        registers[0xF] = 1;
                    }

                    // Effectively XOR with the sprite pixel
                    *screenPixel ^= 0xFFFFFFFF;
                }
            }
        }
    }
}
