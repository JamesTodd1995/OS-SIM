// Main Program Header Information////
/**
  * @file mainprog.h
  *
  * @brief header for mainprog.c that defines MAINPRO_H, loads dependencies, prototypes functions
  */

#ifndef MAINPRO_H
#define MAINPRO_H
 
#include "parMethods.h"
#include "myTimer.h"
#include "simulator.h"

void doesFileExist(char* fileName);
void printConfigData(struct configStruct *configData);

#endif

