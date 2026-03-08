#ifndef CHIP8_H
#define CHIP8_H
#include <cstdint>
#include <string>
#include <random>

namespace Emulator {
    constexpr unsigned int START_ADDRESS = 0x200;

    constexpr unsigned int FONTSET_SIZE = 80;
    constexpr unsigned int FONTSET_START_ADDRESS = 0x50;

    inline uint8_t fontset[FONTSET_SIZE] =
    {
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

    class Chip8 {
    public:
        Chip8();
        void LoadROM(const std::string& filename);

        // Op Codes
        void OP_00E0(); // CLS - Clear the Display
        void OP_00EE(); // RET - Return from a subroutine
        void OP_1nnn(); // JMP addr - Jump to location nnn
        void OP_2nnn(); // CALL addr - Call a subroutine at nnn
        void OP_3xkk(); // SE Vx, byte - Skip next instruction if Vx = kk
        void OP_4xkk(); // SNE Vx, byte - Skip next instruction if Vx != kk
        void OP_5xy0(); // SE Vx, Vy - Skip next instruction if Vx = Vy
        void OP_6xkk(); // LD Vx, byte - Set Vx = kk
        void OP_7xkk(); // ADD Vx, byte - Set Vx = Vx + kk
        void OP_8xy0(); // LD Vx, Vy - Set Vx = Vy
        void OP_8xy1(); // OR Vx, Vy - Set Vx = VX OR Vy
        void OP_8xy2(); // AND Vx, Vy - Set Vx = VX AND Vy
        void OP_8xy3(); // XOR Vx, Vy - Set Vx = VX XOR Vy
        void OP_8xy4(); // ADD Vx, Vy - Set Vx = Vx + Vy, Set VF = carry
        void OP_8xy5(); // SUB Vx, Vy - Set Vx = Vx - Vy, Set VF = Vx > Vy

    private:
        uint8_t registers[16]{}; // VF, last register address 0xFu
        uint8_t memory[4096]{};
        uint16_t index{};
        uint16_t pc{};
        uint16_t stack[16]{};
        uint8_t sp{};
        uint8_t delayTimer{};
        uint8_t soundTimer{};
        uint8_t keypad[16]{};
        uint32_t video[64 * 32]{};
        uint16_t opcode;

        std::default_random_engine randGen;
        std::uniform_int_distribution<uint8_t> randByte;
    };
}

#endif / CHIP8_H/
