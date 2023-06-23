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
                printf("Examples :\n"
                       "        ./main -f monZip.zip -e -p password1234 => Extract zip file with the password 'password1234'\n"
                       "        ./main -i monZip.zip path/local/file.txt path/in/zip/file.txt  => Add or overwrite file\n"
                       "\nFeatures :\n"
                       "    - Utiliser avec des arguments \n"
                       "        --help, -h => affiche les aides\n"
                       "        --open, -o => ouvre le fichier\n"
                       "        --bruteforce, -b => bruteforce le fichier compressé\n"
                       "        --dictionary, -d => bruteforce avec un dictionnaire\n"
                       "        --password, -p => déverouille fichier en saisissant le mot de passe \n"
                       "        --extract [arg1] [arg2], -e [arg1] [arg2] => extrait un fichier \n"
                       "        --include [dossier_destination.zip] [file_add.txt] , -i [arg1] [arg2] => include un fichier \n"
                       "        \n"
                       "    - Ouvrir un fichier zip\n"
                       "        - Sans mot de passe\n"
                       "        - En saisissant un mot de passe\n"
                       "    \n"
                       "    - Bruteforce un fichier zip\n"
                       "        - Brutforce par dictionnaire \n"
                       "        - Brutforce par itération \n"
                       "        \n"
                       "    - Explorer le contenu du fichier unzip\n"
                       "        - afficher le contenu (comme un ls)\n"
                       "        - récupérer un fichier (dossier compressé > hôte)\n"
                       "        - insérer un fichier (hôte > dossier compressé)\n");
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
                break;
        }
    }

    free(password);
    free(filename);

    return 0;
}
