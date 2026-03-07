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
}
