#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int main(int argc, char *argv[]){
    int mode = 0;
    if(argc < 3){
        printf(1, "Usage: p3_chmod mode target\n");
        printf(1, "mode: AB where A,B=[0,7]. A is for the owner and Bi for others.\n");
        printf(1, "  4: read\n");
        printf(1, "  2: write\n");
        printf(1, "  1: execute\n");
        exit();
    }

    if(strlen(argv[1]) != 2 || argv[1][0] < '0' || argv[1][0] > '7' || argv[1][1] < '0' || argv[1][1] > '7'){
        printf(1, "Invalid mode: %s\n", argv[1]);
        exit();
    } 

    argv[1][0] -= '0';
    argv[1][1] -= '0';

    if(argv[1][0] & 4)
        mode |= MODE_RUSR;
    if(argv[1][0] & 2)
        mode |= MODE_WUSR;
    if(argv[1][0] & 1)
        mode |= MODE_XUSR;
    if(argv[1][1] & 4)
        mode |= MODE_ROTH;
    if(argv[1][1] & 2)
        mode |= MODE_WOTH;
    if(argv[1][1] & 1)
        mode |= MODE_XOTH;

    if (chmod(argv[2], mode) == 0)
        printf(1, "chmod successful\n");
    else
        printf(1, "chmod failed!\n");

    exit();
}
