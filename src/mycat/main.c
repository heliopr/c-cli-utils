#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_SIZE 255

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file1> [file2 ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    bool newline = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-nl") == 0 || strcmp(argv[i], "-newline") == 0) {
            newline = true;
            continue;
        }

        FILE *fptr = fopen(argv[i], "r");

        if (fptr == NULL) {
            fprintf(stderr, "Error opening file '%s': ", argv[i]);
            perror(NULL);
            continue;
        }

        char buffer[BUFFER_SIZE];
        while (fgets(buffer, BUFFER_SIZE, fptr) != NULL) {
            printf("%s", buffer);
        }
        fclose(fptr);

        if (newline) {
            printf("\n");
        }
    }

    return EXIT_SUCCESS;
}