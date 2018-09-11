#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>

int fd;
int i;

int main(){
  chdir("/media/newmed");
  for(i = 0; i< 1000000; i++){
  fd = open("newfile.txt", O_WRONLY|O_CREAT|O_TRUNC, 0666);
  close(fd);
  rename("newfile.txt", "newname.txt");
  remove("newname.txt");
}
}
