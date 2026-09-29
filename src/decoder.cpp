#include "decoder.hpp"
#include <iomanip>
#include <sstream>

// Standard RISC-V ABI Register Mapping
static const char* ABI_REG_NAMES[32] = {
    "zero", "ra", "sp",  "gp",  "tp", "t0", "t1", "t2",
    "s0",   "s1", "a0",  "a1",  "a2", "a3", "a4", "a5",
    "a6",   "a7", "s2",  "s3",  "s4", "s5", "s6", "s7",
    "s8",   "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

Instruction decode_instruction(uint32_t raw, uint32_t address) {
    Instruction inst{};
    inst.raw_bits = raw;
    inst.address = address;

    uint32_t opcode = raw & 0x7F;
    uint32_t rd     = (raw >> 7)  & 0x1F;
    uint32_t funct3 = (raw >> 12) & 0x07;
    uint32_t rs1    = (raw >> 15) & 0x1F;
    uint32_t rs2    = (raw >> 20) & 0x1F;
    uint32_t funct7 = (raw >> 25) & 0x7F;

    inst.rd_str  = ABI_REG_NAMES[rd];
    inst.rs1_str = ABI_REG_NAMES[rs1];
    inst.rs2_str = ABI_REG_NAMES[rs2];

    std::ostringstream ss;

    switch (opcode) {
        // OP-IMM (I-Type: addi, slli, etc.)
        case 0x13: {
            int32_t imm_i = static_cast<int32_t>(raw) >> 20; // Sign-extend 12 bits
            inst.imm = imm_i;

            if (funct3 == 0x0) inst.mnemonic = "addi";
            else if (funct3 == 0x1) inst.mnemonic = "slli";
            else if (funct3 == 0x4) inst.mnemonic = "xori";
            else if (funct3 == 0x6) inst.mnemonic = "ori";
            else if (funct3 == 0x7) inst.mnemonic = "andi";
            else inst.mnemonic = "unknown_op_imm";

            ss << inst.mnemonic << " " << inst.rd_str << ", " << inst.rs1_str << ", " << inst.imm;
            break;
        }

        // OP (R-Type: add, sub, mul, etc.)
        case 0x33: {
            if (funct3 == 0x0 && funct7 == 0x00) inst.mnemonic = "add";
            else if (funct3 == 0x0 && funct7 == 0x20) inst.mnemonic = "sub";
            else if (funct3 == 0x0 && funct7 == 0x01) inst.mnemonic = "mul"; // RV32M
            else if (funct3 == 0x4 && funct7 == 0x00) inst.mnemonic = "xor";
            else if (funct3 == 0x6 && funct7 == 0x00) inst.mnemonic = "or";
            else if (funct3 == 0x7 && funct7 == 0x00) inst.mnemonic = "and";
            else inst.mnemonic = "unknown_op";

            ss << inst.mnemonic << " " << inst.rd_str << ", " << inst.rs1_str << ", " << inst.rs2_str;
            break;
        }

        // LOAD (I-Type: lw, lb, etc.)
        case 0x03: {
            int32_t imm_i = static_cast<int32_t>(raw) >> 20;
            inst.imm = imm_i;

            if (funct3 == 0x2) inst.mnemonic = "lw";
            else inst.mnemonic = "unknown_load";

            ss << inst.mnemonic << " " << inst.rd_str << ", " << inst.imm << "(" << inst.rs1_str << ")";
            break;
        }

        // STORE (S-Type: sw, sb, etc.)
        case 0x23: {
            int32_t imm_s = ((static_cast<int32_t>(raw) >> 25) << 5) | ((raw >> 7) & 0x1F);
            // Sign-extend 12 bits
            imm_s = (imm_s << 20) >> 20;
            inst.imm = imm_s;

            if (funct3 == 0x2) inst.mnemonic = "sw";
            else inst.mnemonic = "unknown_store";

            ss << inst.mnemonic << " " << inst.rs2_str << ", " << inst.imm << "(" << inst.rs1_str << ")";
            break;
        }

        // BRANCH (B-Type: beq, bne, etc.)
        case 0x63: {
            int32_t imm_b = ((raw >> 31) << 12) |
                            (((raw >> 25) & 0x3F) << 5) |
                            (((raw >> 8) & 0x0F) << 1) |
                            (((raw >> 7) & 0x01) << 11);
            // Sign-extend 13 bits
            imm_b = (imm_b << 19) >> 19;
            inst.imm = imm_b;

            if (funct3 == 0x0) inst.mnemonic = "beq";
            else if (funct3 == 0x1) inst.mnemonic = "bne";
            else inst.mnemonic = "unknown_branch";

            ss << inst.mnemonic << " " << inst.rs1_str << ", " << inst.rs2_str << ", " << inst.imm;
            break;
        }

        // LUI (U-Type)
        case 0x37: {
            int32_t imm_u = static_cast<int32_t>(raw & 0xFFFFF000);
            inst.imm = imm_u;
            inst.mnemonic = "lui";
            ss << inst.mnemonic << " " << inst.rd_str << ", 0x" << std::hex << (imm_u >> 12);
            break;
        }

        default: {
            inst.mnemonic = "unknown";
            ss << ".word 0x" << std::hex << raw;
            break;
        }
    }

    inst.formatted_asm = ss.str();
    return inst;
}