#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char* argv[]){
    int ptype = fork();

    while(1){
        if(ptype){
            printf(1, "Parent\n");
        }else{
            printf(1, "Child\n");
        }
        yield();
    }
}
