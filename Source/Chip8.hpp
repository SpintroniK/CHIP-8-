#pragma once

#include "Cpu.hpp"
#include "KeyPad.hpp"
#include "Rom.hpp"

#include <array>
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

    auto SetKeys(const KeyArray& k)
    {
        cpu.SetKeys(k);
    }

    auto GetScreen() const
    {
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
            sleep_for(1429us);  // ~700 instructions / second
        }
    }

    Cpu cpu;

    std::thread mainTask;
    std::atomic<bool> isRunning{false};

};