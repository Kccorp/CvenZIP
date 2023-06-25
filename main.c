#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "basicTreatment.h"
#include "brutforceFile.h"

#define MAX_ZIPNAME_LENGTH 256
#define MAX_PASSWORD_LENGTH 256



int main(int argc, char *argv[]) {



    char *password = malloc(sizeof(char) * MAX_PASSWORD_LENGTH);

    char *zipName = malloc(sizeof(char) * MAX_ZIPNAME_LENGTH);



    //check if the user has entered a zipName in 1st argument and a password in 2nd argument
    if (checkIfFileExist(argv[1])) {
        strcpy(zipName, argv[1]);
    } else {
        printf("Please enter a zipName\n\n\n\n");
        printHelp();
        exit(1);
    }


    //for --args parsing (getopt) and their equibalent with -h
    struct option longopts[] = {
            {"help", no_argument, NULL, 'h'},
            {"test", no_argument, NULL, 't'},
            {"extract", no_argument, NULL, 'e'},
            {"password", required_argument, NULL, 'p'}, //{"password", required_argument, NULL, 'p
            {"include", required_argument, NULL, 'i'},
            {"output", required_argument, NULL, 'o'},
            {"show", no_argument, NULL, 's'},
            {NULL, 0, NULL, 0}
    };


    char *outputName = malloc(sizeof(char) * MAX_ZIPNAME_LENGTH);

    //Merge ces 2 trucs ?
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "-output") == 0) {
            if (i + 1 < argc) {
                strcpy(outputName, argv[i + 1]);
                break;
            }
        }
    }for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--password") == 0) {
            if (i + 1 < argc) {
                strcpy(password, argv[i + 1]);
                break;
            }
        }
    }


    int c;
    int index = 0;
    while ((c = getopt_long(argc, argv, "he:i:st", longopts, &index)) != -1) {
        switch (c) {
            case 't':
                isZipPasswordEncrypted(zipName, password,0);
                break;
            case 'h':
                printHelp();
                break;
                case 'e':
                extractFile(zipName, optarg, password);
                break;
            case 'i':
                Add_OverwriteFile(zipName, optarg, outputName);
                break;
            case 's':
                printZipFolder(zipName);
                break;
            default:
                printf("Unknown option\n");
                break;
        }
    }

    free(password);
    free(zipName);

    return 0;
}
