#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>

const char *debian[] = {
    "        #:..        ",
    "    :####*++#####   ",
    "   ###         +##: ",
    "  #:             #  ",
    " #-              ## ",
    " #               ## ",
    " #      #       .#  ",
    " #       #     #:   ",
    " ##        *        ",
    "  ##                ",
    "   ##               ",
    "     ##             ",
    "        *=          ",
    NULL
};

int main() {
    setlocale(LC_ALL, "");

    char *user = getenv("USER");
    if (user == NULL) {
        user = "?";
    }

    // DISTRO
    FILE *file = fopen("/etc/os-release", "r");
    char distro_name[256] = "Unknown Linux Distribution";
    if (file == NULL) {
        perror("Failed to open /etc/os-release");
    }
    else {
        char line[256];

        while (fgets(line, sizeof(line), file)) {
            if (strncmp(line, "NAME=", 5) == 0) {
                char *value = line + 5;
                
                value[strcspn(value, "\r\n")] = '\0';
            
                if (value[0] == '"') {
                    value++;
                    if (value[strlen(value) - 1] == '"') {
                        value[strlen(value) - 1] = '\0';
                    }
                }
        
                strncpy(distro_name, value, sizeof(distro_name) - 1);
                break;
            }
        }

        fclose(file);
    }

    // CPU CORES
    long num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (num_cores <= 0) {
        perror("Failed to get CPU count");
    }

    // RAM
    long page_size = sysconf(_SC_PAGE_SIZE);
    long num_pages = sysconf(_SC_PHYS_PAGES);
    double ram = 0;
    if (page_size > 0 && num_pages > 0) {
        long long total_ram = (long long)num_pages * page_size;
        ram = (double)total_ram / (1024 * 1024 * 1024);
    } else {
        perror("Failed to get RAM info");
    }

    for (int i = 0; debian[i] != NULL; i++) {
        printf("%s\n", debian[i]);
    }

    printf("\n");
    printf("\U0001F464 %s\n", user);
    printf("\U0001F4BB %s\n", distro_name);
    printf("\u2699 %ld\n", num_cores);
    printf("\U0001F40F %.2f GB\n", ram);
    printf("\n");

    return 0;
}