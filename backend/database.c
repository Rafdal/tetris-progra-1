#include <database.h>
#include <string.h>

database_t* db_init(char* filename, uint8_t fields){

    database_t* db = (database_t*) malloc(sizeof(database_t));

    if(db != NULL){
        db->filename = filename;
        db->file = NULL;
        db->fields = fields;

        db->fieldnames = (char**) calloc(fields, sizeof(char*));
        db->field_datatypes = (db_datatype_t*) calloc(fields, sizeof(db_datatype_t));

        // en caso de error al iniciar los arreglos
        if(db->fieldnames == NULL || db->field_datatypes == NULL){
            db_destroy(db); // libero la memoria
            db = NULL; // (marcador de error)
        }
    }
    return db;
}

void db_init_field(database_t* db, uint8_t field_id, char* field_name, db_datatype_t datatype){
    if(db != NULL){
        if(field_id < db->fields){
            db->fieldnames[field_id] = field_name;
            db->field_datatypes[field_id] = datatype;
        }
    }
}

// libera toda la memoria usada (al finalizar el programa)
void db_destroy(database_t* db){
    free(db->field_datatypes);
    free(db->fieldnames);
    free(db);
}


// FUNCIONES DE MANIPULACION DE BASES DE DATOS

// retorna true si se pudo abrir exitosamente
// Esta funcion crea el archivo si no existe, y si existe, chequea que los campos coincidan (solo los nombres).
//      Si no coinciden, borra el contenido del archivo y lo vuelve a crear con el formato correcto
// EJEMPLO:  if(db_open(tabla)){ ... }
bool db_open(database_t* db){
    if(db != NULL){

        db->file = fopen(db->filename, "a+");
        if(db->file != NULL){ // si el archivo se abrio correctamente
            bool format_ok_flag = true; // chequea que los nombres de los campos coincidan
            uint8_t field, ic; // indices (for)
            char c;

            fpos_t pos;
            pos.__state.__value.

            for(field=0; field < db->fields && format_ok_flag; field++){ // recorro cada campo

                uint8_t namesize = strlen(db->fieldnames[field]);

                for(ic=0; ic < namesize && format_ok_flag; ic++){ // recorro cada caracter del campo

                    c = fgetc(db->file); // leo el caracter del nombre del campo
                    
                    if(c == EOF){
                        printf("File EOF!\n");
                        format_ok_flag = false;
                    }
                    if(c != db->fieldnames[field][ic])
                        printf("Comparing %s failed in char %u = '%c' != %c\n", db->fieldnames[field], ic, db->fieldnames[field][ic], c);
                        format_ok_flag = false;
                }

                c = fgetc(db->file);
                switch (c){
                    case EOF:
                    case '\n':
                        if(field < db->fields-1){
                            printf("File EOF or '\\n' !\n");
                            format_ok_flag = false;
                        }
                        break;
                    case ',':
                        break;
                }
            }
            if(!format_ok_flag){
                for(field=0; field < db->fields && format_ok_flag; field++){
                    db->file = freopen(db->filename, "w");
                }
            }
        }
    }
    return false;
}