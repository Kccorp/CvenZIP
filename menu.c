//
// Created by m3kaniml on 26/06/23.
//

#include "menu.h"
#include "basicTreatment.h"
#include <regex.h>

void clear(){
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

int menu(char *zipName,char *lastPath, char *newPath, int indice, int flag, char *password){
    char *inputFile = malloc(sizeof(char) * 100);
    char *outputFile = malloc(sizeof(char) * 100);
    int longueur = strlen(newPath);

    if (newPath[longueur - 1] != '/') {
        menuFile(zipName, newPath, password);
    } else {

        clear();
        switch (flag) {
            case 1:
                printf("Extraction réussi : %s\n\n", lastPath);
                break;
            case -1:
                printf("ECHEC de l'extraction  : %s\n\n", lastPath);
                break;
            case 2:
                printf("Remplacement réussi : %s\n\n", lastPath);
                break;
            case -2:
                printf("ECHEC du remplacement: %s\n\n", lastPath);
                break;
            case 3:
                printf("Insertion du fichier réussi\n\n");
                break;
            case -3:
                printf("ECHEC de l'insertion du fichier\n\n");
                break;
            default:
                break;
        }

            
        printf("Chemin actuel : %s\n\n", newPath);
        // Ouvre le fichier zip
        int error;
        const char delimiteur[] = "/";
        int line = 1;
        char list[255][100];
        char *regex_pattern = malloc(sizeof(char) * strlen(newPath));
        strcpy(regex_pattern, newPath);
        regex_t regex;

        // Compilation de l'expression régulière
        if (regcomp(&regex, regex_pattern, REG_EXTENDED) != 0) {
            fprintf(stderr, "Erreur lors de la compilation de l'expression régulière\n");
            exit(1);
        }

        char *pathFile = malloc(sizeof(char) * 100);
        char *token = malloc(sizeof(char) * 100);

        zip_t *zip = zip_open(zipName, 0, &error);
        if (zip == NULL) {
            printf("Impossible d'ouvrir le fichier zip\n");
            return 1;
        }

        // Obtient le nombre d'entrées dans le fichier zip
        int num_entries = zip_get_num_entries(zip, 0);
//    printf("Nombre d'entrées dans le fichier zip : %d\n", num_entries);


        for (int i = 0; i < num_entries; i++) {
            /* on utilise la position "i" pour récupérer le nom des fichiers */
            int localIndice = 0;
            strcpy(pathFile, zip_get_name(zip, i, ZIP_FL_UNCHANGED));

            token = strtok(pathFile, delimiteur);

            //compteur combien y a t il de fichier a lister dans le zip
            while (token != NULL) {
//            printf("%s", token);
                token = strtok(NULL, delimiteur);
                localIndice++;
            }

            // Stock chaque ligne dans un tableau "list"
            if (localIndice == indice) {
                if (regexec(&regex, zip_get_name(zip, i, ZIP_FL_UNCHANGED), 0, NULL, 0) == 0) {
//                printf("La chaîne commence par \"%s\"\n", newPath);
                    strcpy(list[line - 1], zip_get_name(zip, i, ZIP_FL_UNCHANGED));
                    line++;

                }
            }
        }


// AFFICHAGE DU MENU ===========================================================
        // Affiche le contenu du tableau
        printf("-1 - inclure un fichier \n");
        printf("0 - DOMICILE\n");
        for (int i = 0; i < line - 1; ++i) {
            printf("%d - %s\n", i + 1, list[i]);
        }
        // Ferme le fichier zip
        zip_close(zip);

        int choice;
        printf("\nFaites un choix : \n");
        scanf("%d", &choice);

        switch (choice) {
            case -1:
                //get current path (/ = fake default path)
                if (strcmp(newPath, "/") == 0) {
                    strcpy(outputFile, "");
                } else {
                    strcpy(outputFile, newPath);
                }
                printf("Entrez le chemin du fichier (host) a inclure : \n");
                scanf("%s", inputFile);

                //Recuperer le nom du fichier en cas de chemin/Vers/fichier.txt
                char *fileName = strrchr(inputFile, '/');  // Trouve le dernier '/' dans la chaîne
                if (fileName != NULL) {
                    fileName++;  // passer de /fileName a fileName
                    strcat(outputFile, fileName);
                }

                //include file

                if (Add_OverwriteFile(zipName, inputFile, outputFile)){
                    menu(zipName, lastPath, newPath, 1, 2, password);
                }else


                break;
            case 0:
                menu(zipName, "", "/", 1, 0, password);
                break;
            default:
                menu(zipName, list[choice - 1], list[choice - 1], indice + 1, 0, password);
                break;

        }

        return 0;

    }
}


int menuFile(char *zipName, char *selectFile, char *password) {
    clear();
    char *inputFile= malloc(sizeof(char) * 100);

    printf("0 - DOMICILE\n");
    printf("1 - Extraire le fichier\n");
    printf("2 - Remplacer le fichier\n");

    printf("\nFaites un choix : \n");
    int number;
    scanf("%d", &number);
    switch (number) {
        case 0:
            menu( zipName, selectFile, "/", 1, 0, password);
            break;
        case 1:
            if (extractFile(zipName, selectFile, password)){
                menu( zipName, selectFile, "/", 1, 1, password);
            }else{
                menu( zipName, selectFile, "/", 1, -1, password);
            }


            break;
        case 2:
            printf("Entrez le chemin du fichier nouveau ficher (local au pc) : \n");
            scanf("%s", inputFile);

            if (Add_OverwriteFile(zipName, inputFile, selectFile)){
                menu( zipName, selectFile, "/", 1, 3, password);
            }else{
                menu( zipName, selectFile, "/", 1, -3, password);
            }

            break;
        default:
            printf("Erreur de saisie\n");
            break;
    }




    return 0;
}