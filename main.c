#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
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

    int option;
    while ((option = getopt(argc, argv, "f:p:")) != -1) {
        switch (option) {
            case 'f':
                strcpy(filename, optarg);
                break;
            case 'p':
                strcpy(password, optarg);
                break;
            default:
                break;
        }
    }


    optind = 1;  // Reset getopt() for the second loop

    while ((option = getopt(argc, argv, "hei:o:")) != -1) {
        switch (option) {
            case 'h':
                printHelp();
                break;
            case 'e':
                extractAll(filename, password, cleCheckPassword);
                break;
            case 'i':
                Add_OverwriteFile(argv[optind], argv[optind + 1], argv[optind + 2]);
                break;
            case 'o':
                printZipFolder(argv[optind]);
                break;
            default:
                printHelp();
                break;
        }
    }

    free(password);
    free(filename);

    return 0;
}
