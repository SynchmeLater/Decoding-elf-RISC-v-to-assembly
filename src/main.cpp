#include "parser.hpp"
#include "decoder.hpp"
#include <iostream>
#include <iomanip>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./decoder <path_to_elf>\n";
        return 1;
    }

    try {
        TextSectionData elf_data = parse_elf_text_section(argv[1]);
        uint32_t pc = elf_data.virtual_address;

        std::cout << "Disassembly of section .text:\n\n";

        for (uint32_t raw_inst : elf_data.instructions) {
            Instruction decoded = decode_instruction(raw_inst, pc);

            std::cout << "  " << std::hex << std::setw(8) << std::setfill('0') << pc << ":  "
                      << std::setw(8) << raw_inst << "    "
                      << decoded.formatted_asm << "\n";

            pc += 4;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}