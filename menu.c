//
// Created by m3kaniml on 26/06/23.
//

#include "menu.h"
#include "basicTreatment.h"
#include <regex.h>


int menu(char *zipName){


    int error;
    zip_t *zip = zip_open(zipName, 0, &error);
    if (zip == NULL) {
        printf("Impossible d'ouvrir le fichier zip\n");
        return 1;
    }

    // Obtient le nombre d'entrées dans le fichier zip
    int num_entries = zip_get_num_entries(zip, 0);
    printf("Nombre d'entrées dans le fichier zip : %d\n", num_entries);

//check if the name of the files contain */* and if no print it
    for (int i = 0; i < num_entries; i++) {
        const char *name = zip_get_name(zip, i, 0);
        regex_t regex;
        int reti;
        char msgbuf[100];


        reti = regcomp(&regex, ".*\\*\\/.*", 0);
        if (reti) {
            fprintf(stderr, "Could not compile regex\n");
            exit(1);
        }

        /* Execute regular expression */
        reti = regexec(&regex, name, 0, NULL, 0);
        if (!reti) {
            printf("Le nom du fichier %s contient */*\n", name);
        }
        else if (reti == REG_NOMATCH) {
            printf("Le nom du fichier %s ne contient pas */*\n", name);
        }
        else {
            regerror(reti, &regex, msgbuf, sizeof(msgbuf));
            fprintf(stderr, "Regex match failed: %s\n", msgbuf);
            exit(1);
        }

        /* Free memory allocated to the pattern buffer by regcomp() */
        regfree(&regex);
    }

    // Ferme le fichier zip
    zip_close(zip);

    return 0;
}