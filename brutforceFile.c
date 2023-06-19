//
// Created by kbod on 19/06/23.
//
#include "brutforceFile.h"

void askDicBrutforce (char *pathToDic, int numberOfThreads){
    printf("Saisir le chemin vers le dictionnaire à utiliser : ");
    scanf("%s", pathToDic);

    printf("Saisir le nombre de thread(s) a utiliser : ");
    scanf("%d", &numberOfThreads);


//    dicBrutforce(pathToDic);
    threadsDicController(pathToDic, numberOfThreads);
}

void askIterativeBrutforce (char *pathToZip, int numberOfThreads){
//    printf("Saisir le chemin vers l'archive a extraire' : ");
//    scanf("%s", pathToZip);
//
//    printf("Saisir le nombre de thread(s) a utiliser : ");
//    scanf("%d", &numberOfThreads);

    iterativeBrutforce(pathToZip);
}

void threadsDicController (char *pathToDic, int numberOfThreads){

    int totalline = getLineNumber(pathToDic);
    int startLine = 0;

    struct arg_struct args[numberOfThreads];

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
    struct arg_struct *args = arguments;

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

void *iterativeBrutforce (void *arguments){
    char numbers[] = "0123456789";

    int length = strlen(numbers);

    for(int i=0; i<length; i++){
        workerBrutforce(numbers, length, numbers[i]);
    }
}

void workerBrutforce (char *caracters, int length, char prefix){
    for (int i = 0; i < length; ++i) {
        printf("\n%c%c", prefix, caracters[i]);
    }
}
