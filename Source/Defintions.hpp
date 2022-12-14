#pragma once

#include <cstdint>
#include <limits>

using Byte_t = std::uint8_t;
using Address_t = std::uint16_t;

// Instructions
class Cpu; // Forward declaration

using Instruction_t = Address_t;
using InstructionSetPtmf_t = void(Cpu::*)(Instruction_t);

static constexpr std::uint8_t nbInstructionSets = 16;

// RAM
static constexpr std::uint16_t virtualRamSize = 0x0FFF;
static constexpr std::uint16_t programLoadAddress = 0x0200;
static constexpr std::uint16_t fontSetSize = 16 * 5;
// static constexpr std::uint16_t programOffset = programLoadAddress - fontSetSize;
static constexpr std::uint16_t ramSize = virtualRamSize; // Remove previous line because program starts @ 0x0200

// Registers
using Register_t = Byte_t;

static constexpr std::uint8_t nbRegisters = 16;

// Stack
static constexpr std::uint8_t stackSize = 16;

// Font
static constexpr Address_t fontOffset = 0;

// Display
static constexpr Byte_t screenWidth = 64;
static constexpr Byte_t screenHeight = 32;
static constexpr Byte_t spriteWidth = std::numeric_limits<Byte_t>::digits;
