#include <stdio.h>
#include <string.h>

#pragma warning (disable: 4996)

#define BUF_SIZE 4096

int main(int argc, char* argv[])
{
    unsigned char buffer[BUF_SIZE];
    FILE* source = NULL;
    FILE* target = NULL;
    int bytes = 0;

    if (argc != 3) {
        return -1;
    }

    if (strcmp(argv[1], argv[2]) == 0) {
        return -2;
    }

    source= fopen(argv[1], "rb");
    if (source == NULL) return -1;

    target = fopen(argv[2], "wb");
    if (target == NULL) {
        fclose(source);
        return -1;
    }

    while ((bytes = fread(buffer, 1, BUF_SIZE, source)) > 0) {
        int bytes_wr = fwrite(buffer, 1, bytes, target);

        if (bytes_wr != bytes) {
            return -2;
        }

    }

    if (fclose(source) != 0) {
        return -3;
    }

    if (fclose(target) != 0) {
        return -3;
    }

    printf("The file has been copied successfully.");

    return 0;
}