#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define buf 1024

int main(){
    char input[buf];
    char* argv[100];
    char* path= "/bin/";
    char progpath[20];      //full file path
    int argc;               //arg count
    printf("$mysh ");
while(1){
                       //print shell prompt

    read(0, input, sizeof(input));
  //      if(!read(input, buf, stdin)){  //get command and put it in line
  //      break;                                //if user hits CTRL+D break
  //  }
    if(strcmp(input, "exit")==0){            //check if command is exit
        exit(1);
        break;
    }

    char *token;
    token = strtok(input," ");
    int i=0;
    while(token!=NULL){
        argv[i]=token;
        token = strtok(NULL," ");
        i++;
    }
    argv[i]=NULL;                     //set last value to NULL for execvp



    argc=i;                           //get arg count
    for(i=0; i<argc; i++){
    //    printf("%s\n", argv[i]);      //print command/args
    }
    strcpy(progpath, path);           //copy /bin/ to file path
    strcat(progpath, argv[0]);            //add program to path

    for(i=0; i<strlen(progpath); i++){    //delete newline
        if(progpath[i]=='\n'){
            progpath[i]='\0';
        }
    }
    int pid= fork();


    if(pid==0){               //Child
      if(argv[0] = "cd"){
        chdir(argv[1]);
        printf("Changed directory to %s", argv[1]);
      } else{
          execvp(progpath,argv);
          fprintf(stderr, "Child process could not execvp\n");
          }


    }else{                    //Parent
        wait(NULL);
        printf("DEBUG: Child Process Exited\n");
    }

}
}
