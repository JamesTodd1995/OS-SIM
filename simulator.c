#include "simulator.h" 


/*
 * method startSim will read though the meta data file and get the processes from it
 * once process are made the Simulator will read though the processes
 * in the order that the config data states to.
 */


/*
 * to understand the use of clock_t variables, please look at myTimer.c
 */


/*
 * writing to a log file was hard to do.
 * when code comments such as:

     //====this statement adds to the logNode memory info ================
     sprintf(printArray,"Time: %.6lf, OS: System start",time/CLOCKS_PER_SEC);
     strcat(printingLine, printArray);numberArray
     clean(printArray,printArraySize);
     strcpy(currentLN->data,printingLine);
     currentLN = addLogNode(currentLN);  
     clean(printingLine,printArraySize);
     // ==================================================================

 * pop up i am writing that infomation to a log link list
 * so i can write it to a file when the sim is done.
 *
 * also i did not make methods to do this for me because i thought it would
 * be redundent to do so because i would need 3 diffrent methods to
 */
int startSim(struct Node *metaD,struct configStruct configData)
  {

     // a list of variables needed for the simulator to work
     double time = 0;
     char printingLine[80];
     char numberArray[12];
     int numberArraySize = 12;
     int printingLineSize = 80;
     int howToPrint = 0;
     int numberOfProcess = 0, processCounter = 0, tester = 0, index = 0;
     char printArray[80];
     int printArraySize = 80;

     clock_t startTime, endTime;

     struct Node *metaData = metaD;
     struct logNode *ln = (struct logNode*)malloc(sizeof(struct logNode));
     struct logNode *currentLN = ln;

     clean(printingLine,printingLineSize);
      
     if((strcmp(configData.logToData, "Monitor")) == 0) 
       {   
         howToPrint = printTM;
       }
     if((strcmp(configData.logToData, "Both")) == 0) 
       {   
         howToPrint = printB;
       }
     if((strcmp(configData.logToData, "File")) == 0) 
       {   
         howToPrint = printTL;
         printf("\nyou only picked File out put, this will take time to finish.");
         printf("\n");
       }

     if(howToPrint == printTM || howToPrint == printB)
       {
        printf("\nOperating System Simulator");
        printf("\n==========================\n");
        printf("\nLoading configuration file");
        printf("\nLoading meta-data");
        printf("\n==========================\n");
        printf("\nBegin Simulation");
       }

  
     if(howToPrint == printTM || howToPrint == printB)
       {
        printf("\nTime: %.6lf, System start", time);
       }
     //====this statement adds to the logNode memory info ================
     sprintf(printArray,"Time: %.6lf, OS: System start",time/CLOCKS_PER_SEC);
     strcat(printingLine, printArray);
     clean(printArray,printArraySize);
     strcpy(currentLN->data,printingLine);
     currentLN = addLogNode(currentLN);  
     clean(printingLine,printArraySize);
     // ==================================================================


     // test to see if the meta data has a S(start)0 command, if not error out
     startTime = clock();
     tester = getStart(metaData, configData.processorCycleTimeData);
     if(tester != 0)
       {
          return tester;  
       }
     endTime = clock();


     time += ((double) (endTime-startTime));   



     if(howToPrint == printTM || howToPrint == printB)
       {  
        printf("\nTime: %.6lf, OS: Begin PCB Creation", time/CLOCKS_PER_SEC);
       }

     //====this statement adds to the logNode memory info ================
     sprintf(printArray,"\nTime: %.6lf, OS: Begin PCB Creation",time/CLOCKS_PER_SEC);
     strcat(printingLine, printArray);
     clean(printArray,printArraySize);
     strcpy(currentLN->data,printingLine);
     currentLN = addLogNode(currentLN);  
     clean(printingLine,printArraySize);
     // ==================================================================



     // get the number of processes within the meta data     
     numberOfProcess = getNumberOfProcess(metaData);
     if(numberOfProcess < 0)
       {
          return numberOfProcess;  
       }

     // once the number of processes are known, make an array
     // with the saize equal to the number of process
     struct pcb processes[numberOfProcess];
     struct forThread waitQueue[numberOfProcess];
   
     // this is an array pointer, so that I can pass it though a
     // method and fill it with data.
     struct pcb *arrayPrt = &processes[0];
     struct forThread *waitPrt = &waitQueue[0];

     
     // get process and set it to new
     startTime = clock();
     // fill the process array with newly made PCB!
     setProcessesNew(arrayPrt,metaData, configData);
     endTime = clock();
     time += ((double) (endTime-startTime));


     if(howToPrint == printTM || howToPrint == printB)
       {  
        printf("\nTime: %.6lf, OS: All processes initialized in New state", time/CLOCKS_PER_SEC);
       }
     //====this statement adds to the logNode memory info ================
     sprintf(printArray,"\nTime: %.6lf, OS: All processes now set in New state",time/CLOCKS_PER_SEC);
     strcat(printingLine, printArray);
     clean(printArray,printArraySize);
     strcpy(currentLN->data,printingLine);
     currentLN = addLogNode(currentLN);  
     clean(printingLine,printArraySize);
     // ==================================================================




     // set all processes to a ready state.
     startTime = clock();
     arrayPrt = &processes[0];
     setProcessesReady(arrayPrt,numberOfProcess);
     endTime = clock();
     time += ((double) (endTime-startTime));

  
     if(howToPrint == printTM || howToPrint == printB)
       {  
        printf("\nTime: %.6lf, OS: All processes now set in Ready state", time/CLOCKS_PER_SEC);
       }
    
     //====this statement adds to the logNode memory info ================
     sprintf(printArray,"\nTime: %.6lf, OS: All processes now set in Ready state",time/CLOCKS_PER_SEC);
     strcat(printingLine, printArray);
     clean(printArray,printArraySize);
     strcpy(currentLN->data,printingLine);
     currentLN = addLogNode(currentLN);  
     clean(printingLine,printArraySize);
     // ==================================================================




     
     while(processCounter != numberOfProcess)
       {
          // take a process and set it to running
          // then Run that process
          startTime = clock();

          // here i will be picking a process to run with regard to the
          // shecduling algorithm i nthe config file.
          index = pickProcess(processes,configData, numberOfProcess);


          clean(processes[index].state, 7);
          endTime = clock();
          time+= (endTime - startTime);
          
          startTime = clock();
          if(howToPrint == printTM || howToPrint == printB)
            {
             printf("\nTime: %.6lf, OS: %s ",time/CLOCKS_PER_SEC,configData.cpuSchedulingCodeData);
             printf("picked process %d",index);
             printf(" with time %d (mSec)",processes[index].time);
            }
          //====this statement adds to the logNode memory info ================
          sprintf(printArray,"\nTime: %.6lf, OS: %s picked process %d with time %d (mSec)",time/CLOCKS_PER_SEC,configData.cpuSchedulingCodeData,index,processes[index].time);
          strcat(printingLine, printArray);
          clean(printArray,printArraySize);
          strcpy(currentLN->data,printingLine);
          currentLN = addLogNode(currentLN);
          clean(printingLine,printArraySize);
          // ==================================================================






          strcpy(processes[index].state, "running"); 
          endTime = clock();

          time+= (endTime - startTime);

          if(howToPrint == printTM || howToPrint == printB)
            {
             printf("\n\nTime: %.6lf, OS: processes %d ", time/CLOCKS_PER_SEC, index);
             printf("set in Running State");
            }

          //====this statement adds to the logNode memory info ================
          sprintf(printArray,"\n\nTime: %.6lf, OS: Process %d,  set in Running State",time/CLOCKS_PER_SEC,index);
          strcat(printingLine, printArray);
          clean(printArray,printArraySize);
          strcpy(currentLN->data,printingLine);
          currentLN = addLogNode(currentLN);
          clean(printingLine,printArraySize);
          // ==================================================================






          // read the process now. aka running
          readProcess(&(processes[index].first),configData , &time, index,howToPrint,
                        numberArraySize,printingLine,numberArray,&currentLN,
                        &(processes[index].time), waitPrt);

          // set the finished process to an exit state
          startTime = clock();


          if(processes[index].time > 0)
          {
             clean(processes[index].state, 7);
             strcpy(processes[index].state, "ready"); 

             if(howToPrint == printTM || howToPrint == printB)
               {
                 printf("\nTime: %.6lf, OS: processes %d ", time/CLOCKS_PER_SEC, index);
                 printf("set in Ready State"); 
               }
          }
          else
          {  
             processCounter++;
             clean(processes[index].state, 7);
             strcpy(processes[index].state, "exit");

             if(howToPrint == printTM || howToPrint == printB)
               {
                  printf("\nTime: %.6lf, OS: processes %d ", time/CLOCKS_PER_SEC, index);
                  printf("set in Exit State"); 
               }
          }




          endTime = clock();

          time+= (endTime - startTime);






          //====this statement adds to the logNode memory info ================
          sprintf(printArray,"\nTime: %.6lf, OS: Process %d, set in Exit State",time/CLOCKS_PER_SEC,index);
          strcat(printingLine, printArray);
          clean(printArray,printArraySize);
          strcpy(currentLN->data,printingLine);
          currentLN = addLogNode(currentLN);
          clean(printingLine,printArraySize);
          // ==================================================================

         
          startTime = clock();
       }
     endTime = clock();
     time+= (endTime - startTime);


     if(howToPrint == printTM || howToPrint == printB)
       {
        printf("\nTime: %.6lf,OS: System end", time/CLOCKS_PER_SEC);
       }
          //====this statement adds to the logNode memory info ================
          sprintf(printArray,"\nTime: %.6lf, OS: System end",time/CLOCKS_PER_SEC);
          strcat(printingLine, printArray);
          clean(printArray,printArraySize);
          strcpy(currentLN->data,printingLine);
          currentLN = addLogNode(currentLN);
          clean(printingLine,printArraySize);
          // ==================================================================


     if(howToPrint == printTM || howToPrint == printB)
       {
        printf("\n\nEnd Simulation");
        printf("\n==========================\n");
       }

     if(howToPrint == printTL || howToPrint == printB)
       {
        if(howToPrint == printTL)
          {
           printf("finished, check the log\n");
          }
        writeToLog(ln,configData);
       }
     return 0;

  }





