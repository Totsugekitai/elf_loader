#ifndef _SECTION_HPP_
#define _SECTION_HPP_

#include <elf.h>

Elf64_Shdr *get_shdr(Elf64_Ehdr *ehdr, int num);
Elf64_Shdr *search_shdr_by_name(Elf64_Ehdr *ehdr, const char *name);

#endif
