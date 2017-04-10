#include "myTimer.h"
#include "simulator.h"


/*
 * this a timer file that just waits when it is called.
 * waiting in real time.
 *
 * the variable clock_t works in microseconds, 1/1000000
 * any interaction of waiting needs to be in microseconds.
 * for example msec is the unit in the config data
 * for that to be in microseconds, you need to times that
 * unit by a 1000.
 */


/*
 * myWait is a while loop that keeps on checking the current cpu time.
 * the goalTime + target is what current time needs be to equal
 * the target is the time a meta data needs to take
 */
void myWait(clock_t target)
  {
     clock_t currentTime = clock();
     clock_t goalTime = clock();
     

     while(currentTime <= (goalTime + target))
       {
          currentTime = clock();
       }
  }



void *myTWait ( void *myvar)
  {
     static int tester = 0;
     struct forThread *data = (struct forThread*)myvar;
     int target = data->processWaitTime;
     clock_t currentTime = clock();
     clock_t goalTime = clock();

     while(currentTime <= (goalTime + target))
       {

          currentTime = clock();
       }
     threadHandler(data);
     return NULL;
     tester = tester + 1 - 1; // to silence warning about tester being unused, even though it is being used.
  }

void *myTWait2 ( void *myvar)
  {
 
   struct forThread *data = (struct forThread*)myvar;
   int target = data->processWaitTime;
   clock_t currentTime = clock();
   clock_t goalTime = clock();

  
   while(currentTime <= (goalTime + target))
     {

        currentTime = clock();
     }
  
   return NULL;
  }