// this method looks at the element of the meta data
// if the element has S(start)0 we are good to go,
// else error out

int getStart(struct Node *data, int cTimeMultipler)
  {
     char cL = data->componentLetter; 
     char oStr[12];
     strcpy(oStr, data->oprationString);
     
     if(cL == 'S' && (strcmp(oStr,"start")) == 0)
       {
          return 0;
       }
     else
       {
          return -3; // this is an error int to stop the program.
       }
  }


// this method looks at the meta data to see if there is an equal
// amount of A(start)0 and A(end)0.  this is to test to see if
// we can make valid processes, if not error out
// if so return the number of processes 

int getNumberOfProcess(struct Node *metaData)
  {
     struct Node *current = metaData;
     int numberOfAStarts = 0; 
     int numberOfAEnds = 0;

     // loop though the meta data
     while(current->nextNode != NULL)
       {
          // if the meta data element component letter is an A
          // see if the opration string is a start or end
          if(current->componentLetter == 'A')
             {
                // if start, number of A starts gose up
                if((strcmp(current->oprationString,"start")) == 0)
                  {
                     numberOfAStarts++;
                  }
                // if end, number of A ends gose up
                if((strcmp(current->oprationString,"end")) == 0)
                  {
                     numberOfAEnds++;
                  }
             }
          // move on in the list
          current = current->nextNode;
       }
     // at the end test to see if we are equal in the number of
     // A starts and A ends if not error out
     if(numberOfAStarts != numberOfAEnds)
       {
          printf("\nError");
          return -1;
       }
     // else return the number of processes
     return numberOfAStarts;
  }


