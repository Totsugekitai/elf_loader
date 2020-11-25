#include <elf.h>

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <set>

#include "section.hpp"
#include "util.hpp"

std::vector<Elf64_Dyn *> search_dynamic_entries_by_type(uint8_t *head, Elf64_Sxword type) {
    std::vector<Elf64_Dyn *> dynvec;
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)head;

    Elf64_Shdr *dynamic_shdr = search_shdr_by_name(ehdr, ".dynamic");

    if (sizeof(Elf64_Dyn) != dynamic_shdr->sh_entsize) {
        fprintf(stderr, "Invalid dynamic section entry size (%ld)\n", dynamic_shdr->sh_entsize);
        return dynvec;
    }

    Elf64_Dyn *dynent = (Elf64_Dyn *)(head + dynamic_shdr->sh_offset);
    uint64_t ndynent = dynamic_shdr->sh_size / dynamic_shdr->sh_entsize;
    for (uint64_t i = 0; i < ndynent; i++) {
        if (dynent[i].d_tag == type) {
            dynvec.push_back(&dynent[i]);
        }
    }

    return dynvec;
}

std::set<std::string> get_shared_libraries_name(uint8_t *head) {
    std::set<std::string> shared_libraries_name;

    std::vector<Elf64_Dyn *> dynvec = search_dynamic_entries_by_type(head, DT_NEEDED);
    if (dynvec.empty()) {
        fprintf(stderr, "cannot find vector member\n");
        return shared_libraries_name;
    }

    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)head;
    Elf64_Shdr *dynstr_shdr = search_shdr_by_name(ehdr, ".dynstr");

    for (auto dynent : dynvec) {
        char *s = (char *)head + dynstr_shdr->sh_offset + dynent->d_un.d_val;
        if (strcmp(s, "ld-linux-x86-64.so.2") == 0) {
            continue;
        }
        std::string str = std::string("/lib/") + s;
        shared_libraries_name.insert(str);
    }
    if (shared_libraries_name.empty()) {
        return shared_libraries_name;
    }

    for (auto library : shared_libraries_name) {
        uint8_t *head = (uint8_t *)map_file(library.c_str());
        std::set<std::string> set = get_shared_libraries_name(head);
        if (!set.empty()) {
            shared_libraries_name.merge(set);
        }
    }

    return shared_libraries_name;
}
