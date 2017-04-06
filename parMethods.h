#ifndef PAR_METHODS_H
#define PAR_METHODS_H
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/*
 * structers used in storing data
 * the Node Struct is used as a link list for meta data
 * the ConfigNode is used for holding the config file data
 */



struct Node
{
   char componentLetter;        // component letter for meta data
   char oprationString[12];     // operation string for meta data
   int  cycleTime;              // cycle time for meta data
   struct Node *nextNode;
};


struct configStruct
  {
     int  versionData;
     int  quantumTimeData;
     int  memoryAvailableData;
     int  processorCycleTimeData;
     int  ioCycleTimeData;
     char logToData[8];
     char logFilePathData[20];
     char filePathData[20];
     char cpuSchedulingCodeData[8];
  };


/*
 * function prototypes
 * these prototypes consist of helper methods, printing methods
 * and parsing methods for the data
 * method discriptions will be in the respected c file
 */

void skipFirstLineInFile(FILE *fPrt);
int  oprationStringTest(char *test);                            // helper
int  componentLetterTest(char test);                            // helper
void clean(char array[], int amount);                          // helper
void printList(struct Node *start);                                   // printing
void errorNoOpenParenthesis();                                 // printing

struct Node *parMetaDataFile(FILE *fPrt);                      // parsing
struct ConfigNode addConfigNode(char *dataName, char *data);   // helper
struct Node* addNode(struct Node *currentNode);         // helper

/*
 * functions for parsing config file
 */


int  testDataName(char *dataName);
void testCode(char *dataArray,struct configStruct *configData, int dataType);
void howToTreatData(int dataType, FILE *fPrt,struct configStruct *configData);
void whereToPutFilePathData(char *filePathData,int  dataType,struct configStruct  *configData);
void whereToPutTheData(int data,struct configStruct *configData,int dataType);
void getCommandNames(FILE *fPrt,struct configStruct *configData, int dataType);
void getNumberData(FILE *fPrt,struct configStruct *configData, int dataType);
void getFilePaths(FILE *fPrt,struct configStruct *configData, int dataType);
void getDataName(FILE *fPrt, char *dataName);
struct configStruct parConfigFile(FILE *fPrt);


#endif
