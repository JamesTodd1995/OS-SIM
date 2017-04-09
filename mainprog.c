// Main Program Imformation ////
/**
  * @file mainprog.c
  *
  * @brief Main driver that runs operations defined in helper classes and highest level logic/control
  */

#include"mainprog.h"


int main(int argc, char *argv[])
  {
     FILE *fPrt; 
     int tester = 0;

     if(argc < 2) 
       {
          printf("\nyou did not give enough Command line arguments");
          return 0;
       }
     if(argc > 2)
       {
          printf("\nyou gave to many command line arguments");
          return 0;
       }

     // ADTs for holding data
     struct configStruct configData;
     struct Node *metaDataLinkListNodeFirst  = (struct Node *)malloc (sizeof(struct Node));


     // start of parsing though the config file
     doesFileExist(argv[1]);
     fPrt = fopen(argv[1], "r");

     configData = parConfigFile(fPrt);
     fclose(fPrt);
     // end of parsing though the config file


     // start of parsing though the meta data file
     doesFileExist(configData.filePathData);
     fPrt = fopen(configData.filePathData, "r");

     metaDataLinkListNodeFirst = parMetaDataFile(fPrt);   
     fclose(fPrt);
     // end of parsing though the meta data file


     tester = startSim(metaDataLinkListNodeFirst, configData);
     if(tester == -3)
        {
           printf("\n\nmeta data does not have S(start)0");
           printf("\nfatal Error, ending program.");
        }
     if(tester == -1)
       {
           printf("\n\nmeta data does not have equal number of");
           printf("\nA(start)0 A(end)0.");   
           printf("\nfatal Error, ending program.");    
       }
     return 0;
  }

void doesFileExist(char* fileName)
  {
     FILE *fPrt;
     if((fPrt = fopen(fileName, "r")))
       {  
          fclose(fPrt);
       }
     else
       {
          printf("The file ( %s ) does not exist.\n", fileName);
          printf("Ending program.\n");
          exit(1);
       }
  }

void printConfigData(struct configStruct *configData)
  {
   printf("\nVersion/Phase: %d",configData->versionData);
   printf("\nFile Path: %s",configData->filePathData);
   printf("\nCPU Scheduling Code: %s",configData->cpuSchedulingCodeData);
   printf("\nQuantum Time: %d",configData->quantumTimeData);
   printf("\nMemory Available (KB): %d",configData->memoryAvailableData);
   printf("\nProcessor Cycle Time (msec): %d",configData->memoryAvailableData);
   printf("\nI/O Cycle Time (msec): %d",configData->ioCycleTimeData);
   printf("\nLog To: %s",configData->logToData);
   printf("\nLog File Path: %s\n",configData->logFilePathData);
  }
   
