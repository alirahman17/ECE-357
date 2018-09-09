#define nc 7

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int wordnum;
int stringsize;
int delay;
int stringlength;

void gen(char *s) {
    stringlength = (rand() % (nc + 1 - 3)) + 3;
    static const char letter[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < stringlength; ++i) {
        s[i] = letter[rand() % (sizeof(letter) - 1)];
    }
    s[stringlength] = 0;
}

int main(int argc, char **argv){
  srand(time(NULL));
  if(argv[1] == NULL){
    wordnum = 10000;
  }
  stringsize = atoi(argv[1]);
  char string[stringsize];
  while(wordnum < stringsize){
    gen(&string[wordnum]);
    printf("%s\n", &string[wordnum]);
    wordnum++;
  }
  return(0);
}
