// init: The initial user-level program

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/spinlock.h"
#include "kernel/sleeplock.h"
#include "kernel/fs.h"
#include "kernel/file.h"
#include "user/user.h"
#include "kernel/fcntl.h"

char *argv[] = { "sh", 0 };

void setup_dev (void)
{
  

  if(open("/dev/console", O_RDWR) < 0){
    mknod("/dev/console", DEV_CONSOLE, 1);
    open("/dev/console", O_RDWR);
  }
  
  if(open("/dev/null", O_RDWR) < 0)
    mknod("/dev/null", DEV_NULL, 1);

  if(open("/dev/zero", O_RDWR) < 0)
    mknod("/dev/zero", DEV_ZERO, 1);
}

int main(void)
{
  setup_devices();
  int pid, wpid;
  
  dup(0);  // stdout
  dup(0);  // stderr

  for(;;){
    printf("init: starting sh\n");
    pid = fork();
    if(pid < 0){
      printf("init: fork failed\n");
      exit(1);
    }
    if(pid == 0){
      exec("sh", argv);
      printf("init: exec sh failed\n");
      exit(1);
    }

    for(;;){
      // this call to wait() returns if the shell exits,
      // or if a parentless process exits.
      wpid = wait((int *) 0);
      if(wpid == pid){
        // the shell exited; restart it.
        break;
      } else if(wpid < 0){
        printf("init: wait returned an error\n");
        exit(1);
      } else {
        // it was a parentless process; do nothing.
      }
    }
  }
}
