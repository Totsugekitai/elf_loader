#include <elf.h>

#include <cstdio>
#include <cstring>

Elf64_Phdr *get_phdr(Elf64_Ehdr *ehdr, int num) {
    if ((num < 0) || (num >= ehdr->e_phnum)) {
        fprintf(stderr, "cannot find phdr (%d)\n", num);
        return NULL;
    }
    Elf64_Phdr *phdr =
        (Elf64_Phdr *)((char *)ehdr + ehdr->e_phoff + ehdr->e_phentsize * num);
    return phdr;
}

bool memcpy_segment(Elf64_Phdr *phdr, uint8_t *head) {
    if (phdr == NULL) {
        fprintf(stderr, "Invalid program header pointer\n");
        return false;
    }
    switch (phdr->p_type) {
    case PT_LOAD:
        printf("Type: LOAD");
        memcpy((uint8_t *)phdr->p_vaddr, head + phdr->p_offset, phdr->p_filesz);
        printf(" (loaded %p, from %p, size 0x%lx)\n",
                (uint8_t *)phdr->p_vaddr, head + phdr->p_offset, phdr->p_filesz);
        break;
    default:
        printf("Type: OTHER\n");
        break;
    }
    return true;
}

