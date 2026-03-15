#ifndef CHIP8_H
#define CHIP8_H
#include <cstdint>
#include <string>
#include <random>

namespace Emulator {
    constexpr unsigned int START_ADDRESS = 0x200;

    constexpr unsigned int FONTSET_SIZE = 80;
    constexpr unsigned int FONTSET_START_ADDRESS = 0x50;

    constexpr unsigned int VIDEO_HEIGHT = 50;
    constexpr unsigned int VIDEO_WIDTH = 50;

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
        0xF0, 0x80, 0xF0, 0x80, 0x80 // F
    };


    class Chip8 {
    using Chip8Func = void (Chip8::*)();

    public:
        Chip8();
        void LoadROM(const std::string &filename);

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
        void OP_8xy6(); // SHR Vx - Set Vx =  Vx SHR 1
        void OP_8xy7(); // SUBN Vx, Vy - Set Vx = Vy - Vx, Set VF = Vy > Vx
        void OP_8xyE(); // SHL Vx {, Vy}
        void OP_9xy0(); // SNE Vx, Vy - Skip next instruction if Vx != Vy
        void OP_Annn(); // LD I, addr - Set I = nnn
        void OP_Bnnn(); // JP V0, addr - Jump to location nnn + V0
        void OP_Cxkk(); // RND Vx, byte - Set Vx = random byte AND kk
        void OP_Dxyn();
        // DRW Vx, Vy, nibble - Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
        void OP_Ex9e(); // SKP Vx - Skip the next instruction if a key with the value of Vx is pressed
        void OP_ExA1(); // SKNP Vx - Skip the next instruction if a key with the value of Vx is not pressed
        void OP_Fx07(); // LD Vx, DT - Set Vx = delay timer value
        void OP_Fx0A(); // LD Vx, K - Wait for a key press, store the value of the key in Vx
        void OP_Fx15(); // LD DT, Vx - Set delay timer = Vx
        void OP_Fx18(); // LS ST, Vx - Set sound timer = Vx
        void OP_Fx1E(); // ADD I, Vx - Set I = I + Vx
        void OP_Fx29(); // LD F, Vx - Set I = Location of sprite for digit Vx
        void OP_Fx33(); // LD B, Vx - Store BCD representation of Vx in memory location I, I+1, and I+2
        void OP_Fx55(); // LD [I], Vx - Store registers V0 through Vx in memory starting at location I
        void OP_Fx65(); // LD Vx, [I] - Read registers V0 through Vx from memory starting at location I

        // Tables
        void Table0();
        void Table8();
        void TableE();
        void TableF();
        void OP_NULL();

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

        Chip8Func table[0xF + 1];
        Chip8Func table0[0xE + 1];
        Chip8Func table8[0xE + 1];
        Chip8Func tableE[0xE + 1];
        Chip8Func tableF[0x65 + 1];

        std::default_random_engine randGen;
        std::uniform_int_distribution<uint8_t> randByte;
    };
}

#endif / CHIP8_H/
