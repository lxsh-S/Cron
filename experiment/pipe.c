#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int pipefd[2];
  pipe(pipefd);

  pid_t pid = fork();

  if (pid == 0) {
    // CHILD - writes
    close(pipefd[0]);

    char messages[] = "Hello from the CHILD!";

    write(pipefd[1], messages, strlen(messages) + 1);

    close(pipefd[1]);
  } else {
    // PARENT - reads
    close(pipefd[1]);

    char buffer[1024];

    read(pipefd[0], buffer, sizeof(buffer));

    printf("Parent received: %s\n", buffer);

    close(pipefd[0]);

    wait(NULL);
  }

  return 0;
}
