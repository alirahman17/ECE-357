#include "spinlock.h"

int i;
int k;

int main(int argc, char *argv[]){
  if(argc != 3){
      fprintf(stderr, "CRITICAL ERROR: Incorrect number of arguments (3 required)\n");
      exit(1);
  }
  long long unsigned int fknum = atoll(argv[1]);
  long long unsigned int iternum = atoll(argv[2]);
  printf("fknum = %llu\niternum = %llu\n", fknum, iternum);
  int *map = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0 );
  if(map< 0){
      fprintf(stderr, "CRITICAL ERROR: Unable to MMAP ANON page: %s\n", strerror(errno));
      exit(1);
  }
  map[0] = 0;
//  spinlock *lock;
//  lock = (spinlock *)(map+sizeof(spinlock));
//  lock->primitive_lock= map[1];
  pid_t pid[fknum];
  for (i = 0; i < fknum; i++){
    //fork
    if((pid[i] = fork()) < 0){
      //fork ERROR
        fprintf(stderr, "NON-CRITICAL ERROR: Unable to fork id#%d: %s\n",i, strerror(errno));
        return(1);
    }
    if(pid[i] == 0){
      //  spin_lock(lock);
      for(k = 0; k < iternum; k++){
        map[0]++;
      }
      //spin_unlock(lock);
      exit(0);
    }
  }
  for(i = 0; i < fknum; i++){
    wait(0);
  }
  printf("%d\n", map[0]);
  return(0);
}
