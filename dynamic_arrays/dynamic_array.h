#include <stdlib.h>

#ifndef DYNAMIC_ARRAYS
#define DYNAMIC_ARRAYS

typedef unsigned char byte;

// array that automatically grows as needed
typedef struct da {
	void* p;				// the array.
	void* def_val;			// default value for array value
	size_t unit_size;		// size (in bytes) of each element.
	size_t current_length;	// amount of elements in array. in unit_size units, not in bytes.
	size_t max_length;		// max amount of elemnts able to be held.
} Dynamic_Array;

Dynamic_Array create_da(void*, void*, size_t, size_t, size_t);

void da_push(Dynamic_Array*, void*);

void da_insert(Dynamic_Array*, void*, size_t);

void da_remove(Dynamic_Array*, size_t);

// array with ability to keep references to values without being changed from element deletion
// when you remove element that's referred to, save it as being deleted to fill later
typedef struct ada {
	Dynamic_Array vals;
	Dynamic_Array refs; // of bytes
	unsigned long int empties;
} Associative_Array;

Associative_Array create_ada(Dynamic_Array);

void ada_push(Associative_Array*, void*);

void ada_insert(Associative_Array*, void*, size_t);

void ada_remove(Associative_Array*, size_t);

#endif