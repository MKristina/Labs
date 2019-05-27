#include "decompress.h"
//decoding process
void decompress(FILE *input, FILE *output){
	if (fgetc(input) == 0){
		unsigned char symbol = fgetc(input);
		int cnt;
		fread(&cnt, sizeof(int), 1, input);
		for (int i = 0; i < cnt; i++)
			fputc(symbol, output);
		return;
	}

	fseek(input, 3, SEEK_SET);
	bitarray *arr = load_arr(input);
	if (arr->last_bit == 0)
		return;

	unsigned char delete = arr->data[arr->last_bit / 8 - 1];
	arr->last_bit = arr->last_bit - 8 - delete;
	tree *root = load_tree(arr);

	while (arr->last_bit > arr->skipped_bits)
		read_symbol(arr, root, output);
	
	free(arr->data);
	free(arr);
	free_tree(root);
}
