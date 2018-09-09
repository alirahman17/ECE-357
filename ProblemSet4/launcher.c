#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/wait.h>

int fd1[2], fd2[2], i, status;
pid_t pid1, pid11, pid2, pid21, pid3, pid31;
struct rusage ru;

int main(int argc, char ** argv[]){

  if (pipe(fd1) < 0) {
      fprintf(stderr, "CRITICAL ERROR: Failed to create pipe 1: %s\n", strerror(errno));
      return(-1);
  }
  if (pipe(fd2) < 0) {
      fprintf(stderr, "CRITICAL ERROR: Failed to create pipe 2: %s\n", strerror(errno));
      return(-1);
  }

  if ((pid1 = fork()) < 0) {
      fprintf(stderr, "CRITICAL ERROR: Unable to fork process 1: %s\n", strerror(errno));
      return(-1);
  } else if (pid1 == 0) {
      if (dup2(fd1[1], 1) < 0) {
        fprintf(stderr, "ERROR: Unable to dup to pipe: %s\n", strerror(errno));
        return(-1);
      }
      close(fd1[0]);
      close(fd1[1]);
      close(fd2[0]);
      close(fd2[1]);
      if (execlp("./wordgen.out", "wordgen.out", argv[1], (char *)NULL) == -1) {
          fprintf(stderr, "CRITICAL ERROR: Unable to execute wordgen.c: %s\n", strerror(errno));
      }
      exit(1);
  } else {
      if ((pid2 = fork()) < 0) {
          fprintf(stderr, "CRITICAL ERROR: Unable to fork process 2: %s\n", strerror(errno));
          return(-1);
      } else if (pid2 == 0) {
          if (dup2(fd1[0], 0) < 0) {
              fprintf(stderr, "ERROR: Unable to dup to pipe: %s\n", strerror(errno));
              return(-1);
          }
          if (dup2(fd2[1], 1) < 0) {
              fprintf(stderr, "ERROR: Unable to dup to pipe: %s\n", strerror(errno));
              return(-1);
          }
          close(fd1[0]);
          close(fd1[1]);
          close(fd2[0]);
          close(fd2[1]);
          if (execlp("./wordsearch.out", "wordsearch.out", "word.txt", (char *)NULL) == -1) {
              fprintf(stderr, "CRITICAL ERROR: Unable to execute wordsearch.c: %s\n", strerror(errno));
          }
          exit(1);
      } else {
          if ((pid3 = fork()) < 0) {
              fprintf(stderr, "CRITICAL ERROR: Unable to fork process 3: %s\n", strerror(errno));
              return(-1);
          } else if(pid3 == 0) {
              if (dup2(fd2[0], 0) < 0 ) {
                fprintf(stderr, "ERROR: Unable to dup to pipe: %s\n", strerror(errno));
                return(-1);
              }
              close(fd1[0]);
              close(fd1[1]);
              close(fd2[0]);
              close(fd2[1]);
              if (execlp("./pager.out", "pager.out", (char *)NULL) == -1) {
                  fprintf(stderr, "CRITICAL ERROR: Unable to execute pager.c: %s\n", strerror(errno));
              }
          }
      }
  }

  close(fd1[0]);
  close(fd1[1]);
  close(fd2[0]);
  close(fd2[1]);
  pid11 = wait3(&status, 0, &ru);
  fprintf(stderr, "Child #%d exits with %d\n", pid11, status);
  pid21 = wait3(&status, 0, &ru);
  fprintf(stderr, "Child #%d exits with %d\n", pid21, status);
  pid31 = wait3(&status, 0, &ru);
  fprintf(stderr, "Child #%d exits with %d\n", pid31, status);

  return(0);
}
