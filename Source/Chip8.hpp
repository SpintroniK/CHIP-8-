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
        mainTask = std::thread(std::bind_front(&Chip8::Loop, this));
    }

    void Stop()
    {
        stopToken.store(true);

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

    auto GetPlaySound() const
    {
        return cpu.GetSoundTimer() > 0;
    }
    

private:

    void Loop()
    {
        using namespace std::chrono_literals;
        using namespace std::this_thread;

        for(;;)
        {
            cpu.Step();
            sleep_for(2000us);  // ~500 instructions / second

            if(stopToken.load())
            {
                return;
            }
        }
    }

    Cpu cpu;
    std::thread mainTask;
    std::atomic<bool> stopToken{false};
};