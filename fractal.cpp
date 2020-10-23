#include <cstdio>
#include <cmath>
#include <algorithm>
#include <sys/time.h>
#include "BMP43805351.h"
#include <pthread.h>

static long threadCount;
//static pthread_mutex_t mutex;
static  int width;
static  int frames;
static unsigned char*  pic;



static void* fractal(void * rank)
{
  long my_rank = (long) rank;



  const double Delta = 0.002;
  const double xMid = 0.2315059;
  const double yMid = 0.5214880;

  // compute pixels of each frame
  double delta = Delta;
  for (int frame = my_rank +1; frame < frames; frame+=threadCount) {  // frames
    const double xMin = xMid - delta;
    const double yMin = yMid - delta;
    const double dw = 2.0 * delta / width;
    for (int row = 0; row < width; row++) {  // rows
      const double cy = yMin + row * dw;
      for (int col = 0; col < width; col++) {  // columns
        const double cx = xMin + col * dw;
        double x = cx;
        double y = cy;
        double x2, y2;
        int depth = 256;
        do {
          x2 = x * x;
          y2 = y * y;
          y = 2.0 * x * y + cy;
          x = x2 - y2 + cx;
          depth--;
        } while ((depth > 0) && ((x2 + y2) < 5.0));
        pic[frame * width * width + row * width + col] = (unsigned char)depth;
      }
    }
    delta *= 0.98;
  }


return NULL;
}

int main(int argc, char *argv[])
{
  



  // check command line
  if (argc != 4) {fprintf(stderr, "USAGE: %s frame_width number_of_frames\n", argv[0]); exit(-1);}
 
 threadCount = atol(argv[3]);
 if(threadCount < 1){printf("Threadcount must be at least 1; Exiting the program\n");exit(-1);}

   width = atoi(argv[1]);
  if (width < 10) {fprintf(stderr, "ERROR: frame_width must be at least 10\n"); exit(-1);}
  frames = atoi(argv[2]);
  if (frames < 1) {fprintf(stderr, "ERROR: number_of_frames must be at least 1\n"); exit(-1);}
  printf("width: %d\n", width);
  printf("frames: %d\n", frames);

printf("This is the master thread the number of requested threads is: %ld " , threadCount );

//Mutex initialization
//pthread_mutex_init(&mutex, NULL);


  // allocate picture array
  pic = new unsigned char [frames * width * width];

  // start time
  timeval start, end;
  gettimeofday(&start, NULL);

  // execute timed code

long thread;
 pthread_t * threadId;
 threadId = (pthread_t * )  malloc(threadCount * sizeof(pthread_t));
  
 
     for(thread =0; thread < threadCount-1; thread++){
 
        pthread_create(&threadId[thread], NULL, fractal, (void*) thread);
 
      }
             
fractal((void *)(threadCount -1));

for(thread =0; thread < threadCount-1; thread++){

        pthread_join(threadId[thread],NULL);



  }


  // end time
  gettimeofday(&end, NULL);
  const double runtime = end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec) / 1000000.0;
  printf("compute time: %.4f s\n", runtime);

  // write result to BMP files
  if ((width <= 257) && (frames <= 60)) {
    for (int frame = 0; frame < frames; frame++) {
      BMP24 bmp(0, 0, width - 1, width - 1);
      for (int y = 0; y < width - 1; y++) {
        for (int x = 0; x < width - 1; x++) {
          const int p = pic[frame * width * width + y * width + x];
          const int e = pic[frame * width * width + y * width + (x + 1)];
          const int s = pic[frame * width * width + (y + 1) * width + x];
          const int dx = std::min(2 * std::abs(e - p), 255);
          const int dy = std::min(2 * std::abs(s - p), 255);
          bmp.dot(x, y, dx * 0x000100 + dy * 0x000001);
        }
      }
      char name[32];
      sprintf(name, "fractal%d.bmp", frame + 1000);
      bmp.save(name);
    }
  }

  // clean up
  delete [] pic;

 // pthread_mutex_destroy(&mutex);
  free(threadId);
 

 return 0;
}

