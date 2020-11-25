#ifndef _DYNAMIC_HPP_
#define _DYNAMIC_HPP_

#include <elf.h>

#include <string>
#include <set>

//std::vector<Elf64_Dyn *> search_dynamic_entries_by_type(uint8_t *head, Elf64_Sxword type);
std::set<std::string> get_shared_libraries_name(uint8_t *head);

#endif
