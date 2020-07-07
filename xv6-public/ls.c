#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }
  char permission[10] = "-rwxrwx";
  if(st.type == 2){
    permission[0] = '-';
  }else if(st.type == 1){
    permission[0] = 'd';
  }else{
    permission[0] = 'c';
  }
  
  for(int i = 0; i < 6; i++){
    int idx = 6-i;
    if((st.permission & (1<<i))==0){
      permission[idx] = '-';
    }
    if(permission[0] == 'c'){
      permission[idx] = '-';
    }
  }
  switch(st.type){
  case T_FILE:
    printf(1, "%s %s %s %d %d %d\n", fmtname(path), permission, st.owner, st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }
      char permission[10] = "-rwxrwx";
      if(st.type == 2){
        permission[0] = '-';
      }else if(st.type == 1){
        permission[0] = 'd';
      }else{
        permission[0] = 'c';
      }
  
      for(int i = 0; i < 6; i++){
        int idx = 6-i;
        if((st.permission & (1<<i))==0){
          permission[idx] = '-';
        }
        if(permission[0] == 'c'){
          permission[idx] = '-';
        }
      }
 
      printf(1, "%s %s %s %d %d %d\n", fmtname(buf), permission, st.owner, st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    ls(".");
    exit();
  }
  for(i=1; i<argc; i++)
    ls(argv[i]);
  exit();
}
