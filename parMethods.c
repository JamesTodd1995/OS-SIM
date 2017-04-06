
#include"parMethods.h"

/*
 * the parConfigfile method parses though the config file
 * and saves the needed infomation in the ConfigNodeArray
 *
 * the two arguments are a file pointer and the configNodeArray
 */
struct configStruct parConfigFile(FILE *fPrt)
  {
     int  dataType = 0;
     char dataName[32];
     struct configStruct configData;

     //skip the first line and have the file pointer point
     //to the beginning of the next line.
     skipFirstLineInFile(fPrt);
    
     
     while(!feof(fPrt))
       {
          getDataName(fPrt, dataName);
          dataType = testDataName(dataName);
          if(dataType == 10)
            {
               break;
            }
          howToTreatData(dataType, fPrt, &configData);
       }
     return configData;
  }


/*
 * parMetaDataFile parses though the meta data file taking in a file pointer
 * as an argument
 */

struct Node *parMetaDataFile(FILE *fPrt)
  {
     // these are used in parsing the meta data, cycleTime is used as a plase holder
     int counter = 0, test = 0, cycleTime = 0; 
     int oprationStringArrayLength = 12;
     char componentLetter, in;
     char oprationString[12];
     clean(oprationString,oprationStringArrayLength);
   

     // varables used for link list
     struct Node *first = malloc (sizeof(struct Node));
     struct Node *current =  first;
 
   //  this gets past the first line in the meta data file
   while(in != ':')
     {
        fscanf(fPrt, "%c",&in);
     }

   while(!feof(fPrt))
     {
        // skip whiteSpace until a componentLetter is found
             while(in == ' ' || in == ')' || in == ';' || in == '\n'  || in == ':')
       {
          fscanf(fPrt, "%c",&in);
       }
        // test to see if compnent letter is valid
        // else there is an error, so stop
        test = componentLetterTest(in);

        if(test == 0)
          {
             componentLetter = in;
          }
        // 1 means we hit the end of the meta data file
        else if(test == 1)
          {
             return first;
          }
        else
          {
             exit(1);
          }

        // move the pointer by one to get to a (
        fscanf(fPrt, "%c",&in);

        if(in == '(')
          {
             while((in = fgetc(fPrt)) != ')')
               {
                  oprationString[counter++] = in;
               }
             if((oprationStringTest(oprationString)) != 0)
               {
                  exit(1);
               }
         
          }
        else // is ( is not there, then we have an error, stop
          {
              errorNoOpenParenthesis();
          }

        if(in == ')')// when we are at a ), get the cycle time
          {
             fscanf(fPrt, "%d",&cycleTime);
          }
      
        // with the plase holders for cycle time, component letter,
        // and opration string save put the infomation in to a new node!

        current->componentLetter = componentLetter;
        strcpy(current->oprationString,oprationString);
        current->cycleTime = cycleTime;
        current = addNode(current);
        clean(oprationString, oprationStringArrayLength);
        // clean the opration string array for more use
        counter = 0;
     }
     return first;// when finish, return the first node
  }


// a helper method for testing to see if the component letter is good, else stop
int componentLetterTest(char test)
  {
     switch(test)
     {
        case 'S' :
           return 0;
        case 'A' :
           return 0;
        case 'P' :
           return 0;
        case 'M' :
           return 0;
        case 'I' :
           return 0;
        case 'O' :
           return 0;
        case 'E' :
           return 1;
        case '.' :
           return 1;
        default :
           printf("\nError Meta Data Error\n");
           printf("\nComponent Letter Error: | %c | is not a valid Component Letten", test);
           printf("\nProgram ending\n\n\n");
           exit(1);
     }
  }

void errorNoOpenParenthesis()
  {
     printf("\n\nError Meta Data Error\n");
     printf("When looking for operation string ( was not found\n");
     printf("\nProgram ending\n");
     exit(1);  
  }


