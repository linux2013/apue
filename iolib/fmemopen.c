#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BSZ 48
int
main(int argc, char * argv[])
{
    FILE *fp = NULL;
    char buf[BSZ];

    memset(buf, 'a', BSZ-2);
    buf[BSZ - 2] = '\0';
    buf[BSZ - 1] = 'X';

    if(NULL == (fp = fmemopen(buf, BSZ, "w+"))){
        printf("fmemopen failed\n");
        exit(1);
    }
    
    printf("Initial buffer contents: %s\n", buf);
    fprintf(fp, "Hello, world\n");
    printf("before flush: %s\n", buf);
    fflush(fp);
    printf("after fflush: %s\n", buf);
    printf("len of string in buf = %ld\n", (long)strlen(buf));

    exit(0);
    
}
