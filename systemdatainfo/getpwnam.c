#include <stdio.h>
#include <pwd.h>
#include <string.h>
#include <stdlib.h>

struct passwd * getpwnam(const char* );

int
main(int argc, char argv[])
{
    struct passwd * pwptr;

    pwptr = getpwnam("root");

    printf("user name: %s\n", pwptr->pw_name);
    printf("pass word: %s\n", pwptr->pw_passwd);
    printf("user ID: %d\n", pwptr->pw_uid);
    printf("group ID: %d\n", pwptr->pw_gid);
    printf("real name: %s\n", pwptr->pw_gecos);
    printf("directory: %s\n", pwptr->pw_dir);
    printf("shell: %s\n", pwptr->pw_shell);

    exit(0);
}


struct passwd *
getpwnam(const char *name)
{
    struct passwd * ptr;

    setpwent();
    while( NULL!=(ptr = getpwent())){
        if(0 == strcmp(name, ptr->pw_name)){
            break;
        }
    }
    endpwent();
    return(ptr);
}