// this methods will parse though the meta data and place all
// processes into a PCB and set it to a new state
void setProcessesNew(struct pcb *process,struct Node *data, struct configStruct configData)
  {
     // needed variables
     struct Node *first = (struct Node*)malloc (sizeof(struct Node));   
     struct Node *current = first, *dataPrt = data;
     int index = 0;
     int timeTotal = 0;

     
     // loop though the meta data until there is no more
     while(dataPrt->nextNode != NULL)
       {
       // if we are not at A(start), keep looping until we are
       while(dataPrt->componentLetter != 'A')
         {
            dataPrt = dataPrt->nextNode;

         } 
       // move the pointer for the meta data by one
       // to get to the start of a process
       dataPrt = dataPrt->nextNode;

       // while looping though a process copy the info of that process in to
       // a new link list for the process.
       while(dataPrt->componentLetter != 'A' || 
                (strcmp(dataPrt->oprationString,"end")) != 0)
         {
             current->componentLetter = dataPrt->componentLetter;
             strcpy(current->oprationString,dataPrt->oprationString);
             current->cycleTime = dataPrt->cycleTime;

             if(current->componentLetter == 'I' ||
                   current->componentLetter == 'O')
               {
                  timeTotal += current->cycleTime * configData.ioCycleTimeData;
               }
             if(current->componentLetter == 'P')
               {          
                  timeTotal += current->cycleTime * configData.processorCycleTimeData;
               }
             current = addNode(current);
             dataPrt = dataPrt->nextNode;
         }
       // force the next node to be null to be safe
       current->nextNode = NULL;
       // move the meta data pointer by one to go over the A end element
       dataPrt = dataPrt->nextNode;
       // if we are at S(end)0, move the meta data pointer by one
       // to kill the while loop on the next updata check
       // else there are more processes, get them
       if((strcmp(dataPrt->oprationString,"end")) == 0 &&
                  dataPrt->componentLetter == 'S')
         {
            dataPrt = dataPrt->nextNode;
         }
       // put the list into the PCB and up data it to new state
       process[index].first = first;
       clean(process[index].state, 7);
       strcpy(process[index].state, "new");
       process[index].processNum = index;
       process[index].time = timeTotal;
       index++;
       
       // reset the the link list for more data
       first = (struct Node*)malloc (sizeof(struct Node));
       current = first;
       timeTotal = 0;
       }     
  }


