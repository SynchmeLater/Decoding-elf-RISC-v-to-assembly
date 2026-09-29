#include "parser.hpp"
#include <cstring>
#include <elf.h>
#include <fstream>
#include <stdexcept>

TextSectionData parse_elf_text_section(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open ELF file: " + filepath);
    }

    // 1. Read ELF Header
    Elf32_Ehdr ehdr;
    file.read(reinterpret_cast<char*>(&ehdr), sizeof(ehdr));

    if (ehdr.e_ident[EI_MAG0] != ELFMAG0 || ehdr.e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr.e_ident[EI_MAG2] != ELFMAG2 || ehdr.e_ident[EI_MAG3] != ELFMAG3) {
        throw std::runtime_error("Invalid file format: Not a valid ELF file.");
    }

    if (ehdr.e_ident[EI_CLASS] != ELFCLASS32) {
        throw std::runtime_error("Unsupported ELF format: Expected 32-bit ELF.");
    }

    // 2. Read Section Header Table
    std::vector<Elf32_Shdr> shdrs(ehdr.e_shnum);
    file.seekg(ehdr.e_shoff, std::ios::beg);
    file.read(reinterpret_cast<char*>(shdrs.data()), ehdr.e_shnum * sizeof(Elf32_Shdr));

    // 3. Read Section Header String Table (.shstrtab)
    if (ehdr.e_shstrndx >= ehdr.e_shnum) {
        throw std::runtime_error("Invalid string table section index in header.");
    }
    Elf32_Shdr strtab_hdr = shdrs[ehdr.e_shstrndx];
    std::vector<char> strtab(strtab_hdr.sh_size);
    file.seekg(strtab_hdr.sh_offset, std::ios::beg);
    file.read(strtab.data(), strtab_hdr.sh_size);

    // 4. Locate .text Section Header
    Elf32_Shdr text_hdr{};
    bool found = false;
    for (uint16_t i = 0; i < ehdr.e_shnum; ++i) {
        const char* name = &strtab[shdrs[i].sh_name];
        if (std::strcmp(name, ".text") == 0) {
            text_hdr = shdrs[i];
            found = true;
            break;
        }
    }

    if (!found) {
        throw std::runtime_error("Missing section: .text section not found.");
    }

    // 5. Extract Raw 32-bit Instruction Words
    size_t word_count = text_hdr.sh_size / sizeof(uint32_t);
    std::vector<uint32_t> instructions(word_count);
    file.seekg(text_hdr.sh_offset, std::ios::beg);
    file.read(reinterpret_cast<char*>(instructions.data()), text_hdr.sh_size);

    return { text_hdr.sh_addr, instructions };
}