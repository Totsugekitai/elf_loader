#include <elf.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <vector>
#include <set>
#include <string>
#include <cstdio>
#include <cstring>

#include "dynamic.hpp"
#include "section.hpp"
#include "segment.hpp"
#include "object.hpp"
#include "util.hpp"

extern uint64_t _start_empty_region;
extern uint64_t _end_empty_region;

void *load_dyn_file(uint8_t *head) {
    //Elf64_Ehdr *ehdr = (Elf64_Ehdr *)head;
    std::set<std::string> files = get_shared_libraries_name(head);
    for (auto file : files) {
        printf("%s\n", file.c_str());
    }
    std::exit(0);
}

void *load_file(uint8_t *head) {
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)head;
    if (ehdr->e_type == ET_EXEC) {
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
    } else if (ehdr->e_type == ET_DYN) {
        return load_dyn_file(head);
    }
    fprintf(stderr, "Invalid file type\n");
    return NULL;
}

int main(int argc, char *argv[]) {
    printf("========== loader start ==========\n");
    static char **stackp;
    printf("open file [%s]\n", argv[1]);
    uint8_t *head = (uint8_t *)map_file(argv[1]);
    void *f = load_file(head);
    if (f == NULL) {
        fprintf(stderr, "fail to load file\n");
        return 1;
    }

    argv[0] = (char *)(uint64_t)(argc - 1);
    stackp = &argv[0];
    printf("==========  loader end  ==========\n");

    __asm__ volatile("mov %0, %%rsp;" :: "m"(stackp));
    __asm__ volatile(
        "mov %0, %%rax;"
        "jmp *%%rax" ::"m"(f));
    return 1;
}
