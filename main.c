#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "basicTreatment.h"
#include "brutforceFile.h"

#define MAX_FILENAME_LENGTH 256
#define MAX_PASSWORD_LENGTH 256





int main(int argc, char *argv[]) {
    printf("==============START==============\n");
    srand(time(NULL));  // Initialisation de la graine pour la génération aléatoire

    char *password = malloc(sizeof(char) * MAX_PASSWORD_LENGTH);
    char *filename = malloc(sizeof(char) * MAX_FILENAME_LENGTH);

    // Init no password
    long randomValue = generateRandomLong(1000000000, 2000000000);
    // Conversion en chaîne de caractères
    char cleCheckPassword[20];
    sprintf(cleCheckPassword, "%ld", randomValue);
//    printf("cleCheckPassword : %s\n", cleCheckPassword);
    strcpy(password, cleCheckPassword);


    // Get Password
    for (int i = 1; i < argc; i++) {
        // filename
        if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) {
            // printf("-f DETECT: '%s'\n", filename);
            strcpy(filename, argv[i + 1]);
        }

        // password
        if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--password") == 0) {
            // printf("-p DETECT: '%s'\n", password);
            strcpy(password, argv[i + 1]);
        }
    }

    for (int i = 1; i < argc; i++) {
        // Help
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
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
                   "        - insérer un fichier (hôte > dossier compressé)");

        } else {
            //Extract all
            if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--extract") == 0) {
//                printf("-e DETECT\n");
                extractAll(filename, password, cleCheckPassword);
            }else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--include") == 0) {
//                printf("-i DETECT\n");
                Add_OverwriteFile(argv[i+1], argv[i+2], argv[i+3]);

            }else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--open") == 0) {
//                printf("-o DETECT\n");
                printZipFolder(argv[i+1]);

            }
        }
    }



    printf("==============END==============\n");

    return 0;
}
