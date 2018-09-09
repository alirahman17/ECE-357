#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <signal.h>

char buf[8192];
int fd;
int i, n1, n2, status;
char *map;
struct stat teststat1;
struct stat teststat2;
size_t length = 4096;

void sig_handler(int signo)
{
  printf("%d\n", signo);
  signal(signo, SIG_DFL);
  exit(signo);
}

int main(int argc, char **argv){

  if(*argv[1] == '1'){
    for(i = 0; i <= 31; i++){
    signal(i, sig_handler);
    }
    getpid();
    fd = open("testfile.txt", O_RDWR|O_CREAT|O_TRUNC,0666);
    if(fd < 0){
      fprintf(stderr, "CRITICAL ERROR: Unable to open file testfile for lseek %s\n", strerror(errno));
      return(-1);
    }
    if (lseek(fd, 4096, SEEK_SET) == -1){
      fprintf(stderr, "CRITICAL ERROR: Unable to lseek testfile to mmap file %s\n", strerror(errno));
      return(-1);
    }
    if(write(fd, "", 1) < 0){
      fprintf(stderr, "CRITICAL ERROR: Unable to write to file testfile to mmap file %s\n", strerror(errno));
      return(-1);
    }
    if ((map = mmap(NULL, 4096, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) {
      fprintf(stderr, "CRITICAL ERROR: Unable to mmap file testfile for writing %s\n", strerror(errno));
      return(-1);
    }
    if(close(fd) < 0){
      fprintf(stderr, "CRITICAL ERROR: Unable to close file testfile %s\n", strerror(errno));
      return(-1);
    }
    map[1] = '5';
    if(map < 0){
      return(255);
    }
    if (msync((void *)map, 4096, MS_SYNC) < 0) {
      fprintf(stderr, "CRITICAL ERROR: Unable to msync file testfile for writeback %s\n", strerror(errno));
      return(-1);
    }
    if(munmap((void *)map, 4096) < 0 ){
      fprintf(stderr, "CRITICAL ERROR: Unable to munmap file testfile%s\n", strerror(errno));
      return(-1);
    }
    return(0);
  }
  if(*argv[1] == '2'){
      fd = open("testfile.txt", O_RDWR|O_CREAT|O_TRUNC,0666);
      if(fd < 0){
        fprintf(stderr, "CRITICAL ERROR: Unable to open file testfile for lseek %s\n", strerror(errno));
        return(-1);
      }
       size_t textsize = strlen("TEST\n") + 1;
      if (lseek(fd, textsize-1, SEEK_SET) == -1){
        fprintf(stderr, "CRITICAL ERROR: Unable to lseek testfile to mmap file %s\n", strerror(errno));
        return(-1);
      }
      if(write(fd, "", 1) < 0){
        fprintf(stderr, "CRITICAL ERROR: Unable to write to file testfile to mmap file %s\n", strerror(errno));
        return(-1);
      }
      if((n1 = read(fd, buf, sizeof(buf))) < 0){
        fprintf(stderr, "CRITICAL ERROR: Unable to read file testfile for comparison %s\n", strerror(errno));
        return(-1);
      }
      if ((map = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED,
    					fd, 0)) == MAP_FAILED) {
        fprintf(stderr, "CRITICAL ERROR: Unable to mmap file testfile for writing %s\n", strerror(errno));
    		return(-1);
    	}
      if(close(fd) < 0){
        fprintf(stderr, "CRITICAL ERROR: Unable to close file testfile for writing %s\n", strerror(errno));
        return(-1);
      }
      strcpy(map, "TEST\n");
      if (msync((void *)map, 4096, MS_SYNC) < 0) {
    		fprintf(stderr, "CRITICAL ERROR: Unable to msync file testfile for writeback %s\n", strerror(errno));
    		return(-1);
    	}
      if(munmap((void *)map, 4096) < 0 ){
        fprintf(stderr, "CRITICAL ERROR: Unable to munmap file testfile%s\n", strerror(errno));
        return(-1);
      }
      fd = open("testfile.txt", O_RDONLY);
      if(fd < 0){
        fprintf(stderr, "CRITICAL ERROR: Unable to open file testfile for reading %s\n", strerror(errno));
        return(-1);
      }
      if((n2 = read(fd, buf, sizeof(buf))) < 0){
        fprintf(stderr, "CRITICAL ERROR: Unable to read file testfile for comparison %s\n", strerror(errno));
        return(-1);
      }
      if(close(fd) < 0){
        fprintf(stderr, "CRITICAL ERROR: Unable to close file testfile %s\n", strerror(errno));
        return(-1);
      }

      if(strcmp(buf, "TEST\n") == 0){
        exit(0);
      } else {
        exit(1);
      }
  }
  if(*argv[1] == '3'){
      fd = open("testfile.txt", O_RDWR|O_CREAT|O_TRUNC,0666);
      if(fd < 0){
        fprintf(stderr, "CRITICAL ERROR: Unable to open file testfile for lseek %s\n", strerror(errno));
        return(-1);
      }
       size_t textsize = strlen("TEST\n") + 1;
      if (lseek(fd, textsize-1, SEEK_SET) == -1){
        fprintf(stderr, "CRITICAL ERROR: Unable to lseek testfile to mmap file %s\n", strerror(errno));
        return(-1);
      }
      if(write(fd, "", 1) < 0){
        fprintf(stderr, "CRITICAL ERROR: Unable to write to file testfile to mmap file %s\n", strerror(errno));
        return(-1);
      }
      if((n1 = read(fd, buf, sizeof(buf))) < 0){
        fprintf(stderr, "CRITICAL ERROR: Unable to read file testfile for comparison %s\n", strerror(errno));
        return(-1);
      }
      if ((map = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE,
    					fd, 0)) == MAP_FAILED) {
        fprintf(stderr, "CRITICAL ERROR: Unable to mmap file testfile for writing %s\n", strerror(errno));
    		return(-1);
    	}
      if(close(fd) < 0){
        fprintf(stderr, "CRITICAL ERROR: Unable to close file testfile for writing %s\n", strerror(errno));
        return(-1);
      }
      strcpy(map, "TEST\n");
      if (msync((void *)map, 4096, MS_SYNC) < 0) {
    		fprintf(stderr, "CRITICAL ERROR: Unable to msync file testfile for writeback %s\n", strerror(errno));
    		return(-1);
    	}
      if(munmap((void *)map, 4096) < 0 ){
        fprintf(stderr, "CRITICAL ERROR: Unable to munmap file testfile %s\n", strerror(errno));
        return(-1);
      }
      fd = open("testfile.txt", O_RDONLY);
      if(fd < 0){
        fprintf(stderr, "CRITICAL ERROR: Unable to open file testfile for reading %s\n", strerror(errno));
        return(-1);
      }
      if((n2 = read(fd, buf, sizeof(buf))) < 0){
        fprintf(stderr, "CRITICAL ERROR: Unable to read file testfile for comparison %s\n", strerror(errno));
        return(-1);
      }
      if(close(fd) < 0){
        fprintf(stderr, "CRITICAL ERROR: Unable to close file testfile %s\n", strerror(errno));
        return(-1);
      }
      if(strcmp(buf, "TEST\n") == 0){
        exit(0);
      } else {
        exit(1);
      }

  }
  if(*argv[1] == '4'){
      //MAP_SHARED file between 1 and 2 pages to mapped memory with r/w, write to mem just beyond the last byte
    fd = open("testfile4.txt", O_RDWR|O_CREAT|O_TRUNC,0666);
    if(fd < 0){
      fprintf(stderr, "CRITICAL ERROR: Unable to open file testfile for lseek %s\n", strerror(errno));
      return(-1);
    }
    if (lseek(fd, 5000, SEEK_SET) == -1){
      fprintf(stderr, "CRITICAL ERROR: Unable to lseek testfile to mmap file %s\n", strerror(errno));
      return(-1);
    }
    if(write(fd, "", 1) < 0){
      fprintf(stderr, "CRITICAL ERROR: Unable to write to file testfile to mmap file %s\n", strerror(errno));
      return(-1);
    }
    if ((map = mmap(NULL, 5000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
      fprintf(stderr, "CRITICAL ERROR: Unable to mmap file testfile for writing %s\n", strerror(errno));
      return(-1);
    }
    //try stat
    if(stat("testfile4.txt", &teststat1) < 0){
      fprintf(stderr, "CRITICAL ERROR: failed to stat file testfile4.txt for comparison %s", strerror(errno));
      return(-1);
    }
    map[5002] = '1';
    if (msync((void *)map, 8096, MS_SYNC) < 0) {
      fprintf(stderr, "CRITICAL ERROR: Unable to msync file testfile for writeback %s\n", strerror(errno));
      return(-1);
    }
    if(munmap((void *)map, 4096) < 0 ){
      fprintf(stderr, "CRITICAL ERROR: Unable to munmap file testfile %s\n", strerror(errno));
      return(-1);
    }
    if(close(fd) < 0){
      fprintf(stderr, "CRITICAL ERROR: Unable to close file testfile %s\n", strerror(errno));
      return(-1);
    }
    if(stat("testfile4.txt", &teststat2) < 0){
      fprintf(stderr, "CRITICAL ERROR: failed to stat file testfile4.txt for comparison %s", strerror(errno));
      return(-1);
    }
    if(teststat1.st_size = teststat2.st_size){
      exit(1); //Byte Unchanged
    } else {
      exit(0); //Byte Changed
    }


  }
  if(*argv[1] == '5'){
      //MAP_SHARED file between 1 and 2 pages to mapped memory with r/w, write to mem just beyond the last byte, lseek 16 past EOF, write 1 byte
      fd = open("testfile4.txt", O_RDWR|O_CREAT|O_TRUNC,0666);
      if(fd < 0){
        fprintf(stderr, "CRITICAL ERROR: Unable to open file testfile for lseek %s\n", strerror(errno));
        return(-1);
      }
      if (lseek(fd, 5000, SEEK_SET) == -1){
        fprintf(stderr, "CRITICAL ERROR: Unable to lseek testfile to mmap file %s\n", strerror(errno));
        return(-1);
      }
      if(write(fd, "", 1) < 0){
        fprintf(stderr, "CRITICAL ERROR: Unable to write to file testfile to mmap file %s\n", strerror(errno));
        return(-1);
      }
      if ((map = mmap(NULL, 5000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        fprintf(stderr, "CRITICAL ERROR: Unable to mmap file testfile for writing %s\n", strerror(errno));
        return(-1);
      }
      map[5002] = '7';
      if (lseek(fd, 16, SEEK_END) == -1){
        fprintf(stderr, "CRITICAL ERROR: Unable to lseek testfile to mmap file %s\n", strerror(errno));
        return(-1);
      }
      if(write(fd, "", 1) < 0){
        fprintf(stderr, "CRITICAL ERROR: Unable to write to file testfile to mmap file %s\n", strerror(errno));
        return(-1);
      }
      if (msync((void *)map, 8096, MS_SYNC) < 0) {
        fprintf(stderr, "CRITICAL ERROR: Unable to msync file testfile for writeback %s\n", strerror(errno));
        return(-1);
      }
      if(munmap((void *)map, 4096) < 0 ){
        fprintf(stderr, "CRITICAL ERROR: Unable to munmap file testfile %s\n", strerror(errno));
        return(-1);
      }
      if(close(fd) < 0){
        fprintf(stderr, "CRITICAL ERROR: Unable to close file testfile %s\n", strerror(errno));
        return(-1);
      }
      if(stat("testfile4.txt", &teststat2) < 0){
        fprintf(stderr, "CRITICAL ERROR: failed to stat file testfile4.txt for comparison %s", strerror(errno));
        return(-1);
      }
      if((fd = open("testfile4.txt", O_RDONLY)) < 0){
        fprintf(stderr, "CRITICAL ERROR: Unable to open file testfile for reading %s\n", strerror(errno));
        return(-1);
      }
      n1 = read(fd, buf, sizeof(buf));
      if(buf[5002] == '7'){
        exit(0);
      } else{
        exit(1);
      }
  }
  if(*argv[1] == '6'){
    for(i = 0; i <= 31; i++){
    signal(i, sig_handler);
    }
    getpid();
    fd = open("testfile4.txt", O_RDWR|O_CREAT|O_TRUNC,0666);
    if(fd < 0){
      fprintf(stderr, "CRITICAL ERROR: Unable to open file testfile for lseek %s\n", strerror(errno));
      return(-1);
    }
    if (lseek(fd, 3000, SEEK_SET) == -1){
      fprintf(stderr, "CRITICAL ERROR: Unable to lseek testfile to mmap file %s\n", strerror(errno));
      return(-1);
    }
    if(write(fd, "", 1) < 0){
      fprintf(stderr, "CRITICAL ERROR: Unable to write to file testfile to mmap file %s\n", strerror(errno));
      return(-1);
    }
    if ((map = mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
      fprintf(stderr, "CRITICAL ERROR: Unable to mmap file testfile for writing %s\n", strerror(errno));
      return(-1);
    }
    printf("%s at 3050\n", map[3050]);
    printf("%s at 5000\n", map[5000]);
    if(close(fd) < 0){
      fprintf(stderr, "CRITICAL ERROR: Unable to close file testfile %s\n", strerror(errno));
      return(-1);
    }
    if(munmap((void *)map, 4096) < 0 ){
      fprintf(stderr, "CRITICAL ERROR: Unable to munmap file testfile %s\n", strerror(errno));
      return(-1);
    }
    exit(0);
  }
    return(0);
}
