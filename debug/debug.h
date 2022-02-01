#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>
#include <stdarg.h>


int debug_new_file(char* _filename); // Crear un nuevo archivo en blanco o borrar uno existente
void deb_set_file_name(char* _filename); // Setear nombre de archivo

int deb_println(const char *__format, ...); // Printf con salto de linea

int deb_print(const char *__format, ...); // Printf sin salto de linea

#endif