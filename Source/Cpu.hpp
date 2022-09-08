#pragma once

#include "Defintions.hpp"
#include "Display.hpp"
#include "Helpers.hpp"
#include "Instructions.hpp"
#include "KeyPad.hpp"
#include "Memory.hpp"
#include "Timer.hpp"

#include <array>
#include <bit>
#include <chrono>
#include <functional>
#include <iostream>
#include <limits>
#include <mutex>
#include <random>
#include <thread>


class Cpu
{

public:

    Cpu()
    {
        memory.WriteChunk(fontSet, fontOffset);

        // Start timers
        timersTask = std::thread(std::bind_front(&Cpu::TimersLoop, this));
    }

    ~Cpu()
    {
        stopToken.store(true);
        timersTask.join();
    }

    void ExecuteInstruction(Instruction_t instruction)
    {
        const Byte_t instructionCode = GetNibble<0>(instruction);
        std::invoke(instructionSets[instructionCode], this, instruction);
    }

    void Step()
    {
        const auto instruction = memory.Read<Instruction_t>(pc);
        pc += sizeof(Instruction_t);
        ExecuteInstruction(instruction);
    }

    template <typename T>
    void LoadProgram(const T& data)
    {
        LoadToMemory(data, programLoadAddress);
    }

    void SetKeys(const KeyArray& k)
    {
        std::lock_guard<std::mutex> lock(keysMutex);
        keys = k;
    }

    auto GetScreen() const 
    {
        std::lock_guard<std::mutex> lock(screenMutex);
        return screen;
    }

    auto GetSoundTimer() const
    {
        return soundTimer.Count();
    }

private:

    void TimersLoop()
    {
        using namespace std::chrono;
        using namespace std::this_thread;

        for(;;)
        {
            const auto dt = double{nanoseconds::period::den} / 60.;
            sleep_for(nanoseconds{static_cast<std::uint64_t>(dt)});

            delayTimer.Dec();
            soundTimer.Dec();
            // std::cout << +delayTimer.Count() << std::endl;

            if(stopToken.load())
            {
                return;
            }
        }

    }

    auto GetKeys() const
    {
        std::lock_guard<std::mutex> lock(keysMutex);
        return keys;
    }

    template <typename T>
    void LoadToMemory(const T& data, Address_t address)
    {
        memory.WriteChunk(data, address);
    }

    void ClsRet(Instruction_t instruction)
    {
        if(instruction == OpCode::Cls)
        {
            std::lock_guard<std::mutex> lock(screenMutex);
            screen.Clear();
        }

        if(instruction == OpCode::Ret)
        {
            pc = stack[--sp];   // Return from subroutine
        }
    }

    void Jp(Instruction_t instruction)
    {
        pc = GetLowest12bitAddr(instruction);
    }

    void Call(Instruction_t instruction)
    {
        stack[sp++] = pc;
        pc = GetLowest12bitAddr(instruction);
    }

    void SeByte(Instruction_t instruction)
    {
        const auto x = GetNibble<1>(instruction);

        if(V[x] == GetLowestByte(instruction))
        {
            pc += 2;
        }
    }

    void SneByte(Instruction_t instruction)
    {
        const auto x = GetNibble<1>(instruction);

        if(V[x] != GetLowestByte(instruction))
        {
            pc += 2;
        }
    }

    void SeReg(Instruction_t instruction)
    {
        const auto x = GetNibble<1>(instruction);
        const auto y = GetNibble<2>(instruction);

        if(V[x] == V[y])
        {
            pc += 2;
        }
    }

    void Ld(Instruction_t instruction)
    {
        const auto x = GetNibble<1>(instruction);
        V[x] = GetLowestByte(instruction);
    }

    void Add(Instruction_t instruction)
    {
        const auto x = GetNibble<1>(instruction);
        V[x] += GetLowestByte(instruction);
    }

    void Op(Instruction_t instruction)
    {
        const auto x = GetNibble<1>(instruction);
        const auto y = GetNibble<2>(instruction);
        const auto n = GetNibble<3>(instruction);

        switch(n)
        {
            case 0: V[x] = V[y]; break; // Stores the value of register Vy in register Vx.
            case 1: V[x] = V[x] | V[y]; V[0xF] = 0; break; // OR
            case 2: V[x] = V[x] & V[y]; V[0xF] = 0; break; // AND
            case 3: V[x] = V[x] ^ V[y]; V[0xF] = 0; break; // XOR
            case 4: 
            {
                const auto s = V[x] + V[y];
                (s > std::numeric_limits<Byte_t>::max()) ? V[0xF] = 1 : V[0xF] = 0; 
                V[x] = static_cast<Byte_t>(s & 0xFF);
                break;
            } 
            case 5:
            {
                V[0xF] = V[x] > V[y] ? 1U : 0U;
                V[x] = V[x] - V[y];
                break;
            }
            case 6:
            {
                V[x] = V[y];
                const Register_t c = (V[x] & 0x01) ? 1 : 0;
                V[x] = V[x] >> 1;
                V[0xF] = c;
                break;
            } 
            case 7:
            {
                V[x] = V[y] - V[x];
                V[0xF] = (V[y] > V[x]) ? 1 : 0;
                break;
            }
            case 0xe:
            {
                V[x] = V[y];
                V[x] = V[x] << 1;
                V[0xF] = (V[x] & 0x80) ? 1 : 0;
                break;
            }
            default: break;
        }
    }

