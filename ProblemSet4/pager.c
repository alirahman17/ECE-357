#include <stdio.h>
#include <stdlib.h>

int wordcount, n;
char *inpword;
size_t length1;
ssize_t n1;

int main(int argc, char **argv[]) {
    FILE *dictionary;

    while ((n1 = getline(&inpword, &length1, stdin)) != -1) {
        printf("%s", inpword);
        wordcount++;
        if (wordcount == 23) {
            printf("---Press RETURN for more---\n");
            dictionary = fopen("/dev/tty", "w+");
            n = getc(dictionary);
            if (n == 113 || n == 81) exit(0);
            wordcount = 0;
        }
    }
    fclose(dictionary);
    return 0;
}
