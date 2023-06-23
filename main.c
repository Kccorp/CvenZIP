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

    //for --args parsing (getopt) and their equibalent with -h
    struct option longopts[] = {
            {"help", no_argument, NULL, 'h'},
            {"extract", no_argument, NULL, 'e'},
            {"input", required_argument, NULL, 'i'},
            {"output", required_argument, NULL, 'o'},
            {"file", required_argument, NULL, 'f'},
            {"password", required_argument, NULL, 'p'},
            {NULL, 0, NULL, 0}
    };

    int c;
    int index = 0;
    while ((c = getopt_long(argc, argv, "hef:i:o:p:", longopts, &index)) != -1) {
        switch (c) {
            case 'h':
                printHelp();
                break;
            case 'e':
                extractAll(filename, password, cleCheckPassword);
                break;
            case 'f':
                strcpy(filename, optarg);
                break;
            case 'i':
                Add_OverwriteFile(filename, optarg, optarg);
                break;
            case 'o':
                Add_OverwriteFile(filename, optarg, optarg);
                break;
            case 'p':
                strcpy(password, optarg);
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
