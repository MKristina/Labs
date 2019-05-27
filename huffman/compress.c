#include "compress.h"

//encoding process
void compress(FILE *input, FILE *output){
	int *counts = calloc(256, sizeof(int));
	cnt(input, counts);
	queue *q = NULL;

	for (int i = 0; i < 256; ++i){
		unsigned char byte = (unsigned char)i;
		if (counts[byte] != 0){
			tree *root = malloc(sizeof(tree));
			root->frequency = counts[byte];
			root->value = byte;
			root->right = NULL;
			root->left = NULL;
			q = push(q, root);
		}
	}
	free(counts);

	if (q == NULL) return;

	if (q->next == NULL){
		fputc(0, output);
		fputc(q->value->value, output);
		fwrite(&q->value->frequency, sizeof(int), 1, output);
		return;
	}

	while (q->next != NULL){
		tree *a;
		tree *b;
		q = pop(q, &a);
		q = pop(q, &b);
		tree *root = malloc(sizeof(tree));
		root->frequency = a->frequency + b->frequency;
		root->right = b;
		root->left = a;
		q = push(q, root);
	}

	bitarray **table = calloc(sizeof(bitarray*), 256);
	bitarray *current = create_arr();
	build_table(table, q->value, current);
	free_bitarr(current);

	bitarray *out = create_arr();
	print_tree(q->value, out);

	free_tree(q->value);
	free(q);

	fseek(input, 3, SEEK_SET);
	unsigned char *read = malloc(sizeof(char) * BUFF_SIZE);
	int read_count = fread(read, 1, BUFF_SIZE, input);

	while (read_count != 0){
		for (int i = 0; i < read_count; i++){
			push_bitarr(out, table[read[i]]);
		}
		read_count = fread(read, 1, BUFF_SIZE, input);
	}

	free(read);
	for (int i = 0; i < 256; i++)
		free_bitarr(table[i]);
	free(table);

	int nulll_count = 0;
	while (out->last_bit % 8 != 0){
		push_bit(out, 0);
		nulll_count++;
	}
	push_byte(out, nulll_count);
	fwrite(out->data, 1, out->last_bit / 8, output);
	free_bitarr(out);
}