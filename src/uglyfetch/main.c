#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>

#include "distros.h"

#define MAX_BUFFER 256

void print_distro_art(const char *distro_name) {
    char **art = linux_ascii;

    if (strncmp(distro_name, "Debian", 6) == 0) {
        art = debian_ascii;
    } else if (strncmp(distro_name, "Ubuntu", 6) == 0) {
        art = ubuntu_ascii;
    } else if (strncmp(distro_name, "Arch", 4) == 0) {
        art = arch_ascii;
    } else if (strncmp(distro_name, "Fedora", 6) == 0) {
        art = fedora_ascii;
    }

    if (art == NULL) return;

    for (int i = 0; art[i] != NULL; i++) {
        printf("%s\n", art[i]);
    }
}

int main() {
    setlocale(LC_ALL, "");

    char *user = getenv("USER");
    if (user == NULL) {
        user = "?";
    }

    // DISTRO
    FILE *file = fopen("/etc/os-release", "r");
    char distro_name[MAX_BUFFER] = "Unknown";
    if (file == NULL) {
        perror("Failed to open /etc/os-release");
    }
    else {
        char line[MAX_BUFFER];

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

    print_distro_art(distro_name);

    printf("\n");
    printf("\U0001F464 %s\n", user);
    printf("\U0001F4BB %s\n", distro_name);
    printf("\u2699 %ld\n", num_cores);
    printf("\U0001F40F %.2f GB\n", ram);
    printf("\n");

    return 0;
}