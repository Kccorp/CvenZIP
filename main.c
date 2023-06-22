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

    zip = open_zip("test.zip");

    num_files = zip_get_num_files(zip); // pour tester si ça ouvrait bien , print le nombre de fichier
    printf("number of files in zip: %d\n", num_files);

    close_zip(zip);

    return 0;
}




void extract_file(struct zip *zip, char *filename) { //fonction qui va extraire un fichier du zip et le stocker dans le dossier courant
    struct zip_file *file;
    char buffer[100]; //buffer qui va contenir le contenu du fichier , a modifier pour que ça soit dynamique
    int len;
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

    zip = open_zip("test.zip");

    num_files = zip_get_num_files(zip); // pour tester si ça ouvrait bien , print le nombre de fichier
    printf("number of files in zip: %d\n", num_files);

        // Ouvre le fichier dans le fichier zip
        zip_file_t *file = zip_fopen_index(zip, i, 0);
        if (file == NULL) {
            printf("Impossible d'ouvrir le fichier dans le fichier zip : %s\n", zip_strerror(error));
            zip_close(zip);
            return 1;
        }

        // Crée un fichier local pour écrire les données extraites
        FILE *out_file = fopen(name, "wb");
        if (out_file == NULL) {
            printf("Impossible de créer le fichier local : %s\n", name);
            zip_fclose(file);
            zip_close(zip);
            return 1;
        }
        char *filename = NULL;
        // Lit et écrit les données extraites
        zip_int64_t num_bytes;
        char buf[1024];
        while ((num_bytes = zip_fread(file, buf, sizeof(buf))) > 0) {
            fwrite(buf, sizeof(char), num_bytes, out_file);
        }

        // Ferme le fichier local et le fichier dans le fichier zip
        fclose(out_file);
        zip_fclose(file);
    }

    // Ferme le fichier zip
    zip_close(zip);

    printf("Extraction terminée avec succès.\n");
