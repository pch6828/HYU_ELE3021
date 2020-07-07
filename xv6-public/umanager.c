#include "types.h"
#include "user.h"
#include "fcntl.h"

void
panic(char* s){
    printf(2, "%s\n", s);
    exit();
}

int
fork1(void)
{
    int pid;

    pid = fork();
    if(pid == -1)
        panic("fork");
    return pid;
}


int main(void){
    static char username[20];
    static char password[20];
    char* argv[] = {"sh", 0};


    while(1){
        printf(2, "Username: ");
        memset(username, 0, sizeof(username));
        gets(username, sizeof(username));
        username[strlen(username)-1] = 0;
        printf(2, "Password: ");
        memset(password, 0, sizeof(password));
        gets(password, sizeof(password));
        password[strlen(password)-1] = 0;

        if(login(username, password)==0){
            if(fork1() == 0){
                exec("sh", argv);
            }
            wait();
        }else{
            printf(2, "Wrong Login Information\n");
        }
    }
}
