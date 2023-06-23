#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include "basicTreatment.h"
#include "brutforceFile.h"

#define MAX_FILENAME_LENGTH 256
#define MAX_PASSWORD_LENGTH 256



int main(int argc, char *argv[]) {

    srand(time(NULL));  // Initialisation de la graine pour la génération aléatoire

    char *password = malloc(sizeof(char) * MAX_PASSWORD_LENGTH);
    char *filename = malloc(sizeof(char) * MAX_FILENAME_LENGTH);

    // Init no password
    long randomValue = generateRandomLong(1000000000, 2000000000);
    // Conversion en chaîne de caractères
    char cleCheckPassword[20];
    sprintf(cleCheckPassword, "%ld", randomValue);
    strcpy(password, cleCheckPassword);

    //check if the user has entered a filename in 1st argument and a password in 2nd argument
    if (checkIfFileExist(argv[1])) {
        strcpy(filename, argv[1]);
    } else {
        printf("Please enter a filename\n\n\n\n");
        printHelp();
        exit(1);
    }
    //for --args parsing (getopt) and their equibalent with -h
    struct option longopts[] = {
            {"help", no_argument, NULL, 'h'},
            {"extract", no_argument, NULL, 'e'},
            {"input", required_argument, NULL, 'i'},
            {"output", required_argument, NULL, 'o'},
            {"password", required_argument, NULL, 'p'},
            {"show", no_argument, NULL, 's'},
            {NULL, 0, NULL, 0}
    };

    int c;
    int index = 0;

    while ((c = getopt_long(argc, argv, "hei:o:p:s", longopts, &index)) != -1) {
        switch (c) {
            case 'h':
                printHelp();
                break;
                case 'p':
                strcpy(password, optarg);
                break;
                case 'e':
                extractAll(filename, password, cleCheckPassword);
                break;
            case 'i':
                Add_OverwriteFile(filename, optarg, optarg);
                break;
            case 'o':
                Add_OverwriteFile(filename, optarg, optarg);
                break;
            case 's':
                printZipFolder(filename);
                break;
            default:
                printf("Unknown option\n");
                break;
        }
    }

    free(password);
    free(filename);

    return 0;
}
