#include "libc.h"
#include <stdarg.h>
#include <fcntl.h>   // open()
#include <unistd.h>  // close()

#define MAX_OPEN_FILES 4

// static LWFILE file_pool[MAX_OPEN_FILES]; // Statically allocated

// LWFILE *custom_fopen(const char *filename, const char *mode) {
//     int flags;

//     if (mode[0] == 'r') {
//         flags = O_RDONLY;
//     } else if (mode[0] == 'w') {
//         flags = O_WRONLY | O_CREAT | O_TRUNC;
//     } else if (mode[0] == 'a') {
//         flags = O_WRONLY | O_CREAT | O_APPEND;
//     } else {
//         return NULL;
//     }

//     int fd = open(filename, flags, 0644);
//     if (fd < 0) return NULL;

//     for (int i = 0; i < MAX_OPEN_FILES; ++i) {
//         if (!file_pool[i].used) {
//             file_pool[i].fd = fd;
//             file_pool[i].mode = mode;
//             file_pool[i].used = 1;
//             return &file_pool[i];
//         }
//     }

//     close(fd);
//     return NULL;
// }

// int custom_fclose(LWFILE *stream) {
//     if (!stream || !stream->used) return -1;

//     int result = close(stream->fd);
//     stream->used = 0;
//     return result;
// }

//--------------------------------------------------------

// Compare two strings lexicographically
int custom_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) { 
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

// Copy a string with a maximum length, padding with '\0' if necessary
char *custom_strncpy(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    while (i < n) {  // Fill the rest with '\0' if src is shorter
        dest[i++] = '\0';
    }
    return dest;
}

// Calculate the length of a string
size_t custom_strlen(const char *str) {
    size_t length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

// Custom printf function (does nothing)
int custom_printf(const char* format, ...) {
    // Initialise la liste d'arguments, mais ne fait rien avec
    va_list args;
    va_start(args, format);
    va_end(args);

    // Retourne une valeur par défaut (comme si on avait "imprimé" 0 caractères)
    return 0;
}

// Custom rand function
int custom_rand() {
    return 1;
}

// Custom srand function
void custom_srand(unsigned int seed) {}

time_t custom_time(time_t *t) {
    static time_t fake_time = 652307613; // arbitrary value

    if (t) *t = fake_time;
    return fake_time;
}

void *custom_memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest;
}