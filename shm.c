#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#define SHM_NAME "/myshm"
int main(){
    int fd = shm_open(SHM_NAME, O_CREAT|O_RDWR, 0666);
    ftruncate(fd, 64);
    char *ptr = mmap(0, 64, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    strcpy(ptr, "Shared: Hello!");
    printf("Written to shared memory: %s\n", ptr);
    munmap(ptr, 64);
    shm_unlink(SHM_NAME);
    return 0;
}