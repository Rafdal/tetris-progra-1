#include "debug.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "./libs/easy_timer.h"

static FILE *file = NULL;
static char *filename = NULL;
static uint64_t start_stamp=0;

// -1 = ERROR   |   0 ~ MAX_DEBUG_FILES-1 = OK
bool debug_new_file(char* _filename)
{
    filename = _filename;

    if (file != NULL){
        fclose(file); // si hay uno abierto, lo cierro
    }

    if ((file = fopen(filename, "w")) == NULL)
    {
        printf("ERROR OPENING FILE!\n");
        return false;
    }
    start_stamp = easytimer_get_millis();
    fclose(file);
    return true;
}

// void deb_set_file_name(char* _filename){
//     filename = _filename;
// }
void deb_stamp(void){
    uint64_t dif = easytimer_get_millis()-start_stamp;
    deb_println("ms: %lu", dif);
}

int deb_println(const char *__format, ...){
    int out=1;

    if((file = fopen(filename, "a")) == NULL){
        printf("error opening file %s\n", filename);
    }
    else{
        va_list args; 
        va_start( args, __format); // copiar args
        char buf[1000];
        vsnprintf(buf, sizeof(buf), __format, args);
        va_end( args );
        out = fprintf(file, "%s", buf); // imprimirlos en el archivo
        fprintf(file, "\n");
        fflush(file);
        fclose(file);
    }
    return out;
}

int deb_print(const char *__format, ...){
    int out=1;
    if((file = fopen(filename, "a")) == NULL){
        printf("error opening file %s\n", filename);
    }
    else{
        va_list args; 
        va_start( args, __format); // copiar args
        char buf[1000];
        vsnprintf(buf, sizeof(buf), __format, args);
        va_end( args );
        out = fprintf(file, "%s", buf); // imprimirlos en el archivo
        fflush(file);
        fclose(file);
    }
    return out;
}