// loop though the PCB array and set all processes to ready
void setProcessesReady(struct pcb *process, int numOfProcess)
  {
     int counter = 0;
     for(counter = 0; counter < numOfProcess;counter++)
       {
          clean(process[counter].state, 7);
          strcpy(process[counter].state, "ready");
       }
  }

// this method reads though a process




int readProcess
(
struct Node **first,struct configStruct configData, double *time, int count,int printType,
int numberArraySize, char *printingLine, char *numberArray, struct logNode **currentLN,
int *pcbTime, struct forThread *arrayPrt
)
  {
     // set up needed variables
     struct Node **current = first;
     char comL, opStr[12];
     int  cyT = 0, proT = configData.processorCycleTimeData;
     int  iOT = configData.ioCycleTimeData;
     int test;
     
     // proTime and iOTime are in msec, converting to microseconds
     // this is because on how I have my program wait.
     // look at mytimr.c for more info
     proT = proT*1000;  
     iOT = iOT*1000;
     
     // read though the process data a deal with a specific part of the process.
     while((*current)->nextNode != NULL)
       {
          // get the data of the process
          comL= (*current)->componentLetter;
          strcpy(opStr, (*current)->oprationString);
          cyT = ((*current)->cycleTime);
          
          // this method knows how to treat that process
          test = startAction(comL,opStr,&cyT,proT,iOT,time,count,printType,
                           numberArraySize,printingLine,numberArray,currentLN,configData,pcbTime
                           ,arrayPrt);
          
          if(test == qtTimedOut)
          {
             **first = **current;
             ((*current)->cycleTime) = cyT;

             return qtTimedOut;
          }
          else
          {
             *current = (*current)->nextNode;
             **first = **current;
             return commandEnded;
          }
          // move the pointer
          
          
       }
  return commandEnded;

  }


