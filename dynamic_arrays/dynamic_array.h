#ifndef EXTENDED_ARRAYS
#define EXTENDED_ARRAYS

#include <stdlib.h>

typedef unsigned char byte;

// array that automatically grows as needed
typedef struct da {
	void* p;				// the array.
	void* def_val;			// default value for array value
	size_t unit_size;		// size (in bytes) of each element.
	size_t current_length;	// amount of elements in array. in unit_size units, not in bytes.
	size_t max_length;		// max amount of elemnts able to be held.
} Dynamic_Array;

Dynamic_Array create_da(size_t, void*, size_t);

void da_free(Dynamic_Array);

void da_push(Dynamic_Array*, void*);

void da_insert(Dynamic_Array*, void*, size_t);

void da_remove(Dynamic_Array*, size_t);

void da_clear(Dynamic_Array*);

void da_read(Dynamic_Array*, size_t, void*);

void da_write(Dynamic_Array*, size_t, void*);

// array with ability to keep references to values without being changed from element deletion
// when you remove element that's referred to, save it as being deleted to fill later
typedef struct ada {
	Dynamic_Array vals;
	Dynamic_Array refs; // of bytes
	unsigned long int empties;
} Associative_Array;

Associative_Array create_ada(Dynamic_Array);

void ada_free(Associative_Array*);

size_t ada_push(Associative_Array*, void*);

void ada_insert(Associative_Array*, void*, size_t);

int ada_is_hole(Associative_Array, size_t);

void ada_remove(Associative_Array*, size_t);

void ada_clear(Associative_Array*);

void ada_iter(Associative_Array, void (*fun)(void*, size_t));

#define DAP(name, type, da) type* name = (type*) da // can't put in parenthesis cuz it's making a variable. duh.
#define INDAP(type, da) ((type*)da)
#define DA_PUSH(da, type, val) {type AAA = val; da_push(da, &AAA);}
#define _CREATE_DA(da, max_length, def_val, unit_type) {void* AAAAA = malloc(sizeof(unit_type));((unit_type*)AAAAA)[0] = def_val;da = create_da(max_length, AAAAA, sizeof(unit_type));}
#define CREATE_DA(da, max_length, def_val, unit_type) Dynamic_Array da; _CREATE_DA(da, max_length, def_val, unit_type);
#define DA_DUPLICATE_VAL(da, index, type, new_val) type new_val; da_read(&da, index, &new_val);
#define DA_WRITE_VAL(da, index, type, new_val) type temp = new_val; da_write(&da, index, &temp);

#define ADA_PUSH(ada, type, val) {type AAA = val; ada_push(ada, &AAA);}
#define ADA_PUSH_SAVE(ada, type, val, var) type AAA = val; type var = ada_push(ada, &AAA);
#define _CREATE_ADA(ada, max_length, def_val, unit_type) {void* AAAAA = malloc(sizeof(unit_type));((unit_type*)AAAAA)[0] = def_val;ada = create_ada(create_da(max_length, AAAAA, sizeof(unit_type)));}
#define CREATE_ADA(ada, max_length, def_val, unit_type) Associative_Array ada; _CREATE_ADA(ada, max_length, def_val, unit_type);



#endif