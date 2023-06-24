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
int extractFile(char *zipName, char *extractFile,char *password,char *cleCheckPassword);
int checkPassword(char *zipName, char *extractFile,char *password);
int Add_OverwriteFile(const char* fileZip, const char* pathFileInput, const char* pathFileOutput);
void printHelp();
int checkIfFileExist(char *path);

#endif //CVENZIP_BASICTREATMENT_H
