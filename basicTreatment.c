//
// Created by m3kaniml on 23/06/23.
//

#include "basicTreatment.h"


#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define WRONG_PASSWORD 6969


long generateRandomLong(long min, long max) {
    return min + (long)(rand() / (RAND_MAX / (max - min + 1) + 1));
}


int printZipFolder(char *filename){
    // Ouvre le fichier zip
    int error;
    zip_t *zip = zip_open(filename, 0, &error);
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
        printf("%s\n", zip_get_name(zip, i, ZIP_FL_UNCHANGED));
    }

    // Ferme le fichier zip
    zip_close(zip);

    return 0;

}

int checkPassword(char *zipName, char *extractFile,char *password){
    // Ouvre le fichier zip
    int error;
    zip_t *zip = zip_open(zipName, 0, &error);
    if (zip == NULL) {
        printf(" Impossible d'ouvrir le fichier zip\n");
        return 1;
    }
    // Vérifie si un mot de passe est nécessaire
    if (zip_set_default_password(zip, password) < 0) {
        printf("Mot de passe incorrect ou nécessaire pour extraire le fichier.\n");
        zip_close(zip);
        return 1;
    }

// TEST EXTRACT FILE TO CHECK THE PASSWORD
    struct zip_file *file;
    file = zip_fopen_encrypted(zip, extractFile, 0, password); // Utilisez zip_fopen_encrypted pour ouvrir un fichier zip protégé par mot de passe
    if (file == NULL) {
        printf("Wrong password\n");
        return WRONG_PASSWORD;
    }else{
        printf("Good password\n");
        return 0;
    }
}


int extractFile(char *zipName, char *extractFile,char *password,char *cleCheckPassword) { //fonction qui va extraire un fichier du zip et le stocker dans le dossier courant

// PASSWORD + OPEN ZIPNAME
    // check password or not
    if(strcmp(password, cleCheckPassword)==0){
        strcpy(password, ""); // If password is cleCheckPassword(=default), set password to empty
    }
    // Ouvre le fichier zip
    int error;
    zip_t *zip = zip_open(zipName, 0, &error);
    if (zip == NULL) {
        printf(" Impossible d'ouvrir le fichier zip\n");
        return 1;
    }
    // Vérifie si un mot de passe est nécessaire
    if (zip_set_default_password(zip, password) < 0) {
        printf("Mot de passe incorrect ou nécessaire pour extraire le fichier.\n");
        zip_close(zip);
        return 1;
    }

// EXTRACT FILE
    struct zip_file *file;
    char buffer[100]; //buffer qui va contenir le contenu du fichier , a modifier pour que ça soit dynamique
    int len;
    int err;

    printf("extracting file '%s'...\n", extractFile);


    file = zip_fopen_encrypted(zip, extractFile, 0, password); // Utilisez zip_fopen_encrypted pour ouvrir un fichier zip protégé par mot de passe
    if (file == NULL) {
        printf("1 - Can't open file '%s': %s\n", extractFile, zip_strerror(zip));
        return WRONG_PASSWORD;
    }


    FILE *outfile = fopen(extractFile, "w");
    if (outfile == NULL) {
        printf("2 - Can't open file '%s': %s\n", extractFile, strerror(errno));
        return 1;
    }


    while ((len = zip_fread(file, buffer, sizeof(buffer))) > 0) { //tant qu'on peut lire dans le fichier zipName
        fwrite(buffer, 1, len, outfile); //on stocke dans le fichier filename
    }

    if (len < 0) {
        printf("can't read file '%s': %s\n", extractFile, zip_file_strerror(file));
        return 1;
    }

    if (zip_fclose(file) < 0) { //on ferme le fichier zip
        printf("can't close file '%s': %s\n", extractFile, zip_file_strerror(file));
        return 1;
    }

    fclose(outfile); //on ferme le fichier filename
    printf("file '%s' extracted\n", extractFile);
}

int extractAll(char *filename, char *password, char *cleCheckPassword){
//    printf("-------------extractAll-------------\n");

    // check password or not
    if(strcmp(password, cleCheckPassword)==0){
        strcpy(password, ""); // If password is cleCheckPassword(=default), set password to empty
    }
    // Ouvre le fichier zip
    int error;
    zip_t *zip = zip_open(filename, 0, &error);
    if (zip == NULL) {
        printf(" Impossible d'ouvrir le fichier zip\n");
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



int Add_OverwriteFile(const char* fileZip, const char* pathFileInput, const char* pathFileOutput)
{

    // modifié pour ajouter la gestion des erreurs et quelques fonctionnalités

    int visu = 0;
    struct zip * f_zip=NULL;
    struct zip_source * n_zip=NULL;
    int err = 0;

    // Ouvre le fichier zip
    int error;
    f_zip= zip_open(fileZip, 0, &err);
    if (f_zip == NULL) {
        printf("Impossible d'ouvrir le fichier zip\n");
        return 1;
    }

    // on met dans le zip_source le fichier que l'on veut remplacer
    if((n_zip=zip_source_file(f_zip,pathFileInput, 0, 0)) == NULL) {
        printf("%s\n", zip_strerror(f_zip));
        return 1;
    }

    // recherche de l'emplacement du fichier dans le zip
    visu=zip_name_locate(f_zip,pathFileOutput,0);
    if (visu==-1){
        printf("Le fichier %s n'existe pas dans %s\n", pathFileOutput, fileZip);

        // c'est là qu'on fixe le nom qu'aura le nouveau document dans le fichier zip
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
        // modification d'un document dans le fichier zip : le fichier est déjà dedans
        // notre document remplace le document qui se trouve à l'emplacement visu
        if(zip_replace(f_zip,visu,n_zip) == -1)
        {
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

void printHelp(){
printf("DESCRIPTION\n"
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
}

int checkIfFileExist(char *path){
    FILE *file;
    if ((file = fopen(path, "r"))){
        fclose(file);
        return 1;
    }
    return 0;
}