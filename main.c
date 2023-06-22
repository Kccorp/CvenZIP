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
    // Demande le nom du fichier à décompresser
    char filename[MAX_FILENAME_LENGTH];
    printf("Entrez le nom du fichier à décompresser : ");
    fgets(filename, MAX_FILENAME_LENGTH, stdin);
    filename[strcspn(filename, "\n")] = '\0';  // Supprime le caractère de nouvelle ligne

    // Demande le mot de passe
    char password[MAX_PASSWORD_LENGTH];
    printf("Entrez le mot de passe du fichier (laissez vide s'il n'y en a pas) : ");
    fgets(password, MAX_PASSWORD_LENGTH, stdin);
    password[strcspn(password, "\n")] = '\0';  // Supprime le caractère de nouvelle ligne

    // Ouvre le fichier zip
    int error;
    zip_t *zip = zip_open(filename, 0, &error);
    if (zip == NULL) {
        printf("Impossible d'ouvrir le fichier zip : %s\n", zip_strerror(error));
        return 1;
    }

    // Vérifie si un mot de passe est nécessaire
    if (zip_set_default_password(zip, password) < 0) {
        printf("Mot de passe incorrect ou nécessaire pour extraire le fichier.\n");
        zip_close(zip);
        return 1;
    }

    // Obtient le nombre d'entrées dans le fichier zip
    int num_entries = zip_get_num_entries(zip, 0);
    printf("Nombre d'entrées dans le fichier zip : %d\n", num_entries);

    // Parcourt les entrées et extrait les fichiers
    for (int i = 0; i < num_entries; i++) {
        // Obtient l'information de l'entrée
        const char *name = zip_get_name(zip, i, 0);
        printf("Extraction de : %s\n", name);

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
