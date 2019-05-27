#ifndef HUFFMAN_ADD_H
#define HUFFMAN_ADD_H

#include <stdio.h>

#include "queue.h"
#include "bitarray.h"

void cnt(FILE *input, int *weight);

void build_table(bitarray **table, tree *root, bitarray *current);

void print_tree(tree* root, bitarray *arr);

void free_tree(tree *root);

tree *load_tree(bitarray *arr);

bitarray *load_arr(FILE *out);

void read_symbol(bitarray *arr, tree *root, FILE *decompressed);

#endif 