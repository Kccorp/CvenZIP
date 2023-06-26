//
// Created by m3kaniml on 23/06/23.
//

#include "basicTreatment.h"
#include "brutforceFile.h"


#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"
#include <sys/stat.h>

#define WRONG_PASSWORD 6969

int createParentDirectories(const char *path) {
    char temp[100];
    strcpy(temp, path);
    char *delimiter = strrchr(temp, '/');

    if (delimiter != NULL) {
        *delimiter = '\0';  // Supprime le dernier '/'
        if (mkdir(temp, 0777) != 0) {  // Crée le dossier parent
            printf("Impossible de créer le dossier parent : %s\n", temp);
            return 1;
        }
        *delimiter = '/';  // Restaure le dernier '/'
    }

    return 0;
}
int printZipFolder(char *zipName){
    // Ouvre le fichier zip
    int error;
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
        printf("%s\n", zip_get_name(zip, i, ZIP_FL_UNCHANGED));
    }

    // Ferme le fichier zip
    zip_close(zip);

    return 0;

}

int PasswordGestion(char *password,const char *zipName){
    printf("Un mot de passe est nécessaire pour extraire le fichier\n"
           "Choisissez une option :\n"
              "1) Saisir un mot de passe\n"
                "2) Bruteforce le mot de passe\n"
                    "3) Exit\n");
    int choice;
    scanf("%d",&choice);
    switch (choice) {
        case 1:
            printf("Saisir le mot de passe : ");
            scanf("%s",password);
            removeTrailingNull(password);

            if (isZipPasswordEncrypted(zipName, password, 1) == 1) {
                printf(ANSI_COLOR_RED "Mauvais mot de passe\n" ANSI_COLOR_RESET);
                PasswordGestion(password, zipName);
            }
            return 0;
        case 2:
            printf("Comment voulez-vous bruteforcer le mot de passe ?\n"
                   "1) Méthode : Dictionnaire \n"
                   "2) Méthode : Itération\n");
            int choice2;
            scanf("%d",&choice2);
            if (choice2 == 1){
                askDicBrutforce (zipName);
            }else if(choice2 == 2){
                askIterativeBrutforce(zipName);
            }else{
                printf("Verifiez votre saisis\n");
                PasswordGestion(password, zipName);
            }
        case 3:
            return 1;
        default:
            printf("Verifiez votre saisis\n");
            return 0;
    }
}

int isZipPasswordEncrypted(const char* filename,char *password, int flag)
{
    int error;
    zip_t *zip = zip_open(filename, 0, &error);
    if (zip == NULL) {
        printf("Impossible d'ouvrir le fichier zip\n");
        return 1;
    }


    if(strcmp(password,"")==0){
        zip_file_t *file = zip_fopen_index(zip, 0, 0);
        if (file == NULL) {
            if (flag != 1) {
                printf("Mot de passe nécessaire\n");
                PasswordGestion(password, filename);
            }
            return 1;
        }
    }else {
        zip_set_default_password(zip, password);
        zip_file_t *file = zip_fopen_index(zip, 0, 0);
        if (file == NULL) {
            if (flag != 1) {
                printf("Mauvais mot de passe\n");
                PasswordGestion(password, filename);
            }
            return 1;
        }

        return 0;

    }

    zip_close(zip);
    return 0;
}

int extractAllFiles(char *zipName,char *password) {
    // Ouvre le fichier zip
    int error;
    char *pathFile= malloc(sizeof(char)*255);
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
        strcpy(pathFile,zip_get_name(zip, i, ZIP_FL_UNCHANGED));
        printf("%s\n", pathFile);

        if (strlen(pathFile) > 0 && pathFile[strlen(pathFile) - 1] == '/') {
            int result = mkdir(pathFile, 0777);

            if (result == 0) {
                printf("Le dossier %s a été créé.\n", pathFile);
            } else {
                printf("Impossible de créer le dossier %s.\n", pathFile);
            }
        } else {
            extractFile (zipName,pathFile,password);
            return 0;  // La chaîne ne se termine pas par "/"
        }

    }

    // Ferme le fichier zip
    zip_close(zip);

    return 0;
}

