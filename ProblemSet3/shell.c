#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/resource.h>

char buf[1024];
char *argv[100];
char *redir;
char *shellscript;
char *filename;
char* path= "/bin/";
char progpath[128];
char argument[1024];
char directory[128];
int argc;
int z = 0;
int n = 0;
int q = 0;
int redirectindex = 0;
int fd;
struct rusage usage;

int main()
{

while(1){

  memset(buf, 0, sizeof buf);
  printf("$mysh> ");
  read(0, buf, sizeof(buf));

  size_t length = strlen(buf);
  if (buf[length - 1] == '\n')
      buf[length - 1] = '\0';

  char *token;static clock_t st_time;
static clock_t en_time;
  token = strtok(buf," ");
  int i=0;
  while(token!=NULL){
      argv[i]=token;
      token = strtok(NULL," ");
      i++;
  }

  argv[i]=NULL;
  argc = i;
  if(strcmp(argv[0], "exit")==0){
    _exit(argv[1]);
      break;
  }
  if(argv[0][0] == '#'){
    continue;
  }

  if(strcmp(argv[0], "cd") == 0){
    sprintf(directory,"%s", argv[1]);
    for(z=0; z<strlen(directory); z++){
        if(directory[z]=='\n'){
            directory[z]='\0';
        }
    }
    chdir(directory);
    continue;
  }
  if(argv[0][0] == '.' && argv[0][1] == '/'){
    //Script Interpreter
    shellscript = argv[0] + 2
    fd = open(shellscript, O_RDONLY)
    char scriptbuf[10];
    int bytes_read;
    int k = 0;
    do {
        char t = 0;
        bytes_read = read(fd, &t, 1);
        buffer[k++] = t;
        if(t == '\n' || t == '\0') {
          printf("%s", buffer);
        }
    }
    while (bytes_read != 0);

    continue;
  }
  strcpy(progpath, path);
  strcat(progpath, argv[0]);

  for(z=0; z<strlen(progpath); z++){
      if(progpath[z]=='\n'){
          progpath[z]='\0';
      }
  }


  int pid = fork();
  switch(pid)
  {
  case 0:
    //REDIRECT CHECKING
    for(n = 0; n < i; n++){
      if(argv[n][0] == '<'){
        redir = argv[n];
        argv[n] = NULL;
        filename = redir + 1;
        fd=open(filename,O_WRONLY, 0666);
        dup2(fd,0);
        close(fd);
        break;
      }
      if(argv[n][0] == '>' && argv[n][1] != '>'){
        redir = argv[n];
        argv[n] = NULL;
        filename = redir + 1;
        fd=open(filename,O_CREAT|O_TRUNC|O_WRONLY, 0666);
        dup2(fd,1);
        close(fd);
        break;
      }
      if(argv[n][0] == '2' && argv[n][1] == '>' && argv[n][2] != '>'){
        redir = argv[n];
        argv[n] = NULL;
        filename = redir + 2;
        fd=open(filename,O_CREAT|O_TRUNC|O_WRONLY, 0666);
        dup2(fd,2);
        close(fd);
        break;
      }
      if(argv[n][0] == '>' && argv[n][1] == '>'){
        redir = argv[n];
        argv[n] = NULL;
        filename = redir + 2;
        fd=open(filename,O_CREAT|O_APPEND|O_WRONLY, 0666);
        dup2(fd,1);
        close(fd);
        break;
      }
      if(argv[n][0] == '2' && argv[n][1] == '>' && argv[n][2] == '>'){
        redir = argv[n];
        argv[n] = NULL;
        filename = redir + 3;
        fd=open(filename,O_CREAT|O_APPEND|O_WRONLY, 0666);
        dup2(fd,2);
        close(fd);
        break;
      }

    }
      execvp(progpath,argv);
      fprintf(stderr, "Child process could not execvp\n");
  exit(0);
  break;

  case -1:
  fprintf (stderr, "The child process has not created\n");
  break;

  default:
  wait3(NULL, 0, &usage);
  printf("Microseconds: Real Time: %ld, User Time %ld, System Time %ld\n", ((usage.ru_utime.tv_usec)+(usage.ru_stime.tv_usec)) ,usage.ru_utime.tv_usec, usage.ru_stime.tv_usec);
  break;
  }
}

printf("\n");

}
