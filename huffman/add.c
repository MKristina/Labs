#include "add.h"

//work with tree
void cnt(FILE *input, int *weight){
	const int size = 1024 * 3;
	unsigned char *read = malloc(sizeof(char) * size);
	int read_count;

	while ((read_count = fread(read, 1, size, input)) != 0){
		for (int i = 0; i < read_count; i++){
			weight[read[i]]++;
		}
	}
	free(read);
}

void build_table(bitarray **table, tree *root, bitarray *current){
	if (root->left == NULL){
		table[root->value] = clone(current);
	}
	else{
		push_bit(current, 0);
		build_table(table, root->left, current);
		delete_bit(current);

		push_bit(current, 1);
		build_table(table, root->right, current);
		delete_bit(current);
	}
}

void print_tree(tree *root, bitarray *arr){
	if (root->left != NULL){
		push_bit(arr, 1);
		print_tree(root->left, arr);
		print_tree(root->right, arr);
	}
	else{
		push_bit(arr, 0);
		push_byte(arr, root->value);
	}
}

void free_tree(tree *root){
	if (root != NULL){
		free_tree(root->left);
		free_tree(root->right);
		free(root);
	}
}

tree *load_tree(bitarray *arr)
{
	tree *result = malloc(sizeof(tree));
	if (pop_bit(arr) == 1){
		result->left = load_tree(arr);
		result->right = load_tree(arr);
	}
	else{
		result->left = NULL;
		result->right = NULL;

		result->value = pop_byte(arr);
	}
	return result;
}

bitarray *load_arr(FILE *out){
	bitarray *arr = malloc(sizeof(bitarray));

	arr->skipped_bits = 0;
	arr->last_bit = 0;
	arr->capacity_bit = 1024 * 8;
	arr->data = malloc(sizeof(unsigned char) * 1024);

	unsigned char *temp = malloc(sizeof(char) * 1024);
	int read_count = fread(temp, 1, 1024, out);

	while (read_count != 0){
		for (int i = 0; i < read_count; i++){
			if (arr->last_bit + 8 > arr->capacity_bit){
				arr->data = realloc(arr->data, arr->capacity_bit / 8 + 1024);
				arr->capacity_bit += 1024 * 8;
			}
			push_byte(arr, temp[i]);
		}
		read_count = fread(temp, 1, 1024, out);
	}
	return arr;
}

void read_symbol(bitarray *arr, tree *root, FILE *output){
	if (root->left == NULL){
		fputc(root->value, output);
		return;
	}
	if (pop_bit(arr) == 0){
		read_symbol(arr, root->left, output);
	}
	else{
		read_symbol(arr, root->right, output);
	}
}
