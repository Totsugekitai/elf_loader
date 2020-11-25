#include <elf.h>

#include <cstdio>
#include <cstring>
#include <vector>

#include "symbol.hpp"
#include "object.hpp"
#include "section.hpp"

symbol_t search_symbol_by_name(std::vector<object_t> &objs, const char *name) {
    symbol_t target_sym = { NULL, NULL };
    for (auto obj : objs) {
        Elf64_Ehdr *ehdr = (Elf64_Ehdr *)obj.address;
        Elf64_Shdr *symtab = (Elf64_Shdr *)search_shdr_by_name(ehdr, ".symtab");
        Elf64_Shdr *strtab = (Elf64_Shdr *)search_shdr_by_name(ehdr, ".strtab");
        uint8_t *head = (uint8_t *)ehdr;

        if (symtab->sh_entsize != sizeof(Elf64_Sym)) {
            fprintf(stderr, "Invalid entry size (%ld)\n", symtab->sh_entsize);
            return target_sym;
        }

        for (uint64_t i = 0; i < symtab->sh_size; i += symtab->sh_entsize) {
            Elf64_Sym *symbol = (Elf64_Sym *)(head + symtab->sh_offset + i);
            if (!symbol->st_name) continue;
            if ((symbol->st_shndx == SHN_UNDEF) || (symbol->st_shndx ==SHN_ABS)) continue;
            if (ELF64_ST_BIND(symbol->st_info) != STB_GLOBAL) continue;
            if ((ELF64_ST_TYPE(symbol->st_info) != STT_NOTYPE) &&
                (ELF64_ST_TYPE(symbol->st_info) != STT_OBJECT) &&
                (ELF64_ST_TYPE(symbol->st_info) != STT_FUNC))
                continue;

            if (strcmp(name, (char *)head + strtab->sh_offset + symbol->st_name) == 0) {
                Elf64_Shdr *shdr = (Elf64_Shdr *)(head + ehdr->e_shoff + ehdr->e_shentsize * symbol->st_shndx);
                target_sym.symbol_name = name;
                target_sym.address = head + shdr->sh_offset + symbol->st_value;
                return target_sym;
            }
        }
    }

    fprintf(stderr, "cannot find symbol %s\n", name);
    return target_sym;
}

bool relocate_common_symbol(Elf64_Ehdr *ehdr)
{
    Elf64_Shdr *symtab = search_shdr_by_name(ehdr, ".symtab");
    Elf64_Shdr *bss = search_shdr_by_name(ehdr, ".bss");
    uint8_t *head = (uint8_t *)ehdr;

    if (symtab->sh_entsize != sizeof(Elf64_Sym)) {
        fprintf(stderr, "Invalid entry size (%ld)\n", symtab->sh_entsize);
        return false;
    }

    int n = bss->sh_size;
    for (uint64_t i = 0; i < symtab->sh_size; i += symtab->sh_entsize) {
        Elf64_Sym *symbol = (Elf64_Sym *)(head + symtab->sh_offset + i);
        if (symbol->st_shndx != SHN_COMMON) continue;
        // COMMONシンボルではst_valueはアライメントサイズを表すらしいので
        // st_valueでアライメントする
        n = (n + symbol->st_value - 1) & ~(symbol->st_value - 1);
        symbol->st_shndx = bss - (Elf64_Shdr *)(head + ehdr->e_shoff);
        symbol->st_value = n;
        n += symbol->st_size;
    }

    // 16byteアライメント
    n = (n + 15) & ~15;
    bss->sh_size = n;

    return true;
}
