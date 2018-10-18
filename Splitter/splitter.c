#include <stdio.h>     /* printf, sprintf */
#include "splitter.h"  /* SplitFile, JoinFiles */
#include <stdlib.h>
int SplitFile(char const *  filename,  char const * output, size_t size) {
    FILE* pFile;
    FILE* writeFile;
    char* buffer;
    size_t result;
    long file_count;
    char current_file_name[50];
    
    file_count= 1;

    pFile = fopen ( filename , "rb" );
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

    
    buffer = malloc ( sizeof(char) * size );
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

    do{
        result = fread (buffer, sizeof(char), size, pFile);
        sprintf(current_file_name, "%s%04lu", output, file_count++);

        if(result>0 && result < size){
            writeFile = fopen( current_file_name, "wb" );
            fwrite (buffer , sizeof(char), result, writeFile);
            fclose(writeFile);
        }else if(result==size){
            writeFile = fopen( current_file_name, "wb" );
            fwrite (buffer , sizeof(char), size, writeFile);
            fclose(writeFile);
        }
    }while(result == size);

    free(buffer);
    fclose(pFile);

    return 0;
}

int JoinFiles(char** filenames, int num_files, char const * output) {
    FILE* readFile;
    FILE* writeFile;
    size_t result;
    int i;
    char* buffer;

    writeFile = fopen(output, "wb");
    buffer = malloc( sizeof(char) );

    
    for (i=0; i<num_files; i++) {
        printf("%s\n", *filenames);
        readFile = fopen(*filenames++, "rb");
        if (readFile==NULL) {fputs ("File error",stderr); exit (1);}

        result = 1;
        do{
            result = fread(buffer, sizeof(char), 1, readFile);
            if(result == 1){
                fwrite(buffer, sizeof(char), 1, writeFile);
            }
            
            free(buffer);
            buffer = malloc( sizeof(char) );
        }while(result == 1);

        fclose(readFile);
    }
    free(buffer);
    fclose(writeFile);

    return 0;
}