// a helper method to see if the opration string is good, else stop
int oprationStringTest(char *test)
  {
     if((strcmp(test,"access") == 0))
       {
          return 0;
       }
     if((strcmp(test,"allocate") == 0))
       {
          return 0;
       }
     if((strcmp(test,"end") == 0))
       {
          return 0;
       }
     if((strcmp(test,"hard drive") == 0))
       {
          return 0;
       }
     if((strcmp(test,"keyboard") == 0))
       {
          return 0;
       }
     if((strcmp(test,"printer") == 0))  
       {
          return 0;
       }
     if((strcmp(test,"monitor") == 0))
       {
          return 0;
       }
     if((strcmp(test,"run") == 0))
       {
          return 0;
       }
     if((strcmp(test,"start") == 0))
       {
          return 0;
       }
     printf("\nError Meta Data Error\n");
     printf("\n%s is not a valid opStr\n",test);
     printf("\nEnding program\n");
     exit(1);
  }

// a helper method for adding a node to the link list
struct Node *addNode(struct Node *currentNode)
  {
     struct Node *newNode = (struct Node *) malloc (sizeof(struct Node));
     newNode->nextNode = NULL;
     currentNode->nextNode = newNode;
     return newNode;
  }

     
// a helper method for clean char arrays, this is because the array
// needs to be empty each time it is used so that there is no extra infomation if the array
void clean(char array[], int amount)
  {  
     int loopCount = 0;
     for(loopCount = 0; loopCount < amount; loopCount++)
       {
          array[loopCount] = '\0';
       }  
  }


// a method to print the link list
void printList(struct Node *start)
  {
     struct Node *nextNode, *current;
     current = start;
     while(current->nextNode != NULL)
       {  
          nextNode = current->nextNode;
          printf("\nThe data item component letter is: %c",current->componentLetter);
          printf("\nThe data item operation string is: %s",current->oprationString);
          printf("\nThe data item cycle time is      : %d",current->cycleTime);
          printf("\n");
          current = nextNode;
       } 
  }

/* the functions below are for parsing config */
// this method gets the name of the config data
void getDataName(FILE *fPrt, char *dataName)
  {
     int  sizeOfDataNameArray =32;
     int  arrayPot = 0, counter = 1;
     char in;
     clean(dataName,sizeOfDataNameArray);
     in = fgetc(fPrt);
     while(in != ':' && counter < 32)
       {
          dataName[arrayPot++] = in;
          in = fgetc(fPrt);
          counter++;
       }
  }

// once the name is known, test to see if it is valid
// also, the end condition is here as well to stop parsing
int testDataName(char *dataName)
  {
     if((strcmp(dataName,"Version/Phase") == 0))
       {
          return 1;
       }
     if((strcmp(dataName,"File Path") == 0))
       {
          return 2;
       }
     if((strcmp(dataName,"CPU Scheduling Code") == 0))
       {
          return 3;
       }
     if((strcmp(dataName,"Quantum Time (cycles)") == 0))
       {
          return 4;
       }
     if((strcmp(dataName,"Memory Available (KB)") == 0))
       {
          return 5;
       }
     if((strcmp(dataName,"Processor Cycle Time (msec)") == 0))
       {
          return 6;
       }
     if((strcmp(dataName,"I/O Cycle Time (msec)") == 0))
       {
          return 7;
       }
     if((strcmp(dataName,"Log To") == 0))
       {
          return 8;
       }
     if((strcmp(dataName,"Log File Path") == 0))
       {
          return 9;
       }
     if((strcmp(dataName,"End Simulator Configuration Fil") == 0))
       {
          return 10;
       }
     printf("\nError Config Data Error\n");
     printf("\n (%s) is an invalid data name",dataName);
     printf("\nEnding Program\n");
     exit(1);
  }

