#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <ctype.h>

int indexing, i;
char *buf[475000];
int wordcount;
char *inpword = NULL;
size_t length1 = 0, length2 = 0;
ssize_t n1, n2;

void sig_handler(int signo)
{
  printf("%d\n", signo);
  exit(signo);
}

void UPPERCASE(char *word) {
    for (int j = 0; j < strlen(word); j++) {
        word[j] = toupper(word[j]);
    }
}

int main(int argc, char **argv){
  signal(SIGPIPE, sig_handler);
  FILE *dictionary;
  if(argv[1] == NULL){
    fprintf(stderr, "No word argument specified for comparison\n");
    return(-1);
  }
    dictionary = fopen(argv[1],"r");
  if(dictionary == NULL){
    fprintf(stderr, "CRITICAL ERROR: Unable to open file %s for reading %s\n", argv[1], strerror(errno));
    return(-1);
  }

  while ((n1 = getline(&buf[indexing], &length1, dictionary)) != -1) {
    UPPERCASE(buf[indexing]);
    indexing++;
  }
    fclose(dictionary);

    while ((n2 = getline(&inpword, &length2, stdin)) != -1) {
      UPPERCASE(inpword);
      for(i = 0; i < indexing+1; i++) {
          if (strcmp(buf[i], inpword) == 0) {
              printf("%s", inpword);
              wordcount++;
              break;
          }
      }
    }

      printf("Matched %d words\n", wordcount);
      return(0);
}
