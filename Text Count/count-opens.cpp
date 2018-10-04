#include <fcntl.h>
#include <unistd.h>
#include <iostream>

/*
The program's output is 256, plus 3 automatically run in the first place would emean the maximum is 256, which is the same with the result from ulimit -n.
This number, however can be modified at will since the limit of the hard limit is unlimited.
*/

int main(int argc, char *argv[]){
  int count=0, fd;
  while(( fd = open(argv[0], O_RDONLY)) != -1 )
    count++;
  std::cout<<count<<std::endl;

  close(fd);

  return 0;
}