// onec we have gotten a valid data name, this will say how to treat
// the data the coraspond with the name.
// for example 1 means Version/Phase, so a call to getNumberdata
// to get the numberfor the version of the program.
void howToTreatData(int dataType, FILE *fPrt, struct configStruct *configData)
  {
     if(dataType == 1)
       {
          getNumberData(fPrt, configData, dataType);
       }
     if(dataType == 2)
       {
          getFilePaths(fPrt, configData, dataType);
       }
     if(dataType == 3)
       {
          getCommandNames(fPrt, configData, dataType);
       }
     if(dataType == 4)
       {
          getNumberData(fPrt,configData,dataType);
       }
     if(dataType == 5)
       {
          getNumberData(fPrt,configData,dataType);
       }
     if(dataType == 6)
       {
          getNumberData(fPrt,configData,dataType);
       }
     if(dataType == 7)
       {
          getNumberData(fPrt,configData,dataType);
       }
     if(dataType == 8)
       {
          getCommandNames(fPrt, configData, dataType);
       }
     if(dataType == 9)
       {
          getFilePaths(fPrt, configData, dataType);
       }
     if(dataType == 0)
       {
          return;
       }
  }

// this method gets the data for the data names that have string as data
// which is Log to, CUP Scheduling Code.
// then once the data is known, it is then tested for validness.
void getCommandNames(FILE *fPrt, struct configStruct *configData, int dataType)
  {                            
     char dataArray[7], in;
     int arrayPot = 0, sizeOfDataArray = 7;
     clean(dataArray, sizeOfDataArray);
     if(dataType == 3)
       {
          clean(configData->cpuSchedulingCodeData,sizeOfDataArray);
       }
     if(dataType == 8)
       {
          clean(configData->logFilePathData,sizeOfDataArray);
       }
     in = fgetc(fPrt);
     in = fgetc(fPrt);
     while(in != '\n')
       {
          dataArray[arrayPot++] = in;
          in = fgetc(fPrt);
       }  
     testCode(dataArray, configData, dataType);
  }

// this method gets the data for the file paths in the config file
// then the file paths are tested.
void getFilePaths(FILE *fPrt, struct configStruct *configData, int dataType)
  {
     char in;
     char filePathData[20];
    // char testData[5];
     int  sizeOfFilePathData = 20,  arrayPot = 0;
     clean(filePathData, sizeOfFilePathData);
     in = fgetc(fPrt);
     in = fgetc(fPrt);
     while(in != '\n')
       {
          filePathData[arrayPot++] = in;
          in = fgetc(fPrt);
       }     
     whereToPutFilePathData(filePathData, dataType, configData);
  }

// this method get intager data from the config file,
// then test it.
void getNumberData(FILE *fPrt, struct configStruct *configData, int dataType)
  {
     char dataArray[5];
     char in;
     int sizeOfDataArray = 5;
     int arrayPot = 0;
     int data = 0;
     clean(dataArray,sizeOfDataArray);
     in = fgetc(fPrt);
     in = fgetc(fPrt);
     while(in != '\n')
       {
          dataArray[arrayPot++] = in;
          in = fgetc(fPrt);
       }
     data = atoi(dataArray);
     whereToPutTheData(data, configData,dataType);
  }

// this method puts interger data when it needs to be in the
// congif struct also testing to make sure it is valid
void whereToPutTheData(int data,struct configStruct *configData,int dataType)
  {
     if(dataType == 1)
       {
          if(data >= 0 && data <= 10)
            {
               configData->versionData = data;
               return;
            }
          printf("\nError here Config data Error\n");
          printf("\n\nVersion number is out of range, valid range is 0 <= Ver <= 10\n");
          printf("\nThe input was %d",data);
          printf("\n\nEnding program\n");
          exit(1);
       }
     if(dataType == 4)
       {
          if( data >= 0 && data <=100)
            {
               configData->quantumTimeData = data; 
               return;
            }
          printf("\nError Congif Data Error\n");
          printf("\ninvalid Quantum Time (%d), time needs to between 1 and 100\n",data);
          printf("\nEnding Program\n");
          exit(1);
       }
     if(dataType == 5)
       {
          if( data >= 1 && data <=1024)
            {
               configData->memoryAvailableData = data; 
               return;
            }
          printf("\nError Congif Data Error\n");
          printf("\ninvalid Memory avaulable (%d), memory needs to between 1 and 1024\n",data);
          printf("\nEnding Program\n");
          exit(1);
       }
     if(dataType == 6)
       {
          if( data >= 1 && data <=1000)
            {
               configData->processorCycleTimeData = data; 
               return;
            }
          printf("\nError Congif Data Error\n");
          printf("\ninvalid Processor Cycle Time (%d), Time needs to between 1 and 1000\n",data);
          printf("\nEnding Program\n");
          exit(1);     
       }
     if(dataType == 7)
       {
          if( data >= 1 && data <=10000)
            {
               configData->ioCycleTimeData = data; 
               return;
            }
          printf("\nError Congif Data Error\n");
          printf("\ninvalid Processor I/O Cycle Time (%d)",data);
          printf(", I/O Cycle Time needs to between 1 and 10,000\n");
          printf("\nEnding Program\n");
          exit(1);          
       }
  }

