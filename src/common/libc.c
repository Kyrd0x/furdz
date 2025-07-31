#include "libc.h"
#include <stdarg.h>

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

// Custom snprintf function
// int custom_snprintf(char *str, size_t size, const char *format, ...) {
//     int count;
//     va_list args;
//     va_start(args, format);
//     count = vsnprintf(str, size, format, args);
//     va_end(args);
//     return count;
// }
