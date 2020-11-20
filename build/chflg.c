#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <sys/mman.h>
#include <sys/stat.h>

int chflg(char *head)
{
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)head;

    for (int i = 0; i < ehdr->e_phnum; i++) {
        Elf64_Phdr *phdr = (Elf64_Phdr *)((char *)ehdr + ehdr->e_phoff + ehdr->e_phentsize * i);
        phdr->p_flags = PF_R | PF_W | PF_X;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "cannot open file (%s)\n", argv[1]);
        return 1;
    }
    struct stat sb;
    fstat(fd, &sb);
    char *head = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (chflg(head) < 0) {
        fprintf(stderr, "fail to change flags\n");
    }
    close(fd);
    return 0;
}