// this method puts file path data where it needs to be in the config struct
// and test it
void whereToPutFilePathData(char *filePathData,int  dataType,struct configStruct  *configData)
  {
     char testData[5];
     int lenfthOfData = strlen(filePathData);
     // get the last part of the file path to test to see if it is valid.
     testData[0] = filePathData[lenfthOfData-4];
     testData[1] = filePathData[lenfthOfData-3];
     testData[2] = filePathData[lenfthOfData-2];
     testData[3] = filePathData[lenfthOfData-1];
     testData[4] = '\0';
     if(dataType == 2)
       {
          if((strcmp(testData,".mdf")) != 0)
            {
               printf("\nError Congif Data Error\n");
               printf("\ninvalid File Path (%s) does not end with .mdf",filePathData);
               printf("\n\nending Program\n");
               exit(1);
            }
          clean(configData->filePathData,20);
          strcpy(configData->filePathData,filePathData); 
       }
     if(dataType == 9)
       {
          if((strcmp(testData,".lgf")) != 0)
            {
               printf("\nError Congif Data Error\n");
               printf("\ninvalid Log File Path");
               printf(" (%s) does not end with .lgf",filePathData);

               printf("\n\nending Program\n");
               exit(1);
            }
          clean(configData->logFilePathData,20);
          strcpy(configData->logFilePathData,filePathData); 
       }
  }

// this is a helper method for testing if data is valid
void testCode(char *dataArray,struct configStruct *configData, int dataType)
  { 
     
     if((strcmp(dataArray, "NONE")) == 0)
       {
          strcpy(configData->cpuSchedulingCodeData,"FCFS-N");
          return;
       }
     if((strcmp(dataArray, "FCFS-N")) == 0)
       {
          strcpy(configData->cpuSchedulingCodeData,dataArray);
          return;
       }
     if((strcmp(dataArray, "SJF-N")) == 0)
       {
          strcpy(configData->cpuSchedulingCodeData,dataArray);
          return;
       }
     if((strcmp(dataArray, "SRTF-P")) == 0)
       {
          strcpy(configData->cpuSchedulingCodeData,dataArray);
          return;
       }
     if((strcmp(dataArray, "FCFS-P")) == 0)
       {
          strcpy(configData->cpuSchedulingCodeData,dataArray);
          return;
       }
     if((strcmp(dataArray, "RR-P")) == 0)
       {
          strcpy(configData->cpuSchedulingCodeData,dataArray);
          return;
       }
     if((strcmp(dataArray, "Monitor") == 0))
       {
          strcpy(configData->logToData,dataArray);
          return;
       }
     if((strcmp(dataArray, "File")) == 0)
       {
          strcpy(configData->logToData,dataArray);
          return;
       }
     if((strcmp(dataArray, "Both")) == 0)
       {
          strcpy(configData->logToData,dataArray);
          return;
       }
     if(dataType == 3)    
       { 
          printf("\nError Congif Data Error\n");
          printf("\ninvalid CPU Scgeduling Code (%s)\n",dataArray);
          printf("\nEnding Program\n");
          exit(1);  
       }
     if(dataType == 8)
       {
          printf("\nError Congif Data Error\n");
          printf("\ninvalid Log To Code (%s)\n",dataArray);
          printf("\nEnding Program\n");
          exit(1);
       }
  }

// this skips the first line in the files,
void skipFirstLineInFile(FILE *fPrt)
  {
     char in;
     while(in != '\n')
       {
          in = fgetc(fPrt);
       }
  }
