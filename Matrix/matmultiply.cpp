#include <iostream>
#include <stdio.h>
#include <stdlib.h>    // rand()
#include <pthread.h>   
#include <time.h>      // time
#include <iomanip>     // setw
#include <math.h>     

#define LIMIT 100
#define THRD_NUM 10

using namespace std;

struct args_t {
    float* a;
    float* b;
    float* c;
    int size;
    int st=0;
    int end=-1;
};

void multS(float* a, float* b, float* c, int size);                   
void multM(float* a, float* b, float* c, int size);
void printM(float* c, int n, bool prod=false);
void printAll(float* a, float* b, float* c, int size);

// Global variables
int signal = 1;
int counter = 0;
int thread_ids[THRD_NUM] = { 0, 1, 2, 3, 4, 5 };
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

int main(int argc, char *argv[])
{                    
    if(argc != 3) {
	cerr<<"usage: s|m matrice_size"<<endl;
	exit(1);
    }  
    string choice = argv[1];
    int size = atoi(argv[2]);
    if( choice != "m" and choice != "s" or size <= 0)
    {
	cerr<<"Invalid input"<<endl;
        exit(1);
    }

    srand(time(NULL));

    // allocate those matrices on the heap
    float* a = new float[size*size];
    for(int i = 0; i < size*size; ++i){
      float number = (float( rand() % (2*LIMIT*10) - LIMIT*10)) / 10.0;
      a[i] = number;
    }

    float* b = new float[size*size];
    for(int i = 0; i < size*size; ++i){
       float number = (float( rand() % (2*LIMIT*10) - LIMIT*10)) / 10.0;
       b[i] = number;
    }

    float* c = new float[size*size];

    pthread_mutex_init (&count_mutex, NULL);
    pthread_cond_init (&count_threshold_cv, NULL);

    // run with a single or multiple thread
    if(choice == "m"){
      multM(a, b, c, size);
    }
    else if(choice == "s"){
      multS(a, b, c, size);
    }

   // printAll(a, b, c, size);

    // delete the heap
    delete [] a; 
    delete [] b;
    
    return 0;
}

int at(int n, int r, int c) { return r*n+c; }
int thread_num;

void *mult(void *arguments)
{
    struct args_t *args = (args_t *)arguments;
    float* A = args->a;
    float* B = args->b;
    float* C = args->c;
    int size = args->size;
    int st = args->st;
    int end = args->end;

    if (end = -1) end = size;

    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            float num = 0;
            for (int k = 0; k < size; k++) {
                num += A[at(size, r, k)] * B[at(size, k, c)];
            }

            C[at(size, r, c)] = num;
        }
    }
    
    signal--;
    pthread_exit (NULL);
}

void multS(float* A, float* B, float* C, int size)
{
    signal = 1;
    pthread_attr_t attr;
    pthread_t thread;
    struct args_t args;
    args.a = A;
    args.b = B;
    args.c = C;
    args.size = size; 
    
    pthread_create (&thread, NULL, &mult, &args);

    while(signal!=0) {}
}

void multM(float* A, float* B, float* C, int size) {
    int thrd_num = min(size, THRD_NUM);
    int curr = 0;
    signal = 0;  
    pthread_t thrds[thrd_num];
    int num_line = max(size/THRD_NUM, 50);
    for (int t = 0; t < thrd_num ; t++) {
        struct args_t args;
        args.a = A;
        args.b = B;
        args.c = C;
        args.size = size;
        args.st = curr;
	args.end = min(size, curr+num_line);
        curr += num_line;

        if(curr>=size) break;
        signal++;
        pthread_create (&thrds[t], NULL, &mult, &args);
    }
    while(signal!=0){}

}

void printM(float* m, int size, bool prod)
{
    int wid = log10(LIMIT)+1;
    if (prod)
	wid = wid*2 + (log10(size) + 1);
    wid+=2;
	
    for(int r=0; r<size; r++)
    {
	for(int c=0; c<size; c++)
	{
	    cout<<setw(wid)<<m[at(size, r, c)]<<" ";
	}
	cout<<endl;
    }
}

void printAll(float* a, float* b, float* c, int size)
{
    cout<<"\nMatrix A\n";
    printM(a, size);

    cout<<"\nMatrix B\n";
    printM(b, size);

    cout<<"\nMatrix C\n";
    printM(c, size, true); 
}

