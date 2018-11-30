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

// MQ
mqd_t mqfd;
struct mq_attr attr;

// NAMES
char *mq_name;
char *shm_name;
char *sem_name;

int main(int argc, char *argv[])
{
    if (argc != 4) {
        cerr << "error: improper usage" << endl;
        exit(1);
    }
    
    mq_name = argv[1];
    shm_name = argv[2];
    sem_name = argv[3];
    
    cout<<"MQ "<<mq_name<<endl;

    init();
    
    bool first = true;
    
    //// Out if mq works
//    cout<<"> \e[1m";
    string in;
    ///////////////////
    
    char message[256+1];

    cout<<"Here1"<<endl;
    mq_receive(mqfd, message, 256, NULL);
    cout<<"here2"<<endl;

    cout<<"REC: "<<message<<endl;

    bool cont=true;
    while(1) {

        // OUT if mq works
        //// TO THE SCREEN /////////
//        if(!first)
//            if(!getline(cin, in))
//            { cont=false; }
        ///////////////////////////
        
        // Put this in if mq works
        mq_receive(mqfd, message, 256, NULL);
        string in(message);

	cout<<"REC: "<<message<<endl;

        if(in == "4^%@F@^&@QUIT!;")
            cont = false;
        //        
        string out;// in(message);
        
        out = in;
        if(first) {
            first = false;
            // Im faking a master's ID, take out if mq runs
            out = "8899";
            ///////
        } else {
            if(cont) {
                out = "";
                for(int i=in.size()-1; i>=0; i--)
                    out+=in[i];
            }
            // Take this out if mq works
            else {
                out = "4^%@F@^&@QUIT!;";
            }
            ////////////////////////////
        }
        
        msync(NULL, sizeof(struct shared_use_st), PROT_READ);
        sem_wait(sem_p);
        
        strncpy(shared_stuff->some_text, out.c_str(), TEXT_SZ);
        shared_stuff->written_by_you = 0;
        
        sem_post(sem_p);
        
        while(!shared_stuff->written_by_you)
        {}
        
        if(!cont) {
            break;
        }
        
        /// TO THE SCREEN
        cout<<"> \e[1m";
    }
    
    cleanUp();
    
    exit(EXIT_SUCCESS);
    
    return 0;
}

void init()
{
    // MESSAGE QUEUE
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 256;
    attr.mq_flags   = 0;
    mqfd = mq_open(mq_name, O_RDONLY);
    cout<<"REV M: "<<mqfd<<endl;
    // SHARED MEM
    string message;
    int shmfd;

    shmfd = shm_open(shm_name, O_RDWR, 0666);
    
    if (shmfd == -1) {
        cout<<"REV LINE 35\n";
        perror("shm_open");
        exit(1);
    }
    // Set the size of the memory object
    if(ftruncate(shmfd, sizeof(struct shared_use_st)) == -1) {
        perror("ftruncate");
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
    shared_stuff->written_by_you = 1;
    
    // OPEN SEM
    sem_p = sem_open(sem_name, 0);
    //////////////////////////////////////////////////////
}

void cleanUp()
{
    munmap(shared_memory, sizeof(struct shared_use_st));
    shm_unlink(shm_name);
    sem_close(sem_p);
    wait(&status);
    sem_unlink(sem_name);
}

