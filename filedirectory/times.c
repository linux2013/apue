#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int 
main(int argc, char* argv[])
{
    int i;
    int fd;
    struct stat statbuf;
    struct timespec times[2];

    for(i = 1; i < argc; i++){
        if(stat(argv[i], &statbuf) < 0){
            perror("stat");
            exit(1);
        }

        if(fd = open(argv[i], O_RDWR|O_TRUNC) < 0){
            perror("open error");
            exit(1);
        }
        times[0] = statbuf.st_atim;
        times[1] = statbuf.st_mtim;

        if(futimens(fd, times) < 0){
            perror("futimes");
            exit(1);
        }
        close(fd);
    }

    exit(0);

}
