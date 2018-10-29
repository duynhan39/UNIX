#include "shm_com.h"

const int MAXLEN = 512;

enum msgtype { SRVMSG=1, CLIMSG };

struct msg { long type; char data[MAXLEN]; };

using namespace std;

int main(){
  
  sem_t *sem_p;
  int status;
  char const *sem_name = "/olaland";

  sem_p = sem_open(sem_name, O_CREAT | O_EXCL, 0600, 1);

  //msg srvmsg, rcdmsg;
  msg tmsg;
  //string srvstr, rcdstr;
  string textstr;
  time_t curtime;

  srand(getpid());
  tmsg.type = SRVMSG;

  string mtor("/myq_mtor")
    
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 1;
  attr.mq_msgsize = sizeof(tmsg);
  mqd_t mtor_id = mq_open(
			  mtor.c_str(), 
			  O_CREAT | O_EXCL | O_WRONLY, 
			  0600,
			  &attr);

  cout << "Message queue name is " << mtor << endl;
 
  int fpid = fork();
  if (fpid == 0) {

    int fpid2 = fork();
    if (fpid2 == 0) {
      cout<<"Starting the Upper"<<endl;
      execl("upper", "upper", "/shm_lalalol" ,sem_name, NULL);
    }
    cout<<"Starting the Reverse"<<endl;
    execl("reverse", "reverse", "msg queue", "/shm_lalalol" ,sem_name, NULL);
  }

  while(1) {

    /////////////////////
    cout<<"Flag Ser 1"<<endl;
    /////////////////////

    sem_wait(sem_p);
    // Generate message (time and date)
    cout<<"<\e[1m";
    string in;
    getline(cin, in);
    cout<<"\e[0m";
    srvstr = in;
    srvstr[srvstr.length()-1] = '\0';
    
    // Copy char array into C++ string
    srvstr.copy(srvmsg.data,MAXLEN-1);
    // in case message is longer than buffer
    // make sure it is zero terminated
    srvmsg.data[MAXLEN-1] = '\0';

    /////////////////////
    cout<<"Flag Ser 2"<<endl;
    /////////////////////

    // Send message to client
    cout << "Server sending: " << srvmsg.data << endl;
    mq_send(stoc_id, (const char *)&srvmsg, sizeof(srvmsg), 0);
    // Wait for acknowledgement
    
    sleep(rand()%4);
    sem_post(sem_p);
  }

  sem_close(sem_p);
  wait(&status);
  // delete the semaphore
  sem_unlink(sem_name);

  mq_close(stoc_id);
  mq_close(ctos_id);
  mq_unlink(stoc.c_str());
  mq_unlink(ctos.c_str());

  return 0;
}
