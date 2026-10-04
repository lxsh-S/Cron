#include <sys/wait.h>
#include <unistd.h>

int main() {

  int pipefd[2];
  pipe(pipefd);

  pid_t pid = fork();

  if (pid == 0) {
    // Child -> run "ls"
    close(pipefd[0]); // We dont wanna read

    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);

    execlp("ls", "ls", NULL);
  } else {
    // Parent -> run grep
    close(pipefd[1]);

    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);

    execlp("grep", "grep", ".c", NULL);
    wait(NULL);
  }
}
