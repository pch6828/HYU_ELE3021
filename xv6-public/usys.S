#include "syscall.h"
#include "traps.h"

#define SYSCALL(name) \
  .globl name; \
  name: \
    movl $SYS_ ## name, %eax; \
    int $T_SYSCALL; \
    ret

SYSCALL(fork)
SYSCALL(exit)
SYSCALL(wait)
SYSCALL(pipe)
SYSCALL(read)
SYSCALL(write)
SYSCALL(close)
SYSCALL(kill)
SYSCALL(exec)
SYSCALL(open)
SYSCALL(mknod)
SYSCALL(unlink)
SYSCALL(fstat)
SYSCALL(link)
SYSCALL(mkdir)
SYSCALL(chdir)
SYSCALL(dup)
SYSCALL(getpid)
SYSCALL(sbrk)
SYSCALL(sleep)
SYSCALL(uptime)
SYSCALL(myfunction)
SYSCALL(getppid)
SYSCALL(yield)
SYSCALL(getlev)
SYSCALL(setpriority)
SYSCALL(getadmin)
SYSCALL(exec2)
SYSCALL(setmemorylimit)
SYSCALL(getshmem)
SYSCALL(printallproc)
SYSCALL(loaduserinfo)
SYSCALL(login)
SYSCALL(logout)
SYSCALL(useradd)
SYSCALL(userdel)
SYSCALL(chmod)
