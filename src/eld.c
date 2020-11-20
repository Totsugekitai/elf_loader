#include <elf.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "section.h"
#include "segment.h"

extern uint64_t _start_empty_region;
extern uint64_t _end_empty_region;

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

void *load_file(uint8_t *head) {
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)head;
    if (ehdr->e_type != ET_EXEC) {
        fprintf(stderr, "Invalid file type\n");
        return NULL;
    }
    for (int i = 0; i < ehdr->e_phnum; i++) {
        Elf64_Phdr *phdr = get_phdr(ehdr, i);
        if (!memcpy_segment(phdr, head)) {
            fprintf(stderr, "Failed to memcpy segment (%d)\n", i);
            return NULL;
        }
    }
    if (!clear_bss(ehdr)) {
        return NULL;
    }
    return (uintptr_t *)ehdr->e_entry;
}

int main(int argc, char *argv[]) {
    printf("========== loader start ==========\n");
    static char **stackp;
    int fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "cannot open file (%s)\n", argv[1]);
        return 1;
    }
    printf("empty region start (%p)\n", (void *)&_start_empty_region);
    printf("empty region end   (%p)\n", (void *)&_end_empty_region);

    struct stat sb;
    fstat(fd, &sb);
    int64_t pagesize = sysconf(_SC_PAGESIZE);
    uint32_t len_mmap = ((sb.st_size + (pagesize - 1)) / pagesize) * pagesize;
    uint8_t *head = (uint8_t *)mmap(NULL, len_mmap, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (head == MAP_FAILED) {
        fprintf(stderr, "failed to mmap\n");
        return 1;
    }
    void *f = load_file(head);
    if (f == NULL) {
        fprintf(stderr, "fail to load file\n");
        return 1;
    }
    close(fd);

    argv[0] = (char *)(uint64_t)(argc - 1);
    stackp = &argv[0];
    __asm__ volatile("mov %0, %%rsp;" :: "m"(stackp));
    printf("==========  loader end  ==========\n\n");
    __asm__ volatile("mov %0, %%rax;"
                     "jmp *%%rax"
                     ::"m"(f));
    return 1;
}
