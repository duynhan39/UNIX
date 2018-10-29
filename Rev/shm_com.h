/* A common header file to describe the shared memory we wish to pass about. */
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include <ctime>
#include <cstdlib>
#include <signal.h>
#include <algorithm>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <semaphore.h>
#include <mqueue.h>

#define TEXT_SZ 2048

struct shared_use_st {
    int written_by_you;
    char some_text[TEXT_SZ];
};