// this method knows how to treat a specific action of a process
int startAction
(
char comL,char *opStr,int *cyT,int proT,int iOT,double *time,int count, int printType,
int numberArraySize, char *printingLine, char *numberArray, struct logNode **currentLN,
struct configStruct configData, int *pcbTime, struct forThread *arrayPrt
)
  {
     clock_t startTime, endTime;
     pthread_t thread;
     pthread_attr_t attr;
     pthread_attr_init(&attr);
     char printArray[80];
     int printArraySize = 80;
     int threadTime;
     struct forThread *threadData = (struct forThread*)malloc (sizeof(struct forThread));


     startTime = clock();
     // if the component Letter is a M, memory, do this
     if(comL =='M')
       {
         endTime = clock();
         *time += (endTime - startTime);
        

         if(printType == printTM || printType == printB)
           {
            printf("\nTime: %.6lf, Process %d, ",*time/CLOCKS_PER_SEC,count);
            printf("Memory management %s action start",opStr);
           }
          //====this statement adds to the logNode memory info ================
          sprintf(printArray,"\nTime: %.6lf, Process %d, Memory management %s action start",*time/CLOCKS_PER_SEC,count,opStr);
          strcat(printingLine, printArray);
          clean(printArray,printArraySize);
          strcpy((*currentLN)->data,printingLine);
          *currentLN = addLogNode(*currentLN);
         
          clean(printingLine,printArraySize);
          // ==================================================================

         *time += (*cyT);
         if(printType == printTM || printType == printB)
           {
            printf("\nTime: %.6lf, Process %d, ",*time/CLOCKS_PER_SEC,count);
            printf("Memory management %s action end",opStr);
           }
          //====this statement adds to the logNode memory info ================
          sprintf(printArray,"\nTime: %.6lf, Process %d, Memory management %s action end",*time/CLOCKS_PER_SEC,count,opStr);
          strcat(printingLine, printArray);
          clean(printArray,printArraySize);
          strcpy((*currentLN)->data,printingLine);
          *currentLN = addLogNode(*currentLN);
          
          clean(printingLine,printArraySize);
          // ==================================================================
          myWait((*cyT));
       }

     // if the component Letter is a O, output, do this
     if(comL =='O')
       {
         endTime = clock();
         *time += (endTime - startTime);
         if(printType == printTM || printType == printB)
           {
            printf("\nTime: %.6lf, Process %d, ",*time/CLOCKS_PER_SEC,count);
            printf("%s output start",opStr);
           }

          //====this statement adds to the logNode memory info ================
          sprintf(printArray,"\nTime: %.6lf, Process %d, %s output start",*time/CLOCKS_PER_SEC,count,opStr);
          strcat(printingLine, printArray);
          clean(printArray,printArraySize);
          strcpy((*currentLN)->data,printingLine);
          *currentLN = addLogNode(*currentLN);
          clean(printingLine,printArraySize);
          // ==================================================================
        
         *time += ((*cyT)*iOT);
         if(printType == printTM || printType == printB)
           {  
            printf("\nTime: %.6lf, Process %d, ",*time/CLOCKS_PER_SEC,count);   
            printf("%s output end",opStr);
           }

          //====this statement adds to the logNode memory info ================
          sprintf(printArray,"\nTime: %.6lf, Process %d, %s output end",*time/CLOCKS_PER_SEC,count,opStr);
          strcat(printingLine, printArray);
          clean(printArray,printArraySize);
          strcpy((*currentLN)->data,printingLine);
          *currentLN = addLogNode(*currentLN);
        //  clean(printingLine,printArraySize);
          // ==================================================================

         threadTime = (int)((*cyT)*iOT);
         threadData->processID = count;
         threadData->processWaitTime = threadTime;
         clean(threadData->printLine,80);
         strcpy(threadData->printLine,printingLine);
         threadData->waitPrt = arrayPrt;         


         clean(printingLine,printArraySize);
         pthread_create(&thread, &attr, myTWait,(void *)threadData);
         pthread_join(thread,NULL);
         *pcbTime -= threadTime/1000;
       }
     // if the component Letter is a I, input, do this
     if(comL =='I')
       {
         endTime = clock();
         *time += (endTime - startTime);

         if(printType == printTM || printType == printB)
           {
            printf("\nTime: %.6lf, Process %d, ",*time/CLOCKS_PER_SEC,count);
            printf("%s intput start",opStr);
           }

          //====this statement adds to the logNode memory info ================
          sprintf(printArray,"\nTime: %.6lf, Process %d, %s intput start",*time/CLOCKS_PER_SEC,count,opStr);
          strcat(printingLine, printArray);
          clean(printArray,printArraySize);
          strcpy((*currentLN)->data,printingLine);
          *currentLN = addLogNode(*currentLN);
          clean(printingLine,printArraySize);
          // ==================================================================

         *time += ((*cyT)*iOT);
         if(printType == printTM || printType == printB)
           {
            printf("\nTime: %.6lf, Process %d, ",*time/CLOCKS_PER_SEC,count);
            printf("%s intput end",opStr);
           }
          //====this statement adds to the logNode memory info ================
          sprintf(printArray,"\nTime: %.6lf, Process %d, %s intput end",*time/CLOCKS_PER_SEC,count,opStr);
          strcat(printingLine, printArray);
          clean(printArray,printArraySize);
          strcpy((*currentLN)->data,printingLine);
          *currentLN = addLogNode(*currentLN);
  //        clean(printingLine,printArraySize);
          // ==================================================================

  
       threadTime = (int)((*cyT)*iOT);

//this is :    I


        threadData->processID = count;
        threadData->processWaitTime = threadTime;
        clean(threadData->printLine,80);
        strcpy(threadData->printLine,printingLine);
        threadData->waitPrt = arrayPrt;  

        pthread_create(&thread, &attr, myTWait,(void *)threadData);
        pthread_join(thread,NULL);
        *pcbTime -= threadTime/1000;
        clean(printingLine,printArraySize);
        


       }
     // if the component Letter is a P, process, do this
     if(comL =='P')
       {
         endTime = clock();
         *time += (endTime - startTime);

         if(printType == printTM || printType == printB)
           {
            printf("\nTime: %.6lf, Process %d, ",*time/CLOCKS_PER_SEC,count);
            printf("Run operation start");
           }


          //====this statement adds to the logNode memory info ================
          sprintf(printArray,"\nTime: %.6lf, Process %d,  Run operation start",*time/CLOCKS_PER_SEC,count);
          strcat(printingLine, printArray);
          clean(printArray,printArraySize);
          strcpy((*currentLN)->data,printingLine);
          *currentLN = addLogNode(*currentLN);
          clean(printingLine,printArraySize);
          // ==================================================================







  
         *time += ((*cyT)*proT);


         if((*cyT)* (proT/1000) > configData.quantumTimeData)
         {


            *cyT = ((*cyT) * proT/1000  - configData.quantumTimeData)/10;


            myWait(configData.quantumTimeData*1000);
            *pcbTime -= (configData.quantumTimeData);
         if(printType == printTM || printType == printB)
           {  
            printf("\nTime: %.6lf, Process %d, ",*time/CLOCKS_PER_SEC,count);
            printf("Run operation end");
           }

            return qtTimedOut;
         }
         else
         {
	    myWait((*cyT)*proT);
            *pcbTime -= (*cyT)*proT/1000;

         }



         if(printType == printTM || printType == printB)
           {  
            printf("\nTime: %.6lf, Process %d, ",*time/CLOCKS_PER_SEC,count);
            printf("Run operation end");
           }










          //====this statement adds to the logNode memory info ================
          sprintf(printArray,"\nTime: %.6lf, Process %d, Run operation end",*time/CLOCKS_PER_SEC,count);
          strcat(printingLine, printArray);
          clean(printArray,printArraySize);
          strcpy((*currentLN)->data,printingLine);
          *currentLN = addLogNode(*currentLN);
          clean(printingLine,printArraySize);
          // ==================================================================
       }
     return commandEnded;
  }

