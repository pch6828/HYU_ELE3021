#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char* args[]){
    printf(1, "My pid is %d\nMy ppid is %d\n", getpid(), getppid());
    exit();
}
