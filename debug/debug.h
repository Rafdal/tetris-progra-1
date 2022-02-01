#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>
#include <stdarg.h>

#ifdef USAR_DEBUG
#define DEBUG(msg, ...) deb_println(msg, __VA_ARGS__ );
#warning Usando DEBUG, para desactivar, comentar el    #define USAR_DEBUG     antes de incluir la libreria
#else
#define DEBUG(msg, ...) 
#endif

int debug_new_file(char* _filename); // Crear un nuevo archivo en blanco o borrar uno existente
void deb_set_file_name(char* _filename); // Setear nombre de archivo

int deb_println(const char *__format, ...); // Printf con salto de linea

int deb_print(const char *__format, ...); // Printf sin salto de linea

#endif