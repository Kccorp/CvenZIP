//
// Created by m3kaniml on 23/06/23.
//

#include "basicTreatment.h"
#include "brutforceFile.h"


#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define WRONG_PASSWORD 6969

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

int PasswordGestion(char *password,const char *zipFile){
    printf("a password is needed to extract the file\n"
           "choose an option :\n"
              "1) enter a password\n"
                "2) bruteforce the password\n"
                    "3) exit\n");
    int choice;
    scanf("%d",&choice);
    switch (choice) {
        case 1:
            printf("enter the password: ");
            scanf("%s",password);
            removeTrailingNull(password);

            if (isZipPasswordEncrypted(zipFile, password, 1) == 1) {
                printf(ANSI_COLOR_RED "wrong password\n" ANSI_COLOR_RESET);
                PasswordGestion(password, zipFile);
            }

            return 0;

            break;
        case 2:
            printf("How do you want to bruteforce the password ?\n"
                   "1) with a dictionary\n"
                   "2) with a bruteforce\n");
            int choice2;
            scanf("%d",&choice2);
            if (choice2 == 1){
                askDicBrutforce (zipFile);
            }else if(choice2 == 2){
                askIterativeBrutforce(zipFile);
            }else{
                printf("wrong choice\n");
                PasswordGestion(password, zipFile);
            }
        case 3:
            return 1;
        default:
            printf("wrong choice\n");
            return 0;
    }
    return EXIT_SUCCESS;

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
                printf("passworrd\n");
                PasswordGestion(password, filename);
            }
            return 1;
        }
    }else {
        zip_set_default_password(zip, password);
        zip_file_t *file = zip_fopen_index(zip, 0, 0);
        if (file == NULL) {
            if (flag != 1) {
                printf("badpasswordTEST\n");
                PasswordGestion(password, filename);
            }
            return 1;
        }
        return 0;
    }
    printf("no password\n");
    zip_close(zip);

    return 0;
}

int extractAllFiles(char *zipName,char *password) {
    // Ouvre le fichier zip
    int error;
    char *pathFile=NULL;
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
        printf("for azeroth");
        strcpy(pathFile,zip_get_name(zip, i, ZIP_FL_UNCHANGED));
        printf("%s\n", pathFile);
        extractFile(zipName,pathFile,password);

    }

    // Ferme le fichier zip
    zip_close(zip);

    return 0;
}

int extractFile(char *zipName, char *extractFile,char *password) { //fonction qui va extraire un fichier du zip et le stocker dans le dossier courant

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
}

int Add_OverwriteFile(const char* fileZip, const char* pathFileInput, const char * pathFileOutput)
{

    // modifié pour ajouter la gestion des erreurs et quelques fonctionnalités
    printf("fileZip : %s\n", fileZip);
    printf("pathFileInput : %s\n", pathFileInput);
    printf("pathFileOutput : %s\n", pathFileOutput);


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