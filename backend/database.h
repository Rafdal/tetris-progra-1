#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <varargs.h>

#define DB_MAX_STRING_LEN   128  // Usado para buffer interno

typedef enum { DB_TYPE_STRING, DB_TYPE_INT, DB_TYPE_UNSIGNED_LONG, } db_datatype_t;

typedef struct {
    FILE* file;
    char* filename;     // nombre del archivo de la BD
    char** fieldnames;  // arreglo de nombres de los campos de la bd
    uint8_t fields;     // cantidad de campos de la bd
    db_datatype_t* field_datatypes;   // arreglo de los ID del tipo de dato que tiene cada campo
    // Si necesitas poner mas campos ponelos
}database_t;

typedef struct {
}registry_t;

database_t* db_init(char* filename, uint8_t fields);
// 1) Inicializa la estructura (database_t) usada para controlar la base de datos. Retorna NULL en caso de error
// EJEMPLO: 
//          database_t *tabla = db_init("usuarios.csv", 3); // 3 = cantidad de campos

void db_init_field(database_t* db, uint8_t field_id, char* field_name, db_datatype_t datatype); // Inicializa un campo de la tabla
/* field_id: posicion del campo
 * EJEMPLO:
 *          db_init_field(tabla, 0, "dni", DB_TYPE_UNSIGNED_LONG);
 *          db_init_field(tabla, 1, "nombre", DB_TYPE_STRING);
 *          db_init_field(tabla, 2, "edad", DB_TYPE_INT);
 */

void db_destroy(database_t* db); // libera toda la memoria usada (al finalizar el programa)
// Ej:      db_destroy(tabla);





bool db_open(database_t* db); // retorna true si se pudo abrir exitosamente
// Esta funcion crea el archivo si no existe, y si existe, chequea que los campos coincidan (solo los nombres)
// EJEMPLO:  if(db_open(tabla)){ ... }

void db_close(database_t* db); // Cierra el archivo de la base de datos (se llama al final de leer/escribir todo)

//   MANIPULACION DE REGISTROS

int64_t db_add_new_registry(database_t* db, ...); // Agrega un nuevo registro al final. Retorna su posicion (id)
// Retorna:   id_registro >= 0 == OK  |  -1 == ERROR
// Debe verificar que coincidan los datos ingresados con los campos
// EJEMPLO:
//          db_add_new_registry(tabla, 43200300UL, "Jorge Palazza", 19);

int64_t db_search_registry_by_field(database_t* db, uint8_t field_id, ...);
// Busca EL PRIMER registro cuyo campo <field_id> coincida exactamente con el contenido ingresado
// Si coincide retorna el id del registro >= 0
// Si no coincide retorna -1
// EJEMPLO:
//          db_search_registry_by_field(tabla, 1, "Jorge Palazza"); // Si no hay ningun "Jorge Palazza", retorna -1

bool db_update_registry_field(database_t* db, int64_t reg_index, uint8_t field_id, ...); // actualiza el campo de un registro
// EJ:      db_update_registry_field(tabla, 16, 0, 41200300); // Actualiza solo un campo

bool db_update_registry(database_t* db, int64_t reg_index, ...); // Actualiza un registro entero
// EJ:      db_update_registry(tabla, 16, 41200300, "Ernesto Rancio", 16);

bool db_delete_registry(database_t* db, int64_t reg_index);
// EJ:      db_delete_registry(tabla, 16);  // borra el registro 16


// LECTURA DE REGISTROS
bool db_get_registry(database_t* db, int64_t reg_index, ...); // Lee todo el registro y lo guarda en los punteros ingresados
// EJEMPLO:
//                  char nombre[32];
//                  unsigned long dni;
//                  int edad;
//                  
//                  db_get_registry(tabla, 16, &dni, nombre, &edad);  // el orden de los campos debe coincidir


#endif _DATABASE_H_