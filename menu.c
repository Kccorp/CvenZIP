//
// Created by m3kaniml on 26/06/23.
//

#include "menu.h"
#include "basicTreatment.h"
#include <regex.h>


int menu(char *zipName){

    // Ouvre le fichier zip
    int error;
    const char delimiteur[] = "/";
    int indice = 0;

    char *pathFile= malloc(sizeof(char)*100);
    char *token= malloc(sizeof(char)*100);

    zip_t *zip = zip_open(zipName, 0, &error);
    if (zip == NULL) {
        printf("Impossible d'ouvrir le fichier zip\n");
        return 1;
    }

    // Obtient le nombre d'entrées dans le fichier zip
    int num_entries = zip_get_num_entries(zip, 0);
    printf("Nombre d'entrées dans le fichier zip : %d\n", num_entries);


    for(int i=0; i<num_entries; i++)
    {
        /* on utilise la position "i" pour récupérer le nom des fichiers */
        int indice = 0;
        strcpy(pathFile,zip_get_name(zip, i, ZIP_FL_UNCHANGED));
        token = strtok(pathFile, delimiteur);

        while (token != NULL) {
            printf("[%d]: %s\n", indice, token);
            token = strtok(NULL, delimiteur);
            indice++;
        }
    }



    // Ferme le fichier zip
    zip_close(zip);

    return 0;

}
