#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

#include "distros.h"

#define MAX_BUFFER 256

void get_cpu_name(char *buffer, size_t size) {
    FILE *fptr = fopen("/proc/cpuinfo", "r");
    if (!fptr) return;

    char line[256];
    while (fgets(line, sizeof(line), fptr)) {
        if (strncmp(line, "model name", 10) == 0) {
            char *name = strchr(line, ':');
            if (name) {
                name += 2;
                name[strcspn(name, "\n")] = '\0';
                strncpy(buffer, name, size - 1);
                buffer[size - 1] = '\0';
                break;
            }
        }
    }
    fclose(fptr);
}

void get_gpu_name(char *buffer, size_t size) {
    FILE *fptr = popen("lspci | grep -i 'vga\\|3d' | cut -d ':' -f3", "r");
    if (fptr) {
        if (fgets(buffer, size, fptr) != NULL) {
            buffer[strcspn(buffer, "\n")] = '\0';

            char *start = buffer;
            while (isspace((unsigned char)*start)) {
                start++;
            }

            if (start != buffer) {
                memmove(buffer, start, strlen(start) + 1);
            }
        }
        pclose(fptr);
    }
}

void get_distro_name(char *buffer, size_t size) {
    FILE *file = fopen("/etc/os-release", "r");
    if (file == NULL) {
        perror("Failed to open /etc/os-release");
    } else {
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
        
                strncpy(buffer, value, size - 1);
                buffer[size-1] = '\0';
                break;
            }
        }

        fclose(file);
    }
}

int main() {
    setlocale(LC_ALL, "");

    char *user = getenv("USER");
    if (user == NULL) {
        user = "?";
    }

    char distro_name[MAX_BUFFER] = "Unknown";
    get_distro_name(distro_name, MAX_BUFFER);

    long num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (num_cores <= 0) {
        perror("Failed to get CPU count");
    }

    long page_size = sysconf(_SC_PAGE_SIZE);
    long num_pages = sysconf(_SC_PHYS_PAGES);
    double ram = 0;
    if (page_size > 0 && num_pages > 0) {
        long long total_ram = (long long)num_pages * page_size;
        ram = (double)total_ram / (1024 * 1024 * 1024);
    } else {
        perror("Failed to get RAM info");
    }

    char cpu_name[MAX_BUFFER] = "Unknown";
    get_cpu_name(cpu_name, MAX_BUFFER);

    char gpu_name[MAX_BUFFER] = "Unknown";
    get_gpu_name(gpu_name, MAX_BUFFER);

    // IMPORTE: UPDATE THIS
    #define NUM_STATS 6
    char stats[NUM_STATS][MAX_BUFFER];
    snprintf(stats[0], MAX_BUFFER, "\U0001F464 %s", user);
    snprintf(stats[1], MAX_BUFFER, "\U0001F4BB %s", distro_name);
    snprintf(stats[2], MAX_BUFFER, "\u2699 %ld CPU Cores", num_cores);
    snprintf(stats[3], MAX_BUFFER, "\U0001F40F %.2fGB RAM", ram);
    snprintf(stats[4], MAX_BUFFER, "CPU: %s", cpu_name);
    snprintf(stats[5], MAX_BUFFER, "GPU: %s", gpu_name);

    char **art = linux_ascii;
    if (strncmp(distro_name, "Debian", 6) == 0) {
        art = debian_ascii;
    } else if (strncmp(distro_name, "Ubuntu", 6) == 0) {
        art = ubuntu_ascii;
    } else if (strncmp(distro_name, "Arch Linux", 10) == 0) {
        art = arch_ascii;
    } else if (strncmp(distro_name, "Fedora", 6) == 0) {
        art = fedora_ascii;
    } else if (strncmp(distro_name, "Linux Mint", 10) == 0) {
        art = mint_ascii;
    }

    int max_art_width = 0;
    int art_lines = 0;
    for (int i = 0; i < MAX_ASCII_LINES && art[i] != NULL; i++) {
        int len = strlen(art[i]);
        if (len > max_art_width) {
            max_art_width = len;
        }
        art_lines++;
    }

    int max_lines = (art_lines > NUM_STATS) ? art_lines : NUM_STATS;
    
    printf("\n");
    for (int i = 0; i < max_lines; i++) {
        if (i < art_lines) {
            printf("%-*s   ", max_art_width, art[i]); 
        } else {
            printf("%-*s   ", max_art_width, ""); 
        }

        if (i < NUM_STATS) {
            printf("%s", stats[i]);
        }
        
        printf("\n");
    }
    printf("\n");

    return 0;
}