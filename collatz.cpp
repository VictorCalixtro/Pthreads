#include <cstdio>
#include <algorithm>
#include <sys/time.h>
#include <pthread.h>

static long threadCount;
static long upper;
static long maxlen;
static pthread_mutex_t mutex;


void * collatz(void * rank)
{
   long my_rank = (long) rank;
   

   long localMaxlen =0;
    long i;
    for(i = my_rank*2+1; i <= upper; i+= threadCount*2){
	// work on item i

    long val = i;
    long len = 1;
    while (val != 1) {
      len++;
      if ((val % 2) == 0) {
        val = val / 2;  // even
      } else {
        val = 3 * val + 1;  // odd
      }
    }
    localMaxlen = std::max(localMaxlen, len);




}

 pthread_mutex_lock(&mutex);
 maxlen = std::max(localMaxlen, maxlen);
pthread_mutex_unlock(&mutex);





    return NULL;


}

int main(int argc, char *argv[])
{


 maxlen =0; 
 printf("Collatz v1.2\n");

  // check command line
  if (argc != 3) {fprintf(stderr, "USAGE: %s upper_bound\n", argv[0]); exit(-1);}
  upper = atol(argv[1]);
  
  //This is our number of threads
 // threadCount = strtol(argv[2],NULL, 10);  
 threadCount = atol(argv[2]); 
 if(threadCount < 1){printf("thread count must be at least 1; exiting the program...\n"); exit(-1);}
  

  if (upper < 5) {fprintf(stderr, "ERROR: upper_bound must be at least 5\n"); exit(-1);}
  if ((upper % 2) != 1) {fprintf(stderr, "ERROR: upper_bound must be an odd number\n"); exit(-1);}
 

  printf("upper bound: %ld\n", upper);
 

  pthread_mutex_init(&mutex, NULL);
  // start time
  timeval start, end;
  gettimeofday(&start, NULL);

 

  // execute timed code

 long thread;
 pthread_t * threadId;
//  pthread_t* const threadId = new pthread_t [threadCount];
 
 threadId = (pthread_t * )  malloc(threadCount * sizeof(pthread_t));

 printf("This is the master thread the number of requested threads is: %ld " , threadCount );


 for(thread =0; thread < threadCount-1; thread++){        
        
        pthread_create(&threadId[thread], NULL, collatz, (void*) thread);      
       
}


//Do Work Here
collatz((void *)(threadCount -1));

for(thread =0; thread < threadCount-1; thread++){

	pthread_join(threadId[thread],NULL);



  }




  // end time
  gettimeofday(&end, NULL);
  const double runtime = end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec) / 1000000.0;
  printf("compute time: %.4f s\n", runtime);

  // print result
  printf("longest sequence: %ld elements\n", maxlen);


  //delete [] threadId;
  pthread_mutex_destroy(&mutex);
  free(threadId);

 
 return 0;
}

