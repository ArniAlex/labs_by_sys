#include <stdio.h>

int main(int argc, char* argv[]) {
	FILE* f;
	int n = 0;

	unsigned char buf[] = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5 };
	unsigned char r_buf[4];

	unsigned char b;

	if (argc != 2) { return -1; }
	
	f = fopen(argv[1], "wb"); 
	if (f == NULL) { return -2; }

	if (fwrite(buf, sizeof(unsigned char), 11, f) != 11) { 
		fclose(f); 
		return -3; 
	}

	fclose(f);

	f = fopen(argv[1], "rb");
	if (f == NULL) { return -2; }

	printf("Reading file byte by byte:\n");

	while (fread(&b, sizeof(unsigned char), 1, f) == 1) {
		printf("\nByte: %d\nFILE fields:\nPlaceholder:  %p\n", b, f->_Placeholder);
	}

	if (ferror(f)) {
		fclose(f);
		return -4;
	}

	fclose(f);

	//fseek

	f = fopen(argv[1], "rb");
	if (f == NULL) { return -2; }

	if (fseek(f, 3, SEEK_SET) != 0) {
		fclose(f);
		return -5;
	}

	if (fread(r_buf, sizeof(unsigned char), 4, f) != 4) {
		fclose(f);
		return -6;
	}

	printf("\nBuffer after fseek:\n");
	for (int i = 0; i < 4; i++) {
		printf("%d ", r_buf[i]);
	}
	printf("\n");

	fclose(f);

	return 0;
}