// this method adds a new node to my log struct
struct logNode *addLogNode(struct logNode *current)
  {
     struct logNode *newNode = (struct logNode *) malloc (sizeof(struct logNode));
     newNode->nextNode = NULL;
     current->nextNode = newNode;
     return newNode;

  }

// this method will take the log information and write it to a file
void writeToLog (struct logNode *first, struct configStruct configData)
  { 
    FILE *fPrt;
    struct logNode *current = first;
    fPrt = fopen(configData.logFilePathData,"w");
    while(current->nextNode != NULL)
      {
         fprintf(fPrt,"%s",current->data);
         current = current->nextNode;
      }
    fclose(fPrt);
  }


// this method will pick a process be the scheduling algorithem in the config file.
// how this works is I check to see what algorithem is in config file
// then i will pass my pcb array in the right function to get the index of the next process
// all scheduling algorithems ignore PCB in exit state.S
int pickProcess(struct pcb array[], struct configStruct configData, int size)
  {
   int scheduling = 0, fcfsn = 1000, sjfn = 2000;
   int index = 0;  
   if((strcmp(configData.cpuSchedulingCodeData, "FCFS-N")) == 0) 
     {   
       scheduling = fcfsn;
     }
   if((strcmp(configData.cpuSchedulingCodeData, "SJF-N")) == 0) 
     {   
       scheduling = sjfn;
     }

   switch(scheduling)
     {
       case (1000) :
            index = getFCFSN(array,size);
            break;
       case (2000) :
            index = getSJFN(array,size);
            break;
     }

    return index;
  }
  

// this method returns the index of the process with FCFS in mind
int getFCFSN(struct pcb array[], int size)
  {
   int count = 0;

   for(count = 0; count < size; count++)
     {
      if((strcmp((array[count].state), "exit")) != 0)
      {
        return count;
      }
     }
   return 0;
  }

// this method returns the index of the process with SJF in mind.
int getSJFN(struct pcb array[], int size)
  {
   // the min time is unknow, so i picked 5,000,000 to be the stating min
   // because i know no processes will be longer than that.
   int min = 5000000, index = 0, count;
   
   for(count = 0; count < size; count++)
     {
      if((strcmp((array[count].state), "exit")) != 0)
      {
        if(array[count].time < min)
        {
           min = array[count].time;
           index = count;
        }
      }
     }

 
   return index;
  }







