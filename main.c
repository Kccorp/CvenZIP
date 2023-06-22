#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <zip.h>
#include <time.h>

#define MAX_FILENAME_LENGTH 256
#define MAX_PASSWORD_LENGTH 256



long generateRandomLong(long min, long max) {
    return min + (long)(rand() / (RAND_MAX / (max - min + 1) + 1));
}


int printZipFolder(char *filename){
    // Open zip file
    int error;
    zip_t *zip = zip_open(filename, 0, &error);
    if (zip == NULL) {
        printf("Impossible d'ouvrir le fichier zip\n");
        return 1;
    }

    // Get number of entries in zip file
    int num_entries = zip_get_num_entries(zip, 0);
    printf("Nombre d'entrées dans le fichier zip : %d\n", num_entries);


    for(int i=0; i<num_entries; i++)
    {
        // Use i to get the name of the file
        printf("%s\n", zip_get_name(zip, i, ZIP_FL_UNCHANGED));
    }

    // Close zip file
    zip_close(zip);

    return 0;

}





void extractFile(struct zip *zip, char *filename) { //fonction qui va extraire un fichier du zip et le stocker dans le dossier courant
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
        fwrite(buffer, 1, len, outfile); //on stocke dans le fichier filename
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

int extractAll(char *filename, char *password, char *cleCheckPassword){
//    printf("-------------extractAll-------------\n");

    // check password or not
    if(strcmp(password, cleCheckPassword)==0){
        strcpy(password, ""); // If password is cleCheckPassword(=default), set password to empty
    }


    // Open zip file
    int error;
    zip_t *zip = zip_open(filename, 0, &error);
    if (zip == NULL) {
        printf("Impossible d'ouvrir le fichier zip\n");
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
            printf("Impossible d'ouvrir le fichier dans le fichier zip\n.");
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

    return 0;

}



int Add_OverwriteFile(const char* fileZip, const char* pathFileInput, const char* pathFileOutput){
    // fonction qui va remplacer un fichier dans un zip par un autre fichier ou en ajouter un si il n'existe pas

    int check = 0;
    struct zip * f_zip=NULL;
    struct zip_source * n_zip=NULL;
    int err = 0;

    // Open zip file
    f_zip= zip_open(fileZip, 0, &err);
    if (f_zip == NULL) {
        printf("Impossible d'ouvrir le fichier zip\n");
        return 1;
    }

    // stock the filename in the zip_source
    if((n_zip=zip_source_file(f_zip,pathFileInput, 0, 0)) == NULL) {
        printf("%s\n", zip_strerror(f_zip));
        return 1;
    }

    //search the file in the zip
    check=zip_name_locate(f_zip,pathFileOutput,0);
    if (check==-1){
        printf("Le fichier %s n'existe pas dans %s\n", pathFileOutput, fileZip);

        // add a document in the zip file if the file is not in it
        if(zip_add(f_zip,pathFileOutput,n_zip) == -1)
        {
            printf("%s\n", zip_strerror(f_zip));
            zip_close(f_zip);
            f_zip = NULL;
            zip_source_free(n_zip);
            n_zip = NULL;
            return 1;
        }
        printf("Le fichier %s a été ajouté dans %s\n", pathFileOutput, fileZip);

    }
    else{
        // our document replaces the document located at the "check" location
        if(zip_replace(f_zip,check,n_zip) == -1) {
            printf("%s\n", zip_strerror(f_zip));
            zip_close(f_zip);
            f_zip = NULL;
            zip_source_free(n_zip);
            n_zip = NULL;
            return 1;
        }
        printf("Le fichier %s a été remplacé dans %s\n", pathFileOutput, fileZip);
    }


    return 0;

}



int main(int argc, char *argv[]) {
    printf("==============START==============\n");
    srand(time(NULL)); // Init random

    char *password = malloc(sizeof(char) * MAX_PASSWORD_LENGTH);
    char *filename = malloc(sizeof(char) * MAX_FILENAME_LENGTH);

    // Init no password
    long randomValue = generateRandomLong(1000000000, 2000000000);
    // int to string
    char cleCheckPassword[20];
    sprintf(cleCheckPassword, "%ld", randomValue);
        //printf("cleCheckPassword : %s\n", cleCheckPassword);
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
            printf("Examples :\n"
                   "        ./main -f monZip.zip -e -p password1234 => Extract zip file with the password 'password1234'\n"
                   "        ./main -i monZip.zip path/local/file.txt path/in/zip/file.txt  => Add or overwrite file\n"
                   "\nFeatures :\n"
                   "    - Utiliser avec des arguments \n"
                   "        --help, -h => affiche les aides\n"
                   "        --open, -o => ouvre le fichier\n"
                   "        --bruteforce, -b => bruteforce le fichier compressé\n"
                   "        --dictionary, -d => bruteforce avec un dictionnaire\n"
                   "        --password, -p => déverouille fichier en saisissant le mot de passe \n"
                   "        --extract [arg1] [arg2], -e [arg1] [arg2] => extrait un fichier \n"
                   "        --include [dossier_destination.zip] [file_add.txt] , -i [arg1] [arg2] => include un fichier \n"
                   "        \n"
                   "    - Ouvrir un fichier zip\n"
                   "        - Sans mot de passe\n"
                   "        - En saisissant un mot de passe\n"
                   "    \n"
                   "    - Bruteforce un fichier zip\n"
                   "        - Brutforce par dictionnaire \n"
                   "        - Brutforce par itération \n"
                   "        \n"
                   "    - Explorer le contenu du fichier unzip\n"
                   "        - afficher le contenu (comme un ls)\n"
                   "        - récupérer un fichier (dossier compressé > hôte)\n"
                   "        - insérer un fichier (hôte > dossier compressé)");

        } else {
            // Extract all
            if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--extract") == 0) {
                    // printf("-e DETECT\n");
                extractAll(filename, password, cleCheckPassword);
            }else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--include") == 0) {
                    // printf("-i DETECT\n");
                Add_OverwriteFile(argv[i+1], argv[i+2], argv[i+3]);

            }else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--open") == 0) {
                    // printf("-o DETECT\n");
                printZipFolder(argv[i+1]);

            }
        }
    }

    printf("==============END==============\n");

    return 0;
}
