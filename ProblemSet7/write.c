#include <unistd.h>

int main(){
  write(1, "Test Output!\n", 13);
  return(0);
}
