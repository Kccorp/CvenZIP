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

struct arg_struct {
    char *pathToDic;
    int startLine;
    int endLine;
};

void *dicBrutforce (void *pathToDic);
void askDicBrutforce (char *pathToDic, int numberOfThreads);
void threadsController(char *pathToDic, int numberOfThreads);
int getLineNumber(char *pathToDic);

