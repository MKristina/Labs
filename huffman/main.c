#define _CRT_SECURE_NO_WARNINGS
#include "add.h"

int main(){
	FILE* in = fopen("in.txt", "rb");
	if (!in) {
		perror("File opening failed");
		return EXIT_FAILURE;
	}
	FILE* out = fopen("out.txt", "wb");
	if (!out) {
		perror("File opening failed");
		return EXIT_FAILURE;
	}

	fseek(in, 0, SEEK_END);
	int file_size = ftell(in);
	fseek(in, 0, SEEK_SET);
	if (file_size == 3) exit(0);

	unsigned char mode[4];
	fread(mode, sizeof(unsigned char), 3, in);

	if (mode[0] == 'c') {
		compress(in, out);
		fclose(in);
		fclose(out);
	}

	if (mode[0] == 'd') {
		decompress(in, out);
		fclose(in);
		fclose(out);
	}
	return 0;
}