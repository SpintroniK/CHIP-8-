#pragma once

#include "Defintions.hpp"
#include "Instructions.hpp"
#include "Memory.hpp"

#include <array>
#include <iostream>


class Cpu
{

public:

    Cpu()
    {
        memory.WriteChunk(fontSet, fontOffset);
    }

    ~Cpu() = default;

    auto GetCls() const { return cls; }
    void ResetCls() { cls = false; }


private:

    void ClsRet(Instruction_t instruction)
    {
        if(instruction == OpCode::Cls)
        {
            cls = true;         // Clear screen
        }

        if(instruction == OpCode::Ret)
        {
            pc = stack[--sp];   // Return from subroutine
        }
    }

    void Jp(Instruction_t instruction)
    {

    }

    void Call(Instruction_t instruction)
    {

    }

    void SeByte(Instruction_t instruction)
    {

    }

    void SneByte(Instruction_t instruction)
    {

    }

    void SeReg(Instruction_t instruction)
    {

    }

    void Ld(Instruction_t instruction)
    {

    }

    void Add(Instruction_t instruction)
    {

    }

    void Op(Instruction_t instruction)
    {

    }

    void SneReg(Instruction_t instruction)
    {

    }

    void SetI(Instruction_t instruction)
    {

    }

    void JpV0(Instruction_t instruction)
    {

    }

    void Rnd(Instruction_t instruction)
    {

    }

    void Drw(Instruction_t instruction)
    {

    }

    void Skips(Instruction_t instruction)
    {

    }

    void Fxx(Instruction_t instruction)
    {

    }

    const std::array<InstructionSetPtmf_t, nbInstructionSets> instructionSets
    {
        &Cpu::ClsRet,   // 0x00E0 CLS and 0x00EE RET
        &Cpu::Jp,       // 0x1
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
    
    Memory<ramSize> memory;
    std::array<Address_t, stackSize> stack{};
    std::array<Register_t, nbRegisters> V{};
    Address_t pc{};
    Byte_t sp{};
    bool cls{false};
};