#ifndef ELF_PARSER_HPP
#define ELF_PARSER_HPP

#include <cstdint>
#include <string>
#include <vector>

struct TextSectionData {
    uint32_t virtual_address;          // Base address of .text (e.g., 0x80000000)
    std::vector<uint32_t> instructions; // Raw 32-bit machine instruction words
};

// Reads ELF headers, finds the .text section, and extracts binary instructions
TextSectionData parse_elf_text_section(const std::string& filepath);

#endif // ELF_PARSER_HPP