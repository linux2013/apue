#include <shadow.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char * argv[])
{
    struct spwd * ptrspwd;

    setspent();
    while(NULL != (ptrspwd = getspent())){
        printf("login name: %s\n", ptrspwd->sp_namp);
        printf("encrypted password: %s\n", ptrspwd->sp_pwdp);
        printf("data of last change: %d\n", ptrspwd->sp_lstchg);
        printf("minimum number of days between changes: %d\n", ptrspwd->sp_min);
        printf("maximun number of days between changes: %d\n", ptrspwd->sp_max);
        printf("number of days to warn user to change the password: %d\n", ptrspwd->sp_warn);
        printf("number of days the account may be inactive: %d\n", ptrspwd->sp_inact);
        printf("number of days since 1970-01-01 until account expires: %d\n", ptrspwd->sp_expire);
        printf("reserved: %d\n", ptrspwd->sp_flag);
    }
    
    endspent(); 
}

