#ifndef _SYMBOL_HPP_
#define _SYMBOL_HPP_

#include <string>

typedef struct symbol {
    std::string symbol_name;
    void *address;
} symbol_t;

#endif
