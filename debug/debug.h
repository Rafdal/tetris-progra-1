#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>
#include <stdarg.h>


int debug_new_file(char* _filename);
void deb_set_file_name(char* _filename);

int deb_println(const char *__format, ...);

int deb_print(const char *__format, ...);

#endif