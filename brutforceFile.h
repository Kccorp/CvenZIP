//
// Created by kbod on 19/06/23.
//

#ifndef CVENZIP_BRUTFORCEFILE_H
#define CVENZIP_BRUTFORCEFILE_H

#endif //CVENZIP_BRUTFORCEFILE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

struct arg_struct_dic {
    char *pathToDic;
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
void *iterativeBrutforce(char *chars, int min, int max, void *pathToDic);

void askDicBrutforce (char *pathToDic, int numberOfThreads);
void askIterativeBrutforce (char *pathToZip, int numberOfThreads, int lengthPass);

void threadsDicController(char *pathToDic, int numberOfThreads);
int getLineNumber(char *pathToDic);

void workerBrutforce(char *current, int len, char *chars);


