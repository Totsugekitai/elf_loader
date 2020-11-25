#ifndef _OBJECT_HPP_
#define _OBJECT_HPP_

#include <string>

typedef struct object {
    std::string filename;
    void *address;
} object_t;

#endif
