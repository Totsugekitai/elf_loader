#include <elf.h>

#include <cstdio>
#include <cstring>

Elf64_Shdr *get_shdr(Elf64_Ehdr *ehdr, int num) {
    if ((num < 0) || (num >= ehdr->e_shnum)) {
        fprintf(stderr, "cannot find shdr (%d)\n", num);
        return NULL;
    }
    Elf64_Shdr *shdr = (Elf64_Shdr *)((char *)ehdr + ehdr->e_shoff + ehdr->e_shentsize * num);
    return shdr;
}

Elf64_Shdr *search_shdr_by_name(Elf64_Ehdr *ehdr, const char *name) {
    Elf64_Shdr *nhdr = get_shdr(ehdr, ehdr->e_shstrndx);
    for (int i = 0; i < ehdr->e_shnum; i++) {
        Elf64_Shdr *shdr = get_shdr(ehdr, i);
        if (strcmp((char *)ehdr + nhdr->sh_offset + shdr->sh_name, name) == 0) {
            fprintf(stderr, "found %s\n", name);
            return shdr;
        }
    }
    fprintf(stderr, "cannot found shdr %s\n", name);
    return NULL;
}

bool clear_bss(Elf64_Ehdr *ehdr) {
    const char *bss = ".bss";
    Elf64_Shdr *shdr = search_shdr_by_name(ehdr, bss);
    if (shdr) {
        printf("clear BSS: 0x%08lx, 0x%08lx\n", shdr->sh_addr, shdr->sh_size);
        memset((char *)shdr->sh_addr, 0, shdr->sh_size);
        printf("clear BSS complete\n");
        return true;
    } else {
        fprintf(stderr, ".bss section not found\n");
        return false;
    }
}

