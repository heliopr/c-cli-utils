#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error: No files were given.\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        FILE *fptr;
        char buffer[255];
        fptr = fopen(argv[i], "r");

        if (fptr != NULL) {
            while (fgets(buffer, 255, fptr) != NULL) {
                printf("%s", buffer);
            }
            
            fclose(fptr);
        }
    }

    return 0;
}