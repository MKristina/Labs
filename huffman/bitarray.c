#include "bitarray.h"

//work with bits
bitarray *create_arr(){
	bitarray *arr = malloc(sizeof(bitarray));
	arr->last_bit = 0;
	arr->skipped_bits = 0;
	arr->data = malloc(sizeof(unsigned char) * 2);
	arr->capacity_bit = 16;
	return arr;
}

void delete_bit(bitarray *arr){
	arr->last_bit--;
}

bitarray *clone(bitarray *arr){
	bitarray *new_arr = malloc(sizeof(bitarray));
	new_arr->last_bit = arr->last_bit;
	new_arr->skipped_bits = arr->skipped_bits;
	new_arr->capacity_bit = arr->capacity_bit;

	new_arr->data = malloc(sizeof(unsigned char) * arr->capacity_bit / 8);
	memcpy(new_arr->data, arr->data, new_arr->capacity_bit / 8);
	return new_arr;
}

void push_bit(bitarray *arr, unsigned char bit){
	int number_byte = arr->last_bit / 8;
	int number_bit = arr->last_bit % 8;

	if (arr->last_bit + 1 > arr->capacity_bit){
		arr->data = realloc(arr->data, arr->capacity_bit * 2 / 8);
		arr->capacity_bit *= 2;
	}

	if (bit){
		arr->data[number_byte] = (arr->data[number_byte]) | (128 >> number_bit);
	}
	else{
		arr->data[number_byte] = (arr->data[number_byte]) & ~(128 >> number_bit);
	}
	arr->last_bit++;
}

void push_byte(bitarray *arr, unsigned char byte){
	for (int i = 128; i != 0; i = i >> 1)
		push_bit(arr, byte & i);
}

void push_bitarr(bitarray *arr, bitarray *source){
	int count = 0;
	for (int j = 0;; j++){
		for (int i = 128; i != 0; i = i >> 1){
			push_bit(arr, source->data[j] & i);
			count++;
			if (count == source->last_bit)
				return;
		}
	}
}

unsigned char pop_bit(bitarray *arr){
	unsigned char byte = arr->data[arr->skipped_bits / 8];
	unsigned char bit = (byte >> (7 - arr->skipped_bits % 8));
	arr->skipped_bits++;
	return bit & 1;
}

unsigned char pop_byte(bitarray *arr){
	unsigned char data = 0;
	for (int i = 0; i < 8; i++){
		data <<= 1;
		data = data | pop_bit(arr);
	}
	return data;
}

void free_bitarr(bitarray *arr){
	if (arr != NULL){
		free(arr->data);
		free(arr);
	}
}