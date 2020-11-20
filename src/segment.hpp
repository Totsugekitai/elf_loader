#ifndef _SEGMENT_HPP_
#define _SEGMENT_HPP_

#include <elf.h>

Elf64_Phdr *get_phdr(Elf64_Ehdr *ehdr, int num);
bool memcpy_segment(Elf64_Phdr *phdr, uint8_t *head);

#endif
