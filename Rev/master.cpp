#include "shm_com.h"

const int MAXLEN = 512;

enum msgtype { SRVMSG=1, CLIMSG };

struct msg { long type; char data[MAXLEN]; };

using namespace std;

void init();
void forking(int &fpid1, int &fpid2);

// SEM
sem_t *sem_p;
int status;

// SHM
void *shared_memory = (void *)0;
struct shared_use_st *shared_stuff;

// MQ
mqd_t mqfd;
struct mq_attr attr;

// NAME
char *mq_name;
char *shm_name;
char *sem_name;

//int sig;
void CONT(int signal) { }

int main()
{
    shm_name = (char*)"shm_what";
    sem_name = (char*)"sem_lala";
    mq_name = (char*)"/mq_bloodyhell";
    
    init();
    
    int fpid1, fpid2;
    forking(fpid1, fpid2);
    
    //// First message is to send Master's pid to Upper
//    if(fpid1 != 0)
//    {
//        string pid = to_string((int)fpid1);
//        mq_send(mqfd, pid.c_str(), 256, 1);
//        //pause();
//    }
    
    //////////////////
    cout<<"> \e[1m";
    cout<<fpid1<<" > \e[1m";
 
    string in;
    while(1) {
        bool cont=true;

        //// Put in if mq works ///////////
//        if(!getline(cin, in))
//        { cont=false; }
        
//        mq_send(mqfd, in.c_str(), 256, 1);
//
//        pause();
//        signal(SIGUSR1, CONT);
        
        sleep(3);
        
        // To terminate the program after a certain amount of since since I
        // cant have mq works
        cont = false;
        
        if(!cont) {
            mq_send(mqfd, "4^%@F@^&@QUIT!;", 256, 1);
            break;
        }
        
        /// TO THE SCREEN
        cout<<"> \e[1m";
    }
    //////////////////
    
    wait(&status);
    
    //cout<<"DONE"<<endl;
    
    return 0;
}


void init()
{
    // MESS QUEUE ///////////////////////////////
    attr.mq_maxmsg = 256;
    attr.mq_msgsize = 256;
    attr.mq_flags   = 0;
    mqfd = mq_open(mq_name, O_WRONLY|O_CREAT, 0666, &attr);
    
    // SHARED MEM ///////////////////////////////
    string message;
    int shmfd;
    shmfd = shm_open(shm_name, O_RDWR | O_CREAT | O_EXCL, 0666);
    
    if (shmfd == -1) {
        shm_unlink(shm_name);
        sem_unlink(sem_name);
        shmfd = shm_open(shm_name, O_RDWR | O_CREAT | O_EXCL, 0666);
    }
    // Set the size of the memory object
    if(ftruncate(shmfd, sizeof(struct shared_use_st)) == -1) {
        perror("ftruncate");
        exit(1);
    }
    
    shared_memory = mmap(NULL, sizeof(struct shared_use_st),
                         PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if(shared_memory == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    close(shmfd);
    
    // SEM ///////////////////////////////////////
    sem_t *sem_p;
    sem_p = sem_open(sem_name, O_CREAT | O_EXCL, 0600, 1);
}

void forking(int &fpid1, int &fpid2)
{
    fpid1 = fork();
    if (fpid1 == 0) {
        fpid2 = fork();
        if(fpid2 == 0)
            execl("reverse", "reverse", mq_name, shm_name, sem_name, NULL);
        else
            execl("upper", "upper", shm_name, sem_name, NULL);
    }
}

void cleanUp()
{
    munmap(shared_memory, sizeof(struct shared_use_st));
    shm_unlink(shm_name);
    sem_close(sem_p);
    wait(&status);
    sem_unlink(sem_name);
}
