#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/* Comments after Grading
 Don't use perror, use strerror in fprintf instead
 inputfiles is an unnecessary variable, just use argc and argv instead
*/

int c, err = 0;
char *outFileName = NULL;
char *bname = NULL;
int bufnum = 1024;
int index = 0, inpind = 0, i = 0, iind = 0, standardinput = 0, n;

int main(int argc, char **argv)
{

  char *optarg;
  int optind;
  char inp;
  char inputFiles[16][256] = {NULL};
  char buf[bufnum];
  int fdi, fdo;
  ssize_t numRead, numRead1;

    while ((c = getopt(argc, argv, "b:o:-:")) != -1)
   	 switch (c) {
   	 case 'b':
   		 bname = optarg;
 				 bufnum = atoi(bname);
   		 break;
   	 case 'o':
   		 outFileName = optarg;
   		 break;
	 case '-':
		 standardinput = 1;
		 break;
   	 case '?':
   		 err = 1;
  	    printf("Unknown argument modifier used");
   		 break;
   	 }
  if (optind < argc){
     	 for (; optind < argc; optind++){
    	strcat(inputFiles[index], argv[optind]);
    	index++;
  	}
  	}
  if(index != 0){
    if(outFileName != NULL){
  	   fdo = open(outFileName, O_WRONLY|O_CREAT|O_TRUNC,0666);
      	if(fdo < 0){
        	perror("Unable to open output file: ");
        	return(-1);
        }
        if(standardinput = 1){
          while ((numRead = read(STDIN_FILENO, buf, sizeof(buf)-1)) > 0){
            if (write(fdo, buf, numRead) != numRead){
                	perror("Error writing to output file: ");
                	return(-1);
            if (numRead < 0){
                  perror("Error reading input file: ");
                  return(-1);
                }
              }
          	}
          }
  	for(iind = 0; iind < index; iind ++){
    	//errno = 0;
    	fdi = open(inputFiles[iind], O_RDONLY);
      if(fdi < 0){
      	perror("Unable to open input file: ");
      	return(-1);
      }
    	while ((numRead = read(fdi, buf, sizeof(buf)-1)) > 0){
          if (write(fdo, buf, numRead) != numRead){
            	perror("Error writing to output file: ");
            	return(-1);
          }
          if(numRead < 0){
          	perror("Error reading input file: ");
          	return(-1);
        	}
    	}
  	}
  }
  else{
  	while ((numRead1 = read(fdi, buf, sizeof(buf)-1)) > 0){
      if (write(STDOUT_FILENO, buf, numRead1) != numRead1){
          	perror("Error writing to standard output: ");
          	return (-1);
      }
      if(numRead1 < 0){
        	perror("Error reading input file: ");
        	return(-1);
      }
	   }
   }
  }
  else{
  	if(outFileName != NULL){
    	 fdo = open(outFileName, O_WRONLY|O_CREAT|O_TRUNC,0666);
      	if(fdo < 0){
        	perror("Unable to open output file: ");
        	return(-1);
      	}
    	while ((numRead = read(STDIN_FILENO, buf, sizeof(buf)-1)) > 0){
       if (write(fdo, buf, numRead) != numRead){
          	perror("Error writing to output file: ");
          	return(-1);
        if(numRead < 0){
            	perror("Error reading input file: ");
            	return(-1);
          }
        }
    	}
  	}
	  else{
  	  while (((numRead1 = read(STDIN_FILENO, buf, sizeof(buf)-1)) > 0)){
      	if (write(STDOUT_FILENO, buf, numRead1) != numRead1){
        	perror("Error writing to standard output: ");
      	}
        if(numRead1 < 0){
        	perror("Error reading input file: ");
        	return(-1);
        }
	   }
    }
  }
  close(fdo);
  	if(fdo < 0){
    	perror("Unable to close output file: ");
    	return(-1);
  	}
  close(fdi);
  	if(fdi < 0){
    	perror("Unable to close input file: ");
    	return(-1);
  	}
  return 0;
}
