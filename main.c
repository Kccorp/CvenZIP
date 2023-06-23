#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include "basicTreatment.h"
#include "brutforceFile.h"

#define MAX_FILENAME_LENGTH 256
#define MAX_PASSWORD_LENGTH 256

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"



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
"DESCRIPTION\n"
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