    void SneReg(Instruction_t instruction)
    {
        const auto x = GetNibble<1>(instruction);
        const auto y = GetNibble<2>(instruction);

        if(V[x] != V[y])
        {
            pc += 2;
        }
    }

    void SetI(Instruction_t instruction)
    {
        I = GetLowest12bitAddr(instruction);
    }

    void JpV0(Instruction_t instruction)
    {
        const auto addr = GetLowest12bitAddr(instruction);
        pc = V[0] + addr;
    }

    void Rnd(Instruction_t instruction)
    {
        std::random_device r{};
        std::mt19937 generator{r()};
        std::uniform_int_distribution<std::uint16_t> distrib{std::numeric_limits<Byte_t>::min(), std::numeric_limits<Byte_t>::max()};

        const auto x = GetNibble<1>(instruction);
        V[x] = static_cast<Byte_t>(distrib(generator)) & GetLowestByte(instruction);
    }

    void Drw(Instruction_t instruction)
    {
        const auto x = GetNibble<1>(instruction);
        const auto y = GetNibble<2>(instruction);

        const auto xSprite = V[x];
        const auto ySprite = V[y];

        const auto spriteAddress = I;
        const auto spriteLength = GetNibble<3>(instruction);

        const auto spriteData = memory.ReadChunk(spriteAddress, spriteLength);
        
        const Sprite sprite{xSprite, ySprite, spriteData};

        std::lock_guard<std::mutex> lock(screenMutex);
        V[0xF] = screen.DrawSprite(sprite);
    }

    void Skips(Instruction_t instruction)
    {
        const auto x = GetNibble<1>(instruction);
        const auto kk = GetLowestByte(instruction);
        const auto isPressed = GetKeys()[V[x]];

        if(kk == 0x9E)
        {
            if(isPressed)
            {
                pc += 2;
            }
        }

        if(kk == 0xA1)
        {
            if(!isPressed)
            {
                pc += 2;
            }
        }
    }

    void Fxx(Instruction_t instruction)
    {
        const auto x = GetNibble<1>(instruction);

        switch(GetLowestByte(instruction))
        {
            case 0x07: V[x] = delayTimer.Count(); break;
            case 0x0A:
            {
                const auto k = GetKeys();
                const auto it = std::find(k.begin(), k.end(), true);

                if(it == k.cend())
                {
                    pc -= 2;
                    return;
                }

                V[x] = static_cast<Byte_t>(std::distance(k.cbegin(), it));

                break;
            }
            case 0x15: delayTimer.Set(V[x]); break;
            case 0x18: soundTimer.Set(V[x] + 1); break;
            case 0x1E: I += V[x]; break;
            case 0x29: I = V[x] * Address_t{5}; break;
            case 0x33: 
            {
                memory.WriteByte(V[x] / 100, I);
                memory.WriteByte((V[x] / 10) % 10, I + 1);
                memory.WriteByte((V[x] % 100) % 10, I + 2);
                break;
            }
            case 0x55:
            {
                memory.WriteChunk(std::vector(V.begin(), V.begin() + x + 1), I);
                I += static_cast<Address_t>(x + 1);
                break;
            }
            case 0x65:
            {
                const auto bytes = memory.ReadChunk(I, x + 1);
                std::copy(bytes.begin(), bytes.end(), V.begin());
                I += static_cast<Address_t>(x + 1);
                break;
            }
            default: break;
        }
    }

    const std::array<InstructionSetPtmf_t, nbInstructionSets> instructionSets
    {
        &Cpu::ClsRet,   // 0x00E0 CLS and 0x00EE RET
        &Cpu::Jp,       // 0x1NNN Jump to address NNN
        &Cpu::Call,     // 0x2
        &Cpu::SeByte,   // 0x3
        &Cpu::SneByte,  // 0x4
        &Cpu::SeReg,    // 0x5
        &Cpu::Ld,       // 0x6
        &Cpu::Add,      // 0x7
        &Cpu::Op,       // 0x8
        &Cpu::SneReg,   // 0x9
        &Cpu::SetI,     // 0xA
        &Cpu::JpV0,     // 0xB
        &Cpu::Rnd,      // 0xC
        &Cpu::Drw,      // 0xD
        &Cpu::Skips,    // 0xE
        &Cpu::Fxx,      // 0xF
    };


    static constexpr std::array<Byte_t, fontSetSize> fontSet   
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, //0
        0x20, 0x60, 0x20, 0x20, 0x70, //1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
        0x90, 0x90, 0xF0, 0x10, 0x10, //4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
        0xF0, 0x10, 0x20, 0x40, 0x40, //7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
        0xF0, 0x90, 0xF0, 0x90, 0x90, //A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
        0xF0, 0x80, 0x80, 0x80, 0xF0, //C
        0xE0, 0x90, 0x90, 0x90, 0xE0, //D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
        0xF0, 0x80, 0xF0, 0x80, 0x80  //F
    };

    Screen screen;
    KeyArray keys{};
    Timer<Byte_t> delayTimer;
    Timer<Byte_t> soundTimer;
    std::array<Address_t, stackSize> stack{};
    std::array<Register_t, nbRegisters> V{};
    Memory<ramSize> memory;
    Address_t I;
    Address_t pc{programLoadAddress};
    Byte_t sp{};
    bool cls{false};

    std::thread timersTask;
    std::atomic<bool> stopToken{false};

    mutable std::mutex keysMutex;
    mutable std::mutex screenMutex;

};