#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 255

int
main(int argc , char* argv[])
{
    char name[L_tmpnam];
    char line[MAXLINE];
    FILE *fp = NULL;

    printf("temp file name: %s\n", tmpnam(NULL));

    tmpnam(name);

    printf("%s\n", name);

    if(NULL == (fp = tmpfile())){
        printf("tmpfile error\n");
        exit(1);
    }
    fputs("one line of output\n", fp);
    rewind(fp);
    if(fgets(line, sizeof(line), fp) == NULL){
        printf("fgets error\n");
        exit(1);
    }
    fputs(line, stdout);
    exit(0);
}
