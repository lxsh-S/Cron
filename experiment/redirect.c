#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
  int fd = open("text.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

  if (fd == -1) {
    perror("open");
    return 1;
  }

  printf("File descriptor: %d\n", fd);

  close(fd);

  return 0;
}
