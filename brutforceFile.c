//
// Created by kbod on 19/06/23.
//
#include "brutforceFile.h"

void askDicBrutforce (char *pathToDic, int numberOfThreads){
    printf("Saisir le chemin vers le dictionnaire à utiliser : ");
    scanf("%s", pathToDic);

    printf("Saisir le nombre de thread(s) a utiliser : ");
    scanf("%d", &numberOfThreads);


    threadsDicController(pathToDic, numberOfThreads);
}

void askIterativeBrutforce (char *pathToZip, int numberOfThreads, int lengthPass){
//    printf("Saisir le chemin vers l'archive a extraire' : ");
//    scanf("%s", pathToZip);
//
//    printf("Saisir le nombre de thread(s) a utiliser : ");
//    scanf("%d", &numberOfThreads);

//    printf("Saisir la longueur du mot de passe : ");
//    scanf("%d", &lengthPass);

    char *numbers = "0123456789";
    iterativeBrutforce(numbers, 1, 2, pathToZip);
}

void threadsDicController (char *pathToDic, int numberOfThreads){

    int totalline = getLineNumber(pathToDic);
    int startLine = 0;

    struct arg_struct_dic args[numberOfThreads];

//    create args/struct for threads
    for(int i=0; i<numberOfThreads; i++){
        args[i].pathToDic = pathToDic;
        args[i].startLine = startLine;

        startLine = startLine + totalline/numberOfThreads;

        if(i == numberOfThreads-1){
            args[i].endLine = totalline;
        } else {
            args[i].endLine = startLine - 1;
        }
    }

//    create array of threads
    pthread_t *t=(pthread_t *)malloc(numberOfThreads * sizeof(pthread_t ));

//    create threads
    for(int i=0;i<numberOfThreads;i++){
        pthread_create(&t[i],NULL,&dicBrutforce,(void *)&args[i]);
    }

//    wait for all threads to finish
    for(int i=0;i<numberOfThreads;i++){
        pthread_join(t[i],NULL);
    }
}

void threadsIterController (char *pathToDic, int numberOfThreads, int lengthPass){

    struct arg_struct_iter args[numberOfThreads];

    //    create args/struct for threads
    for(int i=0; i<numberOfThreads; i++){
        args[i].pathToDic = pathToDic;
        args[i].max = lengthPass;
        args[i].min = lengthPass;
        args[i].chars = "0123456789";
    }

    //    create array of threads
    pthread_t *t=(pthread_t *)malloc(numberOfThreads * sizeof(pthread_t ));

//    create threads
    for(int i=0;i<numberOfThreads;i++){
        pthread_create(&t[i],NULL,&iterativeBrutforce,(void *)&args[i]);
    }

//    wait for all threads to finish
    for(int i=0;i<numberOfThreads;i++){
        pthread_join(t[i],NULL);
    }
}

int getLineNumber (char *pathToDic){
    //get the number of line of file
    FILE *file = fopen(pathToDic, "r");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier");
        exit(1);
    }
    int count = 0;
    char c;
    for (c = getc(file); c != EOF; c = getc(file))
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;
    fclose(file);

    return count;
}

void *dicBrutforce (void *arguments){

    //get the args
    struct arg_struct_dic *args = arguments;

    char *path = args->pathToDic;
    int startLine = args->startLine;
    int endLine = args->endLine;

//    open file
    FILE *file = fopen(path, "r");
    if (file == NULL){
        printf("cannot open dictionary file");
        exit(1);
    }

//  read file
    char line[256];
    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        if (i >= startLine && i <= endLine){
            printf("\nLecture de la ligne %d : %s", i, line);
        }
        i++;
    }

    fclose(file);

    return NULL;
}


void *iterativeBrutforce(char *chars, int min, int max, void *arguments) {
    int l;
    for (l = min; l <= max; ++l)
        workerBrutforce("", l, chars);
}

void workerBrutforce(char *current, int len, char *chars) {

    if (strlen(current) == len)
        printf("%s\n", current);

    if (strlen(current) < len) {
        for (int i = 0; i < strlen(chars); ++i) {
            char newCurrent[100]; // Adjust the size as per your requirement
            strcpy(newCurrent, current);
            strncat(newCurrent, &chars[i], 1);
            workerBrutforce(newCurrent, len, chars);
        }
    }
}



