#include "types.h"
#include "user.h"
#include "fcntl.h"

#define LIST    1
#define KILL    2
#define EXEC    3
#define MEMLIM  4

#define MAXARGS 10

struct cmd {
    int type;
};

struct execcmd {
    int type;
    char *argv[MAXARGS];
    int stacksize;
};

struct killcmd {
    int type;
    int pid;
};

struct memlimcmd {
    int type;
    int pid;
    int limit;
};

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

void
runcmd(struct cmd *cmd) {
    struct execcmd* ecmd;
    struct killcmd* kcmd;
    struct memlimcmd* mcmd;

    if(cmd == 0){
        exit();
    }

    switch(cmd->type){
        case EXEC:
            ecmd = (struct execcmd*)cmd;
           if(fork1() == 0){
             if(ecmd->argv[0] == 0){
                printf(2, "exec failed : no argv\n");
                break;
             }
                exec2(ecmd->argv[0], ecmd->argv, ecmd->stacksize);
                //exec(ecmd->argv[0], ecmd->argv);
                printf(2, "exec %s failed\n", ecmd->argv[0]);
            }
            break;
        case KILL:
            kcmd = (struct killcmd*)cmd;
            if(kill(kcmd->pid)==0){
                printf(2, "kill %d successed\n", kcmd->pid);
            }else{
                printf(2, "kill %d failed\n", kcmd->pid);
            }
            break;
        case MEMLIM:
            mcmd = (struct memlimcmd*)cmd;
            if(setmemorylimit(mcmd->pid, mcmd->limit)==0){
                printf(2, "setmemorylimit pid %d limit %d successed\n", mcmd->pid, mcmd->limit);
            }
            else{
                printf(2, "setmemorylimit pid %d limit %d failed\n", mcmd->pid, mcmd->limit);
            }
            break;
        default:
            panic("runcmd");
    }
}

int getcmd(char *buf, int nbuf)
{
  printf(2, "> ");
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  if(buf[0] == 0){
    return -1;
  }
  return 0;
}

struct cmd* parsecmd(char* s)
{
    char* type = malloc(200);
    memset(type, 0, 200);
    char* t = type;
    while((*t++ = *s++) != ' ');

    type[strlen(type)-1] = 0;

    if(strcmp(type, "execute")==0){
        struct execcmd* cmd = malloc(sizeof(struct execcmd));
        memset(cmd, 0, sizeof(struct execcmd));
        cmd->type = EXEC;
        cmd->argv[0] = malloc(60);
        memset(cmd->argv[0], 0, 60);
        t = cmd->argv[0];
        while((*t++ = *s++) != ' ');
        cmd->argv[0][strlen(cmd->argv[0])-1] = 0;
        cmd->stacksize = atoi(s);
        return (struct cmd*)cmd;
    }else if(strcmp(type, "memlim")==0){
        struct memlimcmd* cmd = malloc(sizeof(struct memlimcmd));
        memset(cmd, 0, sizeof(struct memlimcmd));
        cmd->type = MEMLIM;
        char* pid = malloc(20);
        memset(pid, 0, 20);
        t = pid;
        while((*t++ = *s++) != ' ');
        pid[strlen(pid)-1] = 0;

        cmd->pid = atoi(pid);
        cmd->limit = atoi(s);
        return (struct cmd*)cmd;
    }else if(strcmp(type, "kill")==0){
        struct killcmd* cmd = malloc(sizeof(struct killcmd));
        memset(cmd, 0, sizeof(struct killcmd));
        cmd->type = KILL;
        cmd->pid = atoi(s);
        return (struct cmd*)cmd;
    }
    
    return 0;
}

int
main(void)
{
    static char buf[200];
    getadmin("2018008395");
    int fd;

    while((fd = open("console", O_RDWR))>=0){
        if(fd>=3){
            close(fd);
            break;
        }
    }
    
    printf(2, "[PROCESS MANAGER]\n\n");
    while(getcmd(buf, sizeof(buf)) >= 0){
        buf[strlen(buf)-1] = 0;
        if(strcmp(buf, "exit")==0){
            while(wait()!=-1);
            exit();
        }else if(strcmp(buf, "list")==0){
            printallproc();
            continue;
        }
        runcmd(parsecmd(buf));
    }
    exit();
}
