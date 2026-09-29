#ifndef DECODER_HPP
#define DECODER_HPP

#include <cstdint>
#include <string>

struct Instruction {
    uint32_t raw_bits;
    uint32_t address;
    std::string mnemonic;
    std::string rd_str;
    std::string rs1_str;
    std::string rs2_str;
    int32_t imm;
    std::string formatted_asm;
};

// Decodes a raw 32-bit RISC-V instruction word into human-readable assembly
Instruction decode_instruction(uint32_t raw_inst, uint32_t address = 0);

#endif // DECODER_HPP