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


void extract_file(struct zip *zip, char *filename) { //fonction qui va extraire un fichier du zip et le stocker dans le dossier courant
    struct zip_file *file;
    char buffer[100]; //buffer qui va contenir le contenu du fichier , a modifier pour que ça soit dynamique
    int len;
    int err;

    printf("extracting file '%s'...\n", filename);
    file = zip_fopen(zip, filename, 0); //on ouvre le fichier zip et on le stocke dans la structure file
    if (file == NULL) {
        printf("can't open file '%s': %s\n", filename, zip_strerror(zip));
        exit(1);
    }

    FILE *outfile = fopen(filename, "w"); //on crée un fichier de nom filename
    if (outfile == NULL) {
        printf("can't open file '%s': %s\n", filename, strerror(errno));
        exit(1);
    }

    while ((len = zip_fread(file, buffer, sizeof(buffer))) > 0) { //tant qu'on peut lire dans le fichier zip
        fwrite(buffer, 1, len, outfile); //on stocke da,s le fichier filename
    }

    if (len < 0) {
        printf("can't read file '%s': %s\n", filename, zip_file_strerror(file));
        exit(1);
    }

    if (zip_fclose(file) < 0) { //on ferme le fichier zip
        printf("can't close file '%s': %s\n", filename, zip_file_strerror(file));
        exit(1);
    }

    fclose(outfile); //on ferme le fichier filename
    printf("file '%s' extracted\n", filename);
}

int main() {
    struct zip *zip;
    int num_files;
    char *filename = NULL;
    zip = open_zip("test.zip");

    num_files = zip_get_num_files(zip); // pour tester si ça ouvrait bien , print le nombre de fichier
    printf("number of files in zip: %d\n", num_files);
    printf("files in zip:\n");
    for (int i = 0; i < num_files; i++) {
        const char *name = zip_get_name(zip, i, 0);
        printf("  %s\n", name);
    }
    printf("enter file to extract: ");
    scanf("%ms", &filename);
    extract_file(zip, filename);
    close_zip(zip);

    return 0;
}