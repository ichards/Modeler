#include "dynamic_array.h"

//typedef unsigned char byte;

Dynamic_Array create_da(void* adap, void* def_val, size_t unit_size, size_t current_length, size_t max_length) {
	return ((Dynamic_Array) {adap, def_val, unit_size, current_length, max_length});
}

void da_grow(Dynamic_Array* da) {
	// needs to be resized, add eight to array length
	size_t new_size = da->max_length + 8;
	byte* new_array = malloc((new_size) * da->unit_size);
	byte* old_array = (byte*) da->p;
	// fill new array
	for (size_t i=0; i<da->max_length * da->unit_size; i++) {
		new_array[i] = old_array[i];
	}
	byte* def_byte = (byte*) da->def_val;
	for (size_t i=da->max_length; i < new_size; i++) {
		for (size_t j = 0; j<da->unit_size; j++) {
			new_array[(i * da->unit_size) + j] = def_byte[j];
		}
	} 
	// replace array
	da->p = (void*) new_array;
	// free old array
	free(old_array);
	// change max_length
	da->max_length = da->max_length + 8;
}

void da_push(Dynamic_Array* da, void* val) {
	
	// check if da needs to be resized
	if (da->current_length == da->max_length) {
		da_grow(da);
	}
	
	size_t new_val_idx = da->current_length++ * da->unit_size;
	byte* da_bytes = (byte*) da->p;
	byte* val_bytes = (byte*) val;
	
	for (size_t i=0; i<da->unit_size; i++) {
		da_bytes[new_val_idx + i] = val_bytes[i];
	}
}

void da_insert(Dynamic_Array* da, void* val, size_t idx) {
	
	// check if da needs to be resized
	// this is wrong, needs to be based on index
	if (da->current_length == da->max_length) {
		da_grow(da);
	}

	// make space for new value if needed (byte by byte)
	size_t idx_byte = idx * da->unit_size;
	byte* da_bytes = ((byte*) da->p);
	
	// start at end of empty end element and work back, grabbing byte unit_size iterations back
	for (size_t i = ((da->current_length + 1) * da->unit_size) - 1; i >= idx_byte; i--) {
		da_bytes[i] = da_bytes[i - da->unit_size];
	}
	// insert element into index
	
	byte* val_byte = (byte*) val;
	for (size_t i = 0; i<da->unit_size; i++) {
		da_bytes[idx_byte+i] = val_byte[i];
	}
}

void da_remove(Dynamic_Array* da, size_t idx) {
	byte* da_bytes = (byte*) da->p;
	for (size_t i = idx * da->unit_size; i < ((da->current_length - 1) * da->unit_size) - 1; i++) {
		da_bytes[i] = da_bytes[i + da->unit_size];
	}
	da->current_length--;
}

Associative_Array create_ada(Dynamic_Array da) {
	static byte ref_def = 0b11111111;
	byte* bytep = malloc(sizeof(byte) * 1);
	bytep[0] = 0b11111111;
	Associative_Array ada = (Associative_Array) {
		da,
		(Dynamic_Array) {bytep, (void*) &ref_def, sizeof(byte), 0, 1}
		};
	return ada;
}


void ada_push(Associative_Array* ada, void* val) {
	if (ada->empties == 0) {
		size_t new_val_idx = ada->vals.current_length;
		da_push(&ada->vals, val);
		if (ada->vals.current_length % 8 == 0) { // needs more bytes for refs
			byte new_ref = 0b11111111;
			da_grow(&(ada->refs));
			//da_push(&ada->refs, (void*) &new_ref);
		}
		// mark in refs
		size_t val_byte = new_val_idx / 8;
		size_t val_bit = new_val_idx % 8;
		byte* ref_bytes = (byte*) ada->refs.p;
		ref_bytes[val_byte] ^= (0b10000000 >> val_bit);
		
	} else {
		// find leftmost one bit
		byte* ref_bytes = (byte*) ada->refs.p;
		for (size_t cur_byte = 0; cur_byte<ada->refs.current_length; cur_byte++) {
			if (ref_bytes != 0) { // there's a one in here somewhere
				for (size_t cur_bit=0; cur_bit<8; cur_bit++) {
					if ((ref_bytes[cur_byte] & (0b10000000 >> cur_bit)) > 0) { // found it!
						
						//ada->vals.p[(cur_byte * 8) + cur_bit];
						byte* da_bytes = (byte*) (ada->vals.p);
						byte* val_bytes = (byte*) val;
						size_t new_val_idx = ((cur_byte * 8) + cur_bit) * ada->vals.unit_size;
						for (size_t i=0; i<ada->vals.unit_size; i++) {
							da_bytes[new_val_idx + i] = val_bytes[i];
						}
						
						ref_bytes[cur_byte] ^= (0b10000000 >> cur_bit); // get rid of that 1
					}
				}
			}
		}
		ada->vals.current_length++;
		ada->empties--;
	}
}

void ada_insert(Associative_Array* ada, void* val, size_t idx) {
	
	da_insert(&ada->vals, val, idx);

	// iterate through refs and increment those at or after index
	for (size_t i=0; i<ada->refs.current_length; i++) {
		if (((size_t*)(ada->refs.p))[i] >= idx) {
			((size_t*)(ada->refs.p))[i]++;
		}
	}
}

int ada_is_hole(Associative_Array ada, size_t idx) {
	byte* ref_bytes = (byte*) ada.refs.p;
	size_t cur_byte = idx / 8;
	size_t cur_bit = idx % 8;
	if ((ref_bytes[cur_byte] & (0b10000000 >> cur_bit)) > 0) {
		return 1;
	}
	return 0;
}

void ada_remove(Associative_Array* ada, size_t idx) {
	// this will be different from da_remove
	// instead, the ada will hold a da of bytes, where each bit represents whether
	// a value in the array is valid (0) or has been removed (1).
	// this is so the references do not become invalid.
	// when pushing another value, it will then fill in the next invalid bit.
	// the ada will have a counter of removed points to know whether to check or not.
	byte* da_bytes = (byte*) ada->vals.p;
	byte* def_val = (byte*) ada->vals.def_val;
	for (size_t i = idx; i < (idx + 1); i++) {
		for (size_t j = 0; j<ada->vals.unit_size; j++) {
			da_bytes[(i * ada->vals.unit_size) + j] = def_val[j];
		}
	}
	
	// keep track of removed index
	byte* ref_bytes = (byte*) ada->refs.p;

	byte idx_byte = idx / 8;
	byte idx_bit = idx % 8;

	byte removed = 0b10000000;
	removed = removed >> (idx_bit);
	ref_bytes[idx_byte] |= removed;
	ada->empties++;
	

	
	ada->vals.current_length--;
}

#include <stdio.h>
void ada_print_ref(Associative_Array ada) {
	byte* ada_ref_bytes = (byte*) ada.refs.p;
	printf("byte count: %d\n", ada.refs.max_length);
	for (size_t i=0; i<ada.refs.max_length; i++) {
		for (int j=7; j>=0; j--) {
			int bit = 0;
			if ((ada_ref_bytes[i] >> j) & 0b00000001) {
				bit = 1;
			}
			printf("%d", bit);
		}
		printf("\n");
	}
}





















