#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 255

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file1> [file2 ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        FILE *fptr;
        fptr = fopen(argv[i], "r");

        if (fptr == NULL) {
            fprintf(stderr, "Error opening file '%s': ", argv[i]);
            perror("");
            continue;
        }

        char buffer[BUFFER_SIZE];
        while (fgets(buffer, BUFFER_SIZE, fptr) != NULL) {
            printf("%s", buffer);
        }
        
        fclose(fptr);
    }

    return EXIT_SUCCESS;
}