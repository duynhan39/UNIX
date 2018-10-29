#include "shm_com.h"

using namespace std;

int main(int argc, char *argv[])
{
  
  if (argc != 3) {
    cerr << "error: improper usage" << endl;
    exit(1);
  }

  int running = 1;
  void *shared_memory = (void *)0;
  struct shared_use_st *shared_stuff;
  string message;
  int shmfd;
          
  void *addr = (void *)"Lalaland";
          
  sem_t *sem_p;
  sem_p = sem_open(argv[2], 0);
        
  // Open a shared memory object
  char const *shm_name = argv[1];
  shmfd = shm_open(shm_name, O_RDWR, 0);

  if (shmfd == -1) {
    cout<<"HERE\n";
    perror("shm_open");
    exit(1);
  }
    
  sem_wait(sem_p);
  // Map the object into our address space.
  shared_memory = mmap(addr, sizeof(struct shared_use_st),
		       PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);

  if(shared_memory == MAP_FAILED) {
    perror("mmap");
    exit(1);
  }

  // We can close the file descriptor after mmap
  close(shmfd);

  //cout << "Child memory attached at " << shared_memory << endl;

  shared_stuff = (struct shared_use_st *)shared_memory;
  
  while(running) {

    msync(NULL, sizeof(struct shared_use_st), PROT_READ);

    if(!shared_stuff->written_by_you) {
      string in = shared_stuff->some_text;
      shared_stuff->written_by_you = 1;

      if (in == "exit") {
	break;
      }

      transform(in.begin(), in.end(), in.begin(), ::toupper);
      cout<<in<<endl;
    }

    sem_post(sem_p);
  }

  munmap(shared_memory, sizeof(struct shared_use_st));
  sem_close(sem_p);

  exit(EXIT_SUCCESS);
    
  //////////////////////////
  
  return 0;
}
