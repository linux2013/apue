#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>

void make_temp(char *);

int
main(int argc, char * argv[])
{
    char good_template[] = "/tmp/dirXXXXXX";

    printf("try to create first temp file\n");
    make_temp(good_template);
    printf("%s\n", good_template);    
    exit(0);
}

void 
make_temp(char * template)
{
    int fd;
    struct stat sbuf;

    if((fd = mkstemp(template)) < 0){
        printf("can't create temp file\n");
        exit(1);
    }
    close(fd);

    if(stat(template, &sbuf) < 0){
        if(errno == ENOENT){
            printf("file does't exist\n");
        }else{
            printf("stat error\n");
        }
        exit(1);
    }else{
        printf("file exists\n");
        unlink(template);
    }
}
