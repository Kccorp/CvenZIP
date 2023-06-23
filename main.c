#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <zip.h>
#include <time.h>
#include "basicTreatment.h"

#define MAX_FILENAME_LENGTH 256
#define MAX_PASSWORD_LENGTH 256

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"



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
            printf("DESCRIPTION\n"
                   "    Manipulation de fichier compressé\n"
                   "\n"
                   ANSI_COLOR_RED"    -h"ANSI_COLOR_RESET","ANSI_COLOR_RED" --help\n"ANSI_COLOR_RESET
                   "        Afficher l'aide\n"
                   "        \n"
                   ANSI_COLOR_RED"    -f"ANSI_COLOR_RESET","ANSI_COLOR_RED" --file " ANSI_COLOR_YELLOW "<filename.zip>\n"ANSI_COLOR_RESET
                   "        Selectionner le fichier compressé\n"
                   "    \n"
                   ANSI_COLOR_RED"    -p"ANSI_COLOR_RESET","ANSI_COLOR_RED" --password " ANSI_COLOR_YELLOW "<monMotDePasse>\n"ANSI_COLOR_RESET
                   "        Mot de passe du fichier compressé\n"
                   "        \n"
                   ANSI_COLOR_RED"    -o"ANSI_COLOR_RESET","ANSI_COLOR_RED" --open " ANSI_COLOR_YELLOW "<filename>\n"ANSI_COLOR_RESET
                   "        Afficher le contenu d'un fichier compressé\n"
                   "        Ex: -o -f \"filename.zip\"\n"
                   "            -o -f \"filename.zip\"\n"
                   "        \n"
                   ANSI_COLOR_RED"    -i"ANSI_COLOR_RESET","ANSI_COLOR_RED" --include " ANSI_COLOR_YELLOW "<arg1> <arg2> <arg3>\n"ANSI_COLOR_RESET
                   "        Ajouter un fichier dans un fichier compressé\n"
                   "            * arg1 : chemin/fichier compressé de destination\n"
                   "            * arg2 : chemin/fichier que l'on souhaite ajouter dans le fichier compressé\n"
                   "            * arg3 : chemin/fichier où l'on souhaite enregistrer dans le fichier compressé\n"
                   "            \n"
                   ANSI_COLOR_RED"    -e"ANSI_COLOR_RESET","ANSI_COLOR_RED" --extract " ANSI_COLOR_YELLOW "<arg1> <arg2> <arg...>\n"ANSI_COLOR_RESET
                   "        Extrait les fichiers passé en argument \n"
                   "        Ex: -f \"couleur/rouge.txt\" \"couleur/bleu.txt\" \"couleur/vert.txt\"\n"
                   "            -f \"filename.zip\" -p \"1234\" -e \"couleur/rouge.txt\" \"couleur/bleu.txt\" \"couleur/vert.txt\""
                   "\n");

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
