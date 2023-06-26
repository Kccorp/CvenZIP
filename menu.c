//
// Created by m3kaniml on 26/06/23.
//

#include "menu.h"
#include "basicTreatment.h"
#include <regex.h>


int menu(char *zipName,char *lastPath, char *newPath, int indice, int flag){

    int longueur = strlen(newPath);

    if (newPath[longueur - 1] != '/') {
        menuFile(zipName, newPath);
    } else {
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");


        switch (flag) {
            case 1:
                printf("Extraction reussi : %s\n\n", lastPath);
                break;
            case 2:
                printf("ECHEC de l'extraction  : %s\n\n", lastPath);
                break;
            case 3:
                printf("Remplacement reussi : %s\n\n", lastPath);
                break;
            case 4:
                printf("ECHEC du remplacement: %s\n\n", lastPath);
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

        // Affiche le contenu du tableau
        printf("0 - HOME\n");
        for (int i = 0; i < line - 1; ++i) {
            printf("%d - %s\n", i + 1, list[i]);
        }
        // Ferme le fichier zip
        zip_close(zip);

        int number;
        printf("\nFaites un choix : \n");
        scanf("%d", &number);
        if (number == 0) {
            menu(zipName, "", "/", 1, 0);
        }
        menu(zipName, newPath, list[number - 1], indice + 1, 0);
        return 0;

    }
}


int menuFile(char *zipName , char *selectFile) {
//    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    char *inputFile= malloc(sizeof(char) * 100);

    printf("0 - HOME\n");
    printf("1 - Extraire le fichier\n");
    printf("2 - Remplacer le fichier\n");

    printf("\nFaites un choix : \n");
    int number;
    scanf("%d", &number);
    switch (number) {
        case 0:
            menu( zipName, selectFile, "/", 1, 0);
            break;
        case 1:
            if (extractFile(zipName, selectFile, "")){
                menu( zipName, selectFile, "/", 1, 1);
            }else{
                menu( zipName, selectFile, "/", 1, 2);
            }


            break;
        case 2:
            printf("Entrez le chemin du fichier nouveau ficher (local au pc) : \n");
            scanf("%s", inputFile);

            Add_OverwriteFile(zipName, inputFile, selectFile);
            break;
        default:
            printf("Erreur de saisie\n");
            break;
    }




    return 0;
}