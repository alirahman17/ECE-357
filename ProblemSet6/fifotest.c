#include "fifo.h"

int procno, i, k, reader, n, w;
int numwriters = 28;
int writelen = MYFIFO_BUFSIZ;

int main(){
  fifo *fi;
    fi = (fifo * )mmap (NULL, sizeof (fifo), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if(fi < 0){
    fprintf(stderr, "CRITICAL ERROR: Unable to mmap ANONYMOUS page for fifo: %s\n", strerror(errno));
    exit(1);
  }
  fifo_init(fi);
  printf("Beginning test with %d writers, %d items each\n", numwriters, writelen);
  pid_t pid[numwriters];
  for(i = 0; i < numwriters; i++){
    if((pid[i] = fork()) < 0){
      //FORK ERROR
    }
    if(pid[i] == 0){
      //CHILD
      procno = i;
      unsigned long writebuf[writelen];
      for(k = 0; k < writelen; k++){
        writebuf[k] = getpid()*10000 + k;
        fifo_wr(fi, writebuf[k]);
      }
      printf("Writer %d completed\n", i);
      exit(0);
    }
  }

  if((reader = fork()) < 0){
    //FORK ERROR
  }
  if(reader == 0){
    //CHILD
    procno = numwriters;
    unsigned long readbuf[numwriters*writelen];
    int nread = numwriters*writelen;
    for(i = 0; i < nread; i++){
      readbuf[i] = fifo_rd(fi);
    }
    printf("All streams done\n");
  }
  printf("Waiting for children to die\n");
  for(n = 0; n < numwriters + 1; n++){
    wait(0);
  }

  return(0);
}
