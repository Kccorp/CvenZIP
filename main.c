//simple libzip example to create a zip file and add a file to it

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>
#include <errno.h>



struct zip *open_zip(char *filename) { //fonction qui va retourner notre structure zip pour l"utiliser dans le main et autre fonction
    struct zip *zip; //on crée une structure zip definit dans la librairie zip.h
    int err;

    zip = zip_open(filename, 0, &err); //ouvre le zip et stocke le resultat dans la structure zip
    if (zip == NULL) {
        printf("can't open zip archive '%s': %s\n", filename, zip_strerror(zip)); // a modifier afin que si une erreur précise , on puisse l'afficher
        exit(1);
    }

    return zip; //on retourne la structure zip afin de l'utiliser dans le main
}

void close_zip(struct zip *zip) {
    if (zip_close(zip) < 0) { //on ferme le fichier zip avec la fonction zip_close qui prend en parametre la structure zip et retourne -1 si il y a une erreur
        printf("can't close zip archive '%s': %s\n", "test.zip", zip_strerror(zip));
        exit(1);
    }
    printf("closing zip archive...\n");
}

int main() {
    struct zip *zip;
    int num_files;

    zip = open_zip("fs.zip");

    num_files = zip_get_num_files(zip); // pour tester si ça ouvrait bien , print le nombre de fichier
    printf("number of files in zip: %d\n", num_files);

    close_zip(zip);

    return 0;
}