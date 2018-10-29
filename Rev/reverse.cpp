#include "shm_com.h"

using namespace std;

int main(int argc, char *argv[])
{
  if (argc != 4) {
    cerr << "error: improper usage" << endl;
    exit(1);
  }

  ////////////
  //Getting input string from the mess queue
  ////////////



  ///////////

  string in, out;
  //cin>>in;
  // for(int i=in.size()-1; i>=0; i--)
  //   out+=in[i];
  //cout<<out<<endl;
  
  int running = 1; 
  void *shared_memory = (void *)0;
  struct shared_use_st *shared_stuff;
  string message;
  int shmfd;

  void *addr = (void *)"Lalaland";

  srand((unsigned int)getpid());    

  // Open a shared memory object
  char const *shm_name = argv[2];
  shmfd = shm_open(shm_name, O_RDWR | O_CREAT | O_EXCL, 0666);
    
  sem_t *sem_p;
  int status;
  char const *sem_name = argv[3];

  if (shmfd == -1) {
    shm_unlink(shm_name);
    sem_unlink(sem_name);
    cout<<"Err print"<<endl;
    perror("shm_open");
    exit(1);
  }

  // Set the size of the memory object
  if(ftruncate(shmfd, sizeof(struct shared_use_st)) == -1) {
    perror("ftruncate");
    exit(1);
  }

  sem_p = sem_open(sem_name, O_CREAT | O_EXCL, 0600, 1);

  int fpid = fork();

  if (fpid == 0) {
    execl("upper", "upper", argv[2], argv[3], NULL);
  }

  // Map the object into our address space.
  shared_memory = mmap(addr, sizeof(struct shared_use_st),
		       PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
					
  if(shared_memory == MAP_FAILED) {
    perror("mmap");
    exit(1);
  }
					
  // We can close the file descriptor after mmap
  close(shmfd);

  cout << "Memory attached at " << shared_memory << endl;

  shared_stuff = (struct shared_use_st *)shared_memory;
  shared_stuff->written_by_you = 1;
  
  while(running) {

    msync(NULL, sizeof(struct shared_use_st), PROT_READ);
    sem_wait(sem_p);

    if(shared_stuff->written_by_you)
    {
      cout<<">";
      getline(cin, in);
      out="";
      for(int i=in.size()-1; i>=0; i--)
	out+=in[i];
      if(in == "exit")
	out = "exit";
      strncpy(shared_stuff->some_text, out.c_str(), TEXT_SZ);
      shared_stuff->written_by_you = 0;

      if(in == "exit")
	break;
    }
    
    sem_post(sem_p);
    sleep(1);
  }

  // Lastly, the shared memory is unmapped and the memory object deleted.
    
  munmap(shared_memory, sizeof(struct shared_use_st));
  shm_unlink(shm_name);

  sem_close(sem_p);
  wait(&status);
  // delete the semaphore
  sem_unlink(sem_name);
  cout << "Parent is exiting." << endl;


  exit(EXIT_SUCCESS);

  //////////
  

  return 0;
}
