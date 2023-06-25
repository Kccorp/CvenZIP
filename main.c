#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include "basicTreatment.h"
#include "brutforceFile.h"

#define MAX_ZIPNAME_LENGTH 256
#define MAX_PASSWORD_LENGTH 256



int main(int argc, char *argv[]) {



    char *password = malloc(sizeof(char) * MAX_PASSWORD_LENGTH);
    password="";
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
            {"include", required_argument, NULL, 'i'},
            {"output", required_argument, NULL, 'o'},
//            {"password", required_argument, NULL, 'p'},
            {"show", no_argument, NULL, 's'},
            {NULL, 0, NULL, 0}
    };

    //check if the user enter -o or -output and store the arg in outputName
    char *outputName = malloc(sizeof(char) * MAX_ZIPNAME_LENGTH);
    outputName="kiki";
    //if -o or -output is used, store the arg in outputName
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "-output") == 0) {
            if (i + 1 < argc) {
                outputName = argv[i + 1];
                break;
            } else {
                printf("Missing argument for -o or -output option.\n");
                return 1;
            }
        }
    }
    int c;
    int index = 0;
    while ((c = getopt_long(argc, argv, "he:i:o:p:st", longopts, &index)) != -1) {
        switch (c) {
            case 't':
                isZipPasswordEncrypted(zipName, password);
                break;
            case 'h':
                printHelp();
                break;
                case 'p':
                strcpy(password, optarg);
                break;
                case 'e':
                extractFile(zipName, optarg, password);
                break;
            case 'i':
                Add_OverwriteFile(zipName, optarg, outputName);
                break;
//            case 'o':
//                Add_OverwriteFile(zipName, optarg, optarg);
//                break;
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
