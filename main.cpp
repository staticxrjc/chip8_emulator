#include "lib/Chip8.h"
#include "lib/Platform.h"
#include "spdlog/spdlog.h"

int main(int argc, char **argv) {
    if (argc != 4) {
        spdlog::error("Usage: {} <Scale> <Delay> <ROM>", argv[0]);
        std::exit(EXIT_FAILURE);
    }

    int videoScale = std::stoi(argv[1]);
    int cycleDelay = std::stoi(argv[2]);
    char const *romFilename = argv[3];

    spdlog::info("Starting CHIP-8 Emulator");
    spdlog::info("Scale: {}", videoScale);
    spdlog::info("Cycle Delay: {}", cycleDelay);
    spdlog::info("ROM: {}", romFilename);
    spdlog::info("--------------------------------");

    Emulator::Platform platform(
        "CHIP-8 Emulator",
        Emulator::VIDEO_WIDTH * videoScale,
        Emulator::VIDEO_HEIGHT * videoScale,
        Emulator::VIDEO_WIDTH,
        Emulator::VIDEO_HEIGHT
    );

    Emulator::Chip8 chip8;
    chip8.LoadROM(romFilename);

    int videoPitch = sizeof(chip8.video[0]) * Emulator::VIDEO_WIDTH;

    auto lastCycleTime = std::chrono::high_resolution_clock::now();
    bool quit = false;

    while (!quit)
    {
        quit = platform.ProcessInput(chip8.keypad);

        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

        if (dt > cycleDelay)
        {
            lastCycleTime = currentTime;

            chip8.Cycle();

            platform.Update(chip8.video, videoPitch);
        }
    }

    return 0;
}
