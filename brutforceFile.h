//
// Created by kbod on 19/06/23.
//

#ifndef CVENZIP_BRUTFORCEFILE_H
#define CVENZIP_BRUTFORCEFILE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

struct arg_struct_dic {
    char *pathToDic;
    const char *fileToCrack;
    int startLine;
    int endLine;
};

struct arg_struct_iter {
    char *pathToDic;
    int max;
    int min;
    char *chars;
};

void *dicBrutforce (void *parameters);
void *iterativeBrutforce(void *parameters);

void askDicBrutforce (const char *fileToCrack);
void askIterativeBrutforce (char *pathToZip);

void threadsDicController(char *pathToDic, int numberOfThreads, const char *fileToCrack);
int getLineNumber(char *pathToDic);

void threadsIterController (char *pathToDic, int numberOfThreads, int lengthMax, char *chars);
void workerBrutforce(char *current, int len, char *chars, char *fileToCrack);

char* removeTrailingNull(char* str);


#endif //CVENZIP_BRUTFORCEFILE_H



