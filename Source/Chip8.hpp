#pragma once

#include "Cpu.hpp"
#include "Display.hpp"
#include "Rom.hpp"

#include <atomic>
#include <chrono>
#include <thread>
#include <mutex>

class Chip8
{

public:
    Chip8() = default;
    ~Chip8() = default;

    void LoadRom(const Rom& rom)
    {
        cpu.LoadProgram(rom.GetData());
    }

    void Start()
    {
        isRunning.store(true);
        mainTask = std::thread(&Chip8::Loop, this);
    }

    void Stop()
    {
        isRunning.store(false);
        if(mainTask.joinable())
        {
            mainTask.join();
        }
    }

    auto GetScreen() const
    {
        std::lock_guard<std::mutex> lock(screenMutex);
        // screen = 
        return cpu.GetScreen();
    }

private:

    void Loop()
    {
        using namespace std::chrono_literals;
        using namespace std::this_thread;

        while(isRunning)
        {
            cpu.Step();
            sleep_for(2857us);  // ~350 instructions / second
        }
    }

    Cpu cpu;
    Screen screen;

    mutable std::mutex screenMutex;
    std::thread mainTask;
    std::atomic<bool> isRunning{false};

};