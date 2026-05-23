#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        perror(argv[1]);
        return EXIT_FAILURE;
    }

    int ch = 0, last_ch = ' ';
    int count = 0;
    while ((ch = fgetc(fptr)) != EOF) {
        if (!isspace(ch) && isspace(last_ch)) {
            count++;
        }
        last_ch = ch;
    }

    printf("%d\n", count);

    if (fclose(fptr)) {
        perror("fclose error");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}