int extractFile(char *zipName, char *extractFile,char *password) { //fonction qui va extraire un fichier du zip et le stocker dans le dossier courant

//    // Création des dossiers de destination
//    // Get string avant le dernier '/'
//    char temp[100];
//    strcpy(temp, extractFile);
//    char *pathToExtract = strrchr(temp, '/');  // Trouve la dernière occurrence de '/' dans la chaîne
//
//    if (pathToExtract != NULL) {
//        *(pathToExtract + 1) = '\0';  // Ajoute un caractère nul pour terminer la sous-chaîne à cet emplacement
////            printf("Résultat : %s\n", temp); // Affiche le chemin du dossier de destination
//    } else {
//        printf("Erreur lors de la recuperation du chemin de destination.\n");
//        return 1;
//    }
//    //MKDIR
//    printf("zipname : %s\n",zipName);
//    printf("extractFile : %s\n",extractFile);
//    printf("temp : %s\n",temp);
//
//    int result = mkdir(temp, 0777);
//
//    if (result == 0) {
//        printf("Le dossier %s a été créé.\n", temp);
//    } else {
//        printf("Impossible de créer le dossier %s .\n", temp);
//    }


    if (createParentDirectories(extractFile) == 0) {
        if (mkdir(extractFile, 0777) == 0) {
            printf("Le dossier %s a été créé.\n", extractFile);
        } else {
            printf("Impossible de créer le dossier %s.\n", extractFile);
        }
    }
    isZipPasswordEncrypted(zipName,password,0);
    if (strcmp(extractFile,"all") == 0){
        extractAllFiles(zipName, password);
    }else{
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
            return 1;
        }





        // EXTRACT FILE
        struct zip_file *file;
        char buffer[100]; //buffer qui va contenir le contenu du fichier , a modifier pour que ça soit dynamique
        int len;
        int err;

        printf("Extraction en cours de '%s'...\n", extractFile);


        file = zip_fopen_encrypted(zip, extractFile, 0, password); // Utilisez zip_fopen_encrypted pour ouvrir un fichier zip protégé par mot de passe
        if (file == NULL) {
            printf("1 - Impossible d'ouvrir le fichier '%s': %s\n", extractFile, zip_strerror(zip));
            return WRONG_PASSWORD;
        }


        FILE *outfile = fopen(extractFile, "w");
        if (outfile == NULL) {
            printf("2 - Impossible d'ouvrir le fichier '%s': %s\n", extractFile, strerror(errno));
            return 1;
        }


        while ((len = zip_fread(file, buffer, sizeof(buffer))) > 0) { //tant qu'on peut lire dans le fichier zipName
            fwrite(buffer, 1, len, outfile); //on stocke dans le fichier filename
        }

        if (len < 0) {
            printf("1 - Impossible de lire le fichier '%s': %s\n", extractFile, zip_file_strerror(file));
            return 1;
        }

        if (zip_fclose(file) < 0) { //on ferme le fichier zip
            printf("2 - Impossible de lire le fichier'%s': %s\n", extractFile, zip_file_strerror(file));
            return 1;
        }

        fclose(outfile); //on ferme le fichier filename
        printf("Fichier '%s' extrait\n", extractFile);
    }
}

int Add_OverwriteFile(const char* fileZip, const char* pathFileInput, const char * pathFileOutput)
{

    // Default output is input
    if (strcmp(pathFileOutput, "NULL") == 0) {
        strcpy(pathFileOutput, pathFileInput);
    }


    int visu = 0;
    struct zip * f_zip=NULL;
    struct zip_source * n_zip=NULL;
    int err = 0;

    // Ouvre le fichier zip
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

    // Ferme le fichier zip
    zip_close(f_zip);

    return 0;

}

void printHelp(){
printf("DESCRIPTION\n"
       "    Manipulation de fichier compressé\n"
       "\n"
    ANSI_COLOR_RED"    -h"ANSI_COLOR_RESET","ANSI_COLOR_RED" --help\n"ANSI_COLOR_RESET
    "        Afficher l'aide\n"
    "        \n"
//    ANSI_COLOR_RED"    -f"ANSI_COLOR_RESET","ANSI_COLOR_RED" --file " ANSI_COLOR_YELLOW "<filename.zip>\n"ANSI_COLOR_RESET
//    "        Selectionner le fichier compressé\n"
    "    \n"
    ANSI_COLOR_RED"    -p"ANSI_COLOR_RESET","ANSI_COLOR_RED" --password " ANSI_COLOR_YELLOW "<monMotDePasse>\n"ANSI_COLOR_RESET
    "        Mot de passe du fichier compressé\n"
    "        Ex: ./main dossier.zip -e all -p monMotDePasse\n"
    "        \n"
    ANSI_COLOR_RED"    -s"ANSI_COLOR_RESET","ANSI_COLOR_RED" --show \n"ANSI_COLOR_RESET
    "        Afficher le contenu d'un fichier compressé\n"
    "        Ex: ./main dossier.zip -s \n"
    "            ./main dossier.zip --show\n"
    "        \n"
   ANSI_COLOR_RED"    -o"ANSI_COLOR_RESET","ANSI_COLOR_RED" --output " ANSI_COLOR_YELLOW " <pathFile> \n"ANSI_COLOR_RESET
   "         Chemin output\n"
   "\n"

    ANSI_COLOR_RED"    -i"ANSI_COLOR_RESET","ANSI_COLOR_RED" --include " ANSI_COLOR_YELLOW " <arg1> "ANSI_COLOR_RED"-o "ANSI_COLOR_YELLOW"<arg2>\n"ANSI_COLOR_RESET
    "        Ajouter un fichier dans un fichier compressé\n"
    "            * arg1 : chemin/fichier que l'on souhaite ajouter dans le fichier compressé\n"
    "            * arg2 : chemin/fichier où l'on souhaite enregistrer dans le fichier compressé\n"
    "        Ex: ./main dossier.zip -i file.txt -o sousDossier/ici/file.txt\n"
    "            \n"
    ANSI_COLOR_RED"    -e"ANSI_COLOR_RESET","ANSI_COLOR_RED" --extract " ANSI_COLOR_YELLOW "{<zipFile>,"ANSI_COLOR_RED"all"ANSI_COLOR_YELLOW"}\n"ANSI_COLOR_RESET
    "        Extrait les fichiers passé en argument (all pour tout extraire) \n"
    "        Ex: ./main dossier.zip -e file.txt\n"
    "            ./main dossier.zip -e file.txt -p monMotDePasse\n"
    "            ./main dossier.zip -e all\n"
    "            ./main dossier.zip -e all -p monMotDePasse\n"
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