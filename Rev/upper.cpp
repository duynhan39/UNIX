#include "shm_com.h"

using namespace std;

void init();
void cleanUp();

// SEM
sem_t *sem_p;
int status;

// SHM
void *shared_memory = (void *)0;
struct shared_use_st *shared_stuff;

char *shm_name;
char *sem_name;


int main(int argc, char *argv[])
{
    if (argc != 3) {
        cerr << "error: improper usage" << endl;
        exit(1);
    }
    
    shm_name = argv[1];
    sem_name = argv[2];
    
    init();
    
    sem_wait(sem_p);
    
    int masterPID=0;
    bool first = true;
    while(1)
    {
        msync(NULL, sizeof(struct shared_use_st), PROT_READ);
        
        if(!shared_stuff->written_by_you) {
            string in(shared_stuff->some_text);
            
            if(first) {
                first = false;
                //cout<<"Input "<<in<<endl;
                if(in.find_first_not_of("0123456789") == string::npos) {
                    // This is to make sure in is all digits
                    // Still err happens
                    //cout<<"Before the fall\n";
                    //cout<<"Here1"<<endl;
                    //ADD BACK//masterPID = stoi(in);
		    //cout<<"here2"<<endl;
                    //cout<<"Master PID: "<<masterPID<<endl;
                }
                else {
                    cout<<"FAIL!\n";
                    exit(1);
                }
            } else {
                if (in == "4^%@F@^&@QUIT!;") {
                    cout<<endl;
                    break;
                }
                // UPPER FUNCTION /////////////////////////////////////
                transform(in.begin(), in.end(), in.begin(), ::toupper);
                cout<<"\e[0m"<<in<<endl;
                ///////////////////////////////////////////////////////
                
                //kill(masterPID, SIGUSR1);
            }
            // Signal Rev that work here is done!
            shared_stuff->written_by_you = 1;
        }
        sem_post(sem_p);
    }
    shared_stuff->written_by_you = 1;
    sem_post(sem_p);
    
    cleanUp();
    
    exit(EXIT_SUCCESS);
    
    //////////////////////////
    
    return 0;
}


void init()
{
    // SHARED MEM
    shared_memory = (void *)0;
    string message;
    int shmfd;
    shmfd = shm_open(shm_name, O_RDWR, 0);
    
    if (shmfd == -1) {
        cout<<"UPPER  LINE 28\n";
        perror("shm_open");
        exit(1);
    }
    
    // MAPPING SHARED MEM
    shared_memory = mmap(NULL, sizeof(struct shared_use_st),
                         PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    
    if(shared_memory == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    close(shmfd);
    shared_stuff = (struct shared_use_st *)shared_memory;
    
    // SET UP SEM
    sem_p = sem_open(sem_name, 0);
}

void cleanUp()
{
    munmap(shared_memory, sizeof(struct shared_use_st));
    sem_close(sem_p);
}
