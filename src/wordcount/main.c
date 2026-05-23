#include <stdio.h>
#include <stdlib.h>

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
        if (ch != ' ' && ch != '\n' && (last_ch == ' ' || last_ch == '\n'))
            count++;
        last_ch = ch;
    }

    printf("%d\n", count);

    if (fclose(fptr))
        perror("fclose error");

    return EXIT_SUCCESS;
}