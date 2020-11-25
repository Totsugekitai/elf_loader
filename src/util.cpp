#include <unistd.h>
#include <limits.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>

void *map_file(const char *file) {
    char *path = realpath(file, NULL);
    if (path == NULL) {
        fprintf(stderr, "cannot find true file path (%s)\n", file);
        std::exit(1);
    }
    int fd = open(path, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "cannot open file (%s)\n", path);
        std::exit(1);
    }

    struct stat sb;
    fstat(fd, &sb);
    int64_t pagesize = sysconf(_SC_PAGESIZE);
    uint32_t len_mmap = ((sb.st_size + (pagesize - 1)) / pagesize) * pagesize;
    uint8_t *head = (uint8_t *)mmap(NULL, len_mmap, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (head == MAP_FAILED) {
        fprintf(stderr, "failed to mmap\n");
        std::exit(1);
    }
    close(fd);
    return head;
}
