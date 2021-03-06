// Simulator Program Header Information////
/**
  * @file simulator.h
  *
  * @brief header for simulator.c that defines PCB_H, loads dependencies, prototypes functions
  */


#ifndef PCB_H
#define PCB_H

#include "myTimer.h"
#include "parMethods.h"
#include <string.h>

// data types
struct pcb
  {
     struct Node *first;
     char state[7]; //  new, ready, running, exit
     int processNum;
     int time;
  };

struct logNode
  {
     char data[80]; // this is 80 charaters because 80 is a nice size
     struct logNode *nextNode;
  };


struct forThread
   {
     char printLine[80];
     int processID;
     int processWaitTime;
     int howToPrint;
     struct forThread *waitPrt;
   };

struct logNode *addLogNode(struct logNode *current);

enum massage_codes { unk = -1000, printTM, printTL, printB, qtTimedOut,
                     commandEnded, setToBlocked, isLock, isUnlock, allBlocked,
                     notAllBlocked, NULL_PROCESS_ID, waitQueueSignal, allExit,
                     memoryTimeOut};

// function prototypes
int startSim(struct Node *metaData,struct configStruct configData);

int getStart(struct Node *data, int cTimeMultipler);

void setProcessesNew(struct pcb *process,struct Node *data, struct configStruct configData);

void setProcessesReady(struct pcb *process, int numOfProcess);

int  getNumberOfProcess(struct Node *metaData);

int readProcess
(
     struct Node **first, struct configStruct configData, double *time, int count, int printType,
     int numberArraySize, char *printingLine, char *numberArray, struct logNode **currentLN,
     int *pcbTime, struct forThread *arrayPrt, int numOfProcess, struct pcb *pcbArray
);

int startAction
(
     char comL,char *opStr,int *cyT,int proT,int iOT,double *time,int count,int printType,
     int numberArraySize, char *printingLine, char *numberArray, struct logNode **currentLN,
     struct configStruct configData, int *pcbTime, struct forThread *arrayPrt,
     int numOfProcess
);


int pickProcess
(
     struct pcb array[], struct configStruct configData, int size,
     struct forThread *data
);

void writeToLog (struct logNode *first,struct configStruct configData);

int getNextIndexInWaitQueue(struct forThread *data, int size);

int getFCFSN(struct pcb array[], int size);

int getSJFN(struct pcb array[], int size);

int getFCFSP(struct pcb array[], int size);

int getRRP(struct pcb array[], int size);

void threadHandler(struct forThread *data);

void setToNull(struct forThread *data, int size);

int countWaitQueue(struct forThread *data, int size);

void unblockProcess(struct pcb array[], int index, int subTime);

void setIndexToNull (struct forThread *data, int index);

#endif

