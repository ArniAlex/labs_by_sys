#include <stdio.h>
#include <string.h>

int is_prime(unsigned char n) {
    if (n < 2) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

void xor8(FILE* f) {
    unsigned char byte;
    unsigned char res = 0;

    while (fread(&byte, sizeof(unsigned char), 1, f) == 1) {
        res ^= byte;
    }
    printf("%u\n", res);
}

void xorodd(FILE* f) {
    unsigned int chunk;
    unsigned int result = 0;

    while (fread(&chunk, sizeof(unsigned int), 1, f) == 1) {
        unsigned char* bytes = (unsigned char*)&chunk;
        int has_prime = 0;

        for (int i = 0; i < 4; i++) {
            if (is_prime(bytes[i])) {
                has_prime = 1;
                break;
            }
        }

        if (has_prime) {
            result ^= chunk;
        }
    }
    printf("%u\n", result);
}

void mask(FILE* f, char* _mask) {
    unsigned int mask_value;
    sscanf(_mask, "%x", &mask_value);

    unsigned int num;
    int count = 0;

    while (fread(&num, sizeof(unsigned int), 1, f) == 1) {
        if ((num & mask_value) == mask_value) {
            count++;
        }
    }
    printf("%d\n", count);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <file> <flag> [mask_value]\n", argv[0]);
        return -1;
    }

    FILE* file = fopen(argv[1], "rb");
    if (!file) {
        perror("Error opening file");
        return -2;
    }

    char* flag = argv[2];

    if (strcmp(flag, "xor8") == 0) {
        xor8(file);
    }
    else if (strcmp(flag, "xorodd") == 0) {
        xorodd(file);
    }
    else if (strcmp(flag, "mask") == 0) {
        if (argc < 4) {
            printf("Error: mask requires hex value\n");
            fclose(file);
            return -3;
        }
        mask(file, argv[3]);
    }
    else {
        printf("Unknown flag: %s\n", flag);
    }

    fclose(file);
    return 0;
}