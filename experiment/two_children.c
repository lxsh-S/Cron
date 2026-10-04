#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int pipefd[2];
  pipe(pipefd);

  pid_t ls_pid = fork();

  if (ls_pid == 0) {
    // CHILD
    close(pipefd[0]);
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);

    execlp("ls", "ls", NULL);

    return 1;
  }

  pid_t grep_pid = fork();

  if (grep_pid == 0) {
    close(pipefd[1]);

    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);

    execlp("grep", "grep", ".c", NULL);

    return 1;
  }

  // So now Cron remains parent
  close(pipefd[0]);
  close(pipefd[1]);

  waitpid(ls_pid, NULL, 0);
  waitpid(grep_pid, NULL, 0);

  printf("Both commands finished!!\n");

  return 0;
}
