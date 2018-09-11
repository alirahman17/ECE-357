#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
//Program Purpose: Searching for a file inside directories given the file exists

int c;
char targetfile[1024];
char testfile[1024];
char buftar[1024];
char buftes[1024];
char *testpath;
int n = 0, i = 0, equality = 0;
int targetfd;
int testfd;
int equality;
struct stat targetstat;
char intermediatepath[1024];
char testvar[1024];
char other[128];
ssize_t targetread, testread;

bool sameinode(const struct stat targetst, const struct stat testst){
  return(targetst.st_dev == testst.st_dev && targetst.st_ino == testst.st_ino);
}
bool differentsize(const struct stat targetst, const struct stat testst){
  if(targetst.st_size == testst.st_size){
    return(0);
  } else {
    return(1);
    }
}

bool filecomparison(char target, char test){
  for(i = 1; i< targetread; i++){
    equality = memcmp(target, test, i);
  }
  if(i = targetread && equality == 0){
    return(0);
  }
  if(equality != 0){
    return(1);
  }
}

void filesearch(char *pathname, int targetfd){
  DIR *dirp;
  struct dirent *direc;
  struct stat teststat;
  struct stat symstat;
    if (!(dirp=opendir(pathname))){
        fprintf(stderr, "NON-CRITICAL ERROR: Can not open directory %s\n",pathname);
        return(1);
    }

    while ((direc=readdir(dirp)) != NULL){
        snprintf(testfile, sizeof(testfile), "%s/%s",pathname, direc->d_name);
        if(lstat(testfile, &teststat) < 0){
          fprintf(stderr, "CRITICAL ERROR: failed to lstat file %s for comparison %s", direc->d_name, strerror(errno));
          continue;
        }
        if(S_ISLNK(teststat.st_mode)){
          if((symstat.st_mode&4 )){
            sprintf(other, "OK READ BY OTHER");
          } else {
            sprintf(other, "NOT READABLE BY OTHER");
          }
          if(stat(testfile, &symstat) < 0){
            fprintf(stderr, "CRITICAL ERROR: failed to stat file %s for comparison %s", direc->d_name, strerror(errno));
            continue;
          }
          if(sameinode(targetstat, symstat)){
            printf("%s is a symlink that resolves to target, %s\n", testfile, other);
          }
          if(!differentsize(targetstat, symstat) && !sameinode(targetstat, symstat)){
            if(!filecomparison(buftar,buftes)){
            printf("%s is a symlink that resolves to duplicate of target, %s \n", testfile, other);
            }
          }
        }else if(S_ISREG(teststat.st_mode)){
          testfd = open(direc->d_name, O_RDONLY);
          testread = read(testfd, buftes, sizeof(buftar));
          if((teststat.st_mode&4 )){
            sprintf(other, "OK READ BY OTHER");
          } else {
            sprintf(other, "NOT READABLE BY OTHER");
          }
          if(sameinode(targetstat, teststat)){
            printf("%s is a hard link to target, %s \n", testfile, other);
          }
          if(!differentsize(targetstat, teststat) && !sameinode(targetstat, teststat)){
            if(!filecomparison(buftar,buftes)){
            printf("%s is a duplicate of target (nlink=%d), %s \n", testfile, teststat.st_nlink, other);
            }
          }
          close(testfd);
        } else if(S_ISDIR(teststat.st_mode)){
            if(strcmp(direc->d_name, ".") == 0 || strcmp(direc->d_name, "..") == 0){
              continue;
            }
            sprintf(intermediatepath, "%s", testfile);
            filesearch(intermediatepath, targetfd);
        }
    }
    if(closedir(dirp) < 0){
      fprintf(stderr, "CRITICAL ERROR: Can not close directory %s\n",pathname);
      exit(1);
    }
}

int main(int argc, char **argv){
  if(argc < 3){
    printf("CRITICAL ERROR: Too Few Arguments: Expected targetfilename, pathname\n");
    exit(-1);
  }
        char *pathname = argv[2];
        sprintf(targetfile,"%s/%s", pathname, argv[1]);
  if(targetfd = open(targetfile, O_RDONLY) < 0){
    fprintf(stderr, "CRITICAL ERROR: Unable to open file %s for reading %s", argv[1], strerror(errno));
    return(-1);
  }
    targetread = read(targetfd, buftar, sizeof(buftar));
  if(targetread < 0){
    fprintf(stderr, "CRITICAL ERROR: Unable to read file %s for comparison %s", argv[1], strerror(errno));
    return(-1);
  }
  if(stat(targetfile, &targetstat) < 0){
    fprintf(stderr, "CRITICAL ERROR: failed to stat file %s for comparison %s", argv[1], strerror(errno));
    return(-1);
  }
    filesearch(pathname, targetfd);
      close(targetfd);
      return(0);
}
