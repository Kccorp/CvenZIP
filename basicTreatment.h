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
int extractAllFiles(char *zipName,char *password);
int extractFile(char *zipName, char *extractFile,char *password);
//checkPassword(char *zipName, char *extractFile,char *password);
int Add_OverwriteFile(const char* fileZip, const char* pathFile, const char* output);
void printHelp();
int checkIfFileExist(char *path);
int isZipPasswordEncrypted(const char* filename, char *password,int flag);
int PasswordGestion(char *password, const char *zipFile);

#endif //CVENZIP_BASICTREATMENT_H
