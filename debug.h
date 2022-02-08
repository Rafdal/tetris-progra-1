#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#ifdef USAR_DEBUG
#define DEB_SET_FILE(f)  debug_new_file(f);
#define DEB_DELAY()  easytimer_delay(1000);
#define DEBUG(...)  {printf(__VA_ARGS__);putchar('\n');}
#define LOGTIME()   deb_stamp();
#define PULL_TIME() deb_update_stamp();
#define DEBLINE()   deb_println("%s: %u", __FILE__, __LINE__);
#define LOGN(...)  deb_println( __VA_ARGS__ );
#define LOG(...)  deb_print( __VA_ARGS__ );
#warning Usando DEBUG, para desactivar, comentar el    #define USAR_DEBUG     antes de incluir la libreria
#else
#define DEB_SET_FILE(f)
#define DEB_DELAY()
#define DEBUG(...)
#define LOGTIME()
#define PULL_TIME()  
#define DEBLINE()
#define LOGN(...)
#define LOG(...)
#endif

bool debug_new_file(char* _filename); // Crear un nuevo archivo en blanco o borrar uno existente
// void deb_set_file_name(char* _filename); // Setear nombre de archivo
void deb_stamp(void);
void deb_update_stamp(void);

int deb_print(const char *__format, ...); // Printf sin salto de linea

int deb_println(const char *__format, ...); // Printf con salto de linea

#endif