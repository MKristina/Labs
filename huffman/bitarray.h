#ifndef HUFFMAN_BITARRAY_H
#define HUFFMAN_BITARRAY_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct bitarray{
	int last_bit;
	int skipped_bits;
	unsigned char *data;
	int capacity_bit;
} bitarray;

bitarray *create_arr();

bitarray *load_arr(FILE *out);

void push_bit(bitarray *arr, unsigned char bit);

void delete_bit(bitarray *arr);

bitarray *clone(bitarray *arr);

void push_byte(bitarray *arr, unsigned char byte);

unsigned char pop_bit(bitarray *arr);

unsigned char pop_byte(bitarray *arr);

void push_bitarr(bitarray *arr, bitarray *source);

void free_bitarr(bitarray *arr);

#endif
