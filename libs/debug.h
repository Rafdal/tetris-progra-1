#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>
#include <stdarg.h>

#ifdef USAR_DEBUG
#define LOG(...)  deb_println( __VA_ARGS__ );
#define DEBUG(...) {printf(__VA_ARGS__);putchar('\n');}
#warning Usando DEBUG, para desactivar, comentar el    #define USAR_DEBUG     antes de incluir la libreria
#else
#define LOG(...) 
#define DEBUG(...)
#endif

int debug_new_file(char* _filename); // Crear un nuevo archivo en blanco o borrar uno existente
// void deb_set_file_name(char* _filename); // Setear nombre de archivo

int deb_print(const char *__format, ...); // Printf con salto de linea

// int deb_println(const char *__format, ...); // Printf con salto de linea

#endif