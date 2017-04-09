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
     int howToPrint = 0, test = 0;
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
     setToNull(waitPrt, numberOfProcess);

     
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


     // at this point all processes have been made and put in to pcb.
     // all pcb are set to ready
     // this while loop will run the processes.
     //
     // step one get an index for a pcb
     //      this is based off of the scheduleing algorithem in the config file
     //      once an index is found get that pcb ready for running
     //          NOTE  this is where we might want to have the OS wait if all processes are in blocked state.
     //
     // step two put the selected pcb in runner state and run it
     //       in the running state this is where all of the magic happens
     //       this is also where the we need to edit/add to our program to finish sim04.  there are other places where code is needed but this is the main place.
     //       there are two main funcitons for this step:  readProcess  and  startAction
     //       
     //              readProcess takes in the selected PCB, and much much more, and reads the op commands from it.
     //                once a an op command is ready to run then readProess calls startAction
     //
     //              startAction takes the needed infomation about an op command and deals with that command.
     //                 here is where we need to add code for each section.

     
     while(processCounter != numberOfProcess)
       {
          // take a process and set it to running
          // then Run that process
          startTime = clock();
           

          //     ================   Step one  ====================
          // here i will be picking a process to run with regard to the
          // shecduling algorithm i nthe config file.
          index = pickProcess(processes,configData, numberOfProcess, waitPrt);
          if(index == allExit)
            {

              break;
            }

          clean(processes[index].state, 7);
          endTime = clock();
          time+= (endTime - startTime);
          
          startTime = clock();
          if(howToPrint == printTM || howToPrint == printB)
            {
             printf("\n\nTime: %.6lf, OS: %s ",time/CLOCKS_PER_SEC,configData.cpuSchedulingCodeData);
             printf("picked process %d",index);
             printf(" with time %d (mSec)",processes[index].time);
            }
          //====this statement adds to the logNode memory info ================
          sprintf(printArray,"\n\nTime: %.6lf, OS: %s picked process %d with time %d (mSec)",time/CLOCKS_PER_SEC,configData.cpuSchedulingCodeData,index,processes[index].time);
          strcat(printingLine, printArray);
          clean(printArray,printArraySize);
          strcpy(currentLN->data,printingLine);
          currentLN = addLogNode(currentLN);
          clean(printingLine,printArraySize);
          // ==================================================================


          // ================ End of Step one =================================


          // ====================== start of Step 2 ===========================
  

          strcpy(processes[index].state, "running"); 
          endTime = clock();

          time+= (endTime - startTime);

          if(howToPrint == printTM || howToPrint == printB)
            {
             printf("\nTime: %.6lf, OS: processes %d ", time/CLOCKS_PER_SEC, index);
             printf("set in Running State");
            }

          //====this statement adds to the logNode memory info ================
          sprintf(printArray,"\nTime: %.6lf, OS: Process %d,  set in Running State",time/CLOCKS_PER_SEC,index);
          strcat(printingLine, printArray);
          clean(printArray,printArraySize);
          strcpy(currentLN->data,printingLine);
          currentLN = addLogNode(currentLN);
          clean(printingLine,printArraySize);
          // ==================================================================






          // read the process now. aka running
          test = readProcess(&(processes[index].first),configData , &time, index,
                             howToPrint, numberArraySize,printingLine,numberArray,
                             &currentLN, &(processes[index].time), waitPrt,
                             numberOfProcess, arrayPrt);
          if(test == setToBlocked)
            {
              clean(processes[index].state, 7);
              strcpy(processes[index].state, "block");
              printf("\nprocess %d is now blocked", index);
              test = 0;
            }

          // set the finished process to an exit state
          startTime = clock();

          // ========================  End of step 2 ==========================

          // ========================  Start of Step 3 ========================

          if(processes[index].time > 0 && 
                            (strcmp((processes[index].state), "block")) != 0)
          {
             clean(processes[index].state, 7);
             strcpy(processes[index].state, "ready"); 


             if(howToPrint == printTM || howToPrint == printB)
               {
                 printf("\nTime: %.6lf, OS: processes %d ", time/CLOCKS_PER_SEC, index);
                 printf("set in Ready State"); 
               }
          }
          else if(processes[index].time <= 0 )
          {  
             processCounter++;
             clean(processes[index].state, 7);
             strcpy(processes[index].state, "exit");

             if(howToPrint == printTM || howToPrint == printB)
               {
                  printf("\nTime: %.6lf, OS: processes %d ", time/CLOCKS_PER_SEC, index);
                  printf("set in Exit State"); 
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

          }


          
          // =================== End of Step 3 ================================
          startTime = clock();
myWait(100000);
       }
     endTime = clock();
     time+= (endTime - startTime);


     if(howToPrint == printTM || howToPrint == printB)
       {
        printf("\nTime: %.6lf, OS: System end", time/CLOCKS_PER_SEC);
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
int *pcbTime, struct forThread *arrayPrt, int numOfProcess, struct pcb *pcbArray
)
  {
     // set up needed variables
     struct Node **current = first;
     char comL, opStr[12];
     int  cyT = 0, proT = configData.processorCycleTimeData;
     int  iOT = configData.ioCycleTimeData;
     int test, processesWaiting, loopCount = 0;
     
     // proTime and iOTime are in msec, converting to microseconds
     // this is because on how I have my program wait.
     // look at mytimr.c for more info
     proT = proT*1000;  
     iOT = iOT*1000;
     
     // read though the process data a deal with a specific part of the process.
     while(*current != NULL)
       {
          // get the data of the process
          comL= (*current)->componentLetter;
          strcpy(opStr, (*current)->oprationString);
          cyT = ((*current)->cycleTime);
          
          // this method knows how to treat that process
          test = startAction(comL,opStr,&((*current)->cycleTime),proT,iOT,time,count,printType,
                           numberArraySize,printingLine,numberArray,currentLN,configData,pcbTime
                           ,arrayPrt, numOfProcess);
          
          if(test == qtTimedOut)
          {
             **first = **current;  
             return qtTimedOut;
          }
          else if(test == setToBlocked)
          {
             *current = (*current)->nextNode;
             **first = **current;

             return setToBlocked;
          }
          else if( test == waitQueueSignal)
          {
             processesWaiting = countWaitQueue(arrayPrt, numOfProcess);
             for(loopCount = 0; loopCount < processesWaiting; loopCount++)
               { 
                 unblockProcess(pcbArray, getNextIndexInWaitQueue(arrayPrt,numOfProcess), 0);
                 setIndexToNull (arrayPrt, getNextIndexInWaitQueue(arrayPrt,numOfProcess));
               }
             **first = **current;
             return qtTimedOut;
          }
          else
          {
             *current = (*current)->nextNode;
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
struct configStruct configData, int *pcbTime, struct forThread *arrayPrt,
int numOfProcess
)
  {
     clock_t startTime, endTime;
     pthread_t thread;
     pthread_attr_t attr;
     pthread_attr_init(&attr);
     char printArray[80];
     int printArraySize = 80, test = 0;
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
        //    printf("\nTime: %.6lf, Process %d, ",*time/CLOCKS_PER_SEC,count);   
        //    printf("%s output end",opStr);
           }

          //====this statement adds to the logNode memory info ================
          sprintf(printArray,"\nProcess %d, %s output end",count,opStr);
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
         threadData->howToPrint = printType;       


         clean(printingLine,printArraySize);
         pthread_create(&thread, &attr, myTWait,(void *)threadData);
         //pthread_join(thread,NULL);
         //*pcbTime -= threadTime/1000;
         return setToBlocked;
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
        threadData->howToPrint = printType;


        clean(printingLine,printArraySize);
        

         pthread_create(&thread, &attr, myTWait,(void *)threadData);

         return setToBlocked;


       }
     // if the component Letter is a P, process, do this
     if(comL =='P')
       {
         int loopCount = 0, counter = 0;
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


         loopCount = configData.quantumTimeData/configData.processorCycleTimeData;
        
         if(loopCount > *cyT)
           {
             loopCount = *cyT;
           }



         for(counter = 0; counter < loopCount; counter++)
           {
             *cyT = *cyT - 1;
             *pcbTime = *pcbTime - configData.processorCycleTimeData;
             *time += configData.processorCycleTimeData;
             // do the check
             // do the wait a cycle
             myWait(configData.processorCycleTimeData*1000);
             // update the time, P(run) 7 to 6

             test = countWaitQueue(arrayPrt, numOfProcess);
             // if true, leave with updating data
             if(test < 0)
               { 
                 test = 0;
                 if(printType == printTM || printType == printB)
                  {  
                   printf("\nTime: %.6lf, Process %d, ",*time/CLOCKS_PER_SEC,count);
                   printf("Run operation end");
                  }
                 //====this statement adds to the logNode memory info ================
                 sprintf(printArray,"\nTime: %.6lf, Process %d, Run operation end",*time/            CLOCKS_PER_SEC,count);
                 strcat(printingLine, printArray);
                 clean(printArray,printArraySize);
                 strcpy((*currentLN)->data,printingLine);
                 *currentLN = addLogNode(*currentLN);
                 clean(printingLine,printArraySize);
                 // ==================================================================
               
                 return waitQueueSignal;
               }
             // false, continue.
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

          if(*cyT > 0)
            {
              return qtTimedOut;
            } 
          else
            {
              return commandEnded;
            }

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
int pickProcess
(
struct pcb array[], struct configStruct configData, int size,
struct forThread *data
)
  {
   int scheduling = 0, fcfsn = 1000, sjfn = 2000, fcfsp = 3000;
   int index = 0, test, subTime = 0, loopCounter = 0;
   int processIndex = 0;
  
   if((strcmp(configData.cpuSchedulingCodeData, "FCFS-N")) == 0) 
     {   
       scheduling = fcfsn;
     }
   if((strcmp(configData.cpuSchedulingCodeData, "SJF-N")) == 0) 
     {   
       scheduling = sjfn;
     }
   if((strcmp(configData.cpuSchedulingCodeData, "FCFS-P")) == 0) 
     {   
       scheduling = fcfsp;
     }

   switch(scheduling)
     {
       case (1000) :
            index = getFCFSN(array,size);
            break;
       case (2000) :
            index = getSJFN(array,size);
            break;
       case (3000) :
            index = getFCFSP(array,size);
            break;
     }
    
    if(index == allBlocked)
      {
        while(index == allBlocked)
          {

            printf("\n all processes are blocked");

           myWait(1000000);

            test = countWaitQueue(data,size);
            if(test != -1)
              {
               for(loopCounter = 0; loopCounter < test; loopCounter++)
                {

                 processIndex =getNextIndexInWaitQueue(data,size);
                 subTime = data[processIndex].processWaitTime/1000;
                 unblockProcess(array,processIndex,subTime);
                 setIndexToNull(data,processIndex);
                }
                index = 0;
              }
          }
        
        pickProcess(array,configData,size,data);
      }
    else if(index == allExit)
     {
       return allExit;
     }
    return index;
  }
  

// this method returns the index of the process with FCFS-N in mind
int getFCFSN(struct pcb array[], int size)
  {
   int count = 0;
   static int lastIndex = -1;
   for(count = 0; count < size; count++)
     {
      if((strcmp((array[count].state), "exit")) != 0 &&
          (strcmp((array[count].state), "block")) != 0 &&
           count > lastIndex)
      {
        if((count+1) == size)
        {
          lastIndex = -1;
        }
        else
        {
          lastIndex = count;
        }
        return count;
      }
     }
   return allBlocked;
  }

int getFCFSP(struct pcb array[], int size)
  {
     int count = 0, index = -1;
     int isExit = 0, isBlocked =0;
  
     for(count = 0; count < size; count++)
       {
        if((strcmp((array[count].state), "exit")) != 0 &&
            (strcmp((array[count].state), "block")) != 0 &&
              index == -1)
          {
             index = count;
          }
        if((strcmp((array[count].state), "exit")) == 0)
          {
            isExit++;
          }
        if((strcmp((array[count].state), "block")) == 0)
          {
            isBlocked++;
          }
       }

     if(isExit == (size - 1))
       {
          return allExit;
       }
     else if(isBlocked == (size - 1) || (isBlocked + isExit) == (size - 1))
       {
          return allBlocked;
       }
     else
       {
          return index;
       }
  }

// this method returns the index of the process with SJF-N in mind.
int getSJFN(struct pcb array[], int size)
  {
   // the min time is unknow, so i picked 5,000,000 to be the stating min
   // because i know no processes will be longer than that.
   int min = 5000000, index = 0, count;
   
   for(count = 0; count < size; count++)
     {
      if((strcmp((array[count].state), "exit")) != 0 &&
          (strcmp((array[count].state), "block")) != 0)
      {
        if(array[count].time < min)
        {
           min = array[count].time;
           index = count;
        }
      }
     }
   return allBlocked;
  }


void threadHandler (struct forThread *data)
  {
   static int lock  = isUnlock;
  
   if(lock == isLock)
     {
      while(lock == isLock)
        {
         
        }
     }

     lock = isLock;

     data->waitPrt[data->processID] = *data;
     
     lock = isUnlock;
  }
     
void setToNull(struct forThread *data, int size)
  {
    int count = 0;
    for(count = 0; count < size; count++)
      {
        struct forThread dummieData;
        dummieData.processID = NULL_PROCESS_ID;
        data[count] = dummieData;
      }
  }


int getNextIndexInWaitQueue(struct forThread *data, int size)
   {
    int index = 0, count = 0;
    for(index = 0; index < size; index++)
      {
        if(data[index].processID != NULL_PROCESS_ID)
          {
             return index;
          }
      }
    return count;
   }
// get the index of a process in the waiting queue,, this is might not needed
int countWaitQueue(struct forThread *data, int size)
  {
    int index = 0, count;
    for(index = 0; index < size; index++)
      {
        if(data[index].processID != NULL_PROCESS_ID)
          {
             count++;
          }
      }
    return count;
  }



void unblockProcess(struct pcb array[], int index, int subTime)
  {
     if((array[index].time - subTime) > 0)
       {
        printf("\n\n Process %d is set to Ready state",array[index].processNum);
        clean(array[index].state, 7);
        strcpy(array[index].state, "ready");
        array[index].time -= subTime;
       }
     else
       {
        printf("\n\n Process %d is set to Exit state",array[index].processNum);
        clean(array[index].state, 7);
        strcpy(array[index].state, "exit");
        array[index].time -= subTime;
       } 
   
  }

void setIndexToNull (struct forThread *data, int index)
  {
     struct forThread dummieData;
     dummieData.processID = NULL_PROCESS_ID;
     data[index] = dummieData;
  }





// openmp ,, mpi








