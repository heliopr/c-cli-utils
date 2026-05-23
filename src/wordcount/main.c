#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE *fptr = stdin; 

    if (argc > 1 && strcmp(argv[1], "-") != 0) {
        fptr = fopen(argv[1], "r");
        if (fptr == NULL) {
            perror(argv[1]);
            return EXIT_FAILURE;
        }
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

    if (fptr != stdin) {
        if (fclose(fptr)) {
            perror("fclose error");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}