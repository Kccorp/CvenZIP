//
// Created by m3kaniml on 23/06/23.
//

#ifndef CVENZIP_BASICTREATMENT_H
#define CVENZIP_BASICTREATMENT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <zip.h>
#include <errno.h>

long generateRandomLong(long min, long max);
int printZipFolder(char *filename);
void extractFile(struct zip *zip, char *filename);
int extractAll(char *filename, char *password, char *cleCheckPassword);
int Add_OverwriteFile(const char* fileZip, const char* pathFileInput, const char* pathFileOutput);
void printHelp();

#endif //CVENZIP_BASICTREATMENT_H
