#ifdef __cplusplus
extern "C" {  //only visible by C++ compiler
#endif
#ifndef SPLITTER_H
#define SPLITTER_H
#include <stddef.h> /* size_t definition */

enum 
{E_BAD_SOURCE=1, E_BAD_DESTINATION, E_NO_MEMORY, E_NO_ACTION, E_SMALL_SIZE};

int SplitFile(char const *  filename,  char const * output, size_t size);
int JoinFiles(char** filenames, int num_files, char const * output);

#endif 

#ifdef __cplusplus
} //only visible by C++ compiler
#endif