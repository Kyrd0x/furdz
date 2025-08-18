#ifndef LIBC_H
#define LIBC_H

#ifdef LIGHTWEIGHT
    #include <stdarg.h>
    #include <stddef.h>
    #include <time.h>
    //------------------LIBC REDEFINITION---------------------------------------------------------
    // Custom implementations of standard C library functions
    #define strcmp custom_strcmp
    #define strncpy custom_strncpy
    #define strlen custom_strlen
    #define printf custom_printf
    #define rand custom_rand
    #define srand custom_srand
    #define time custom_time
    #define memcpy custom_memcpy
    // #define fopen custom_fopen
    // #define fclose custom_fclose

    
    
    int custom_strcmp(const char *s1, const char *s2);
    char *custom_strncpy(char *dest, const char *src, size_t n);
    size_t custom_strlen(const char *str);
    int custom_printf(const char* format, ...);
    int custom_rand();
    void custom_srand(unsigned int seed);
    time_t custom_time(time_t  *t);
    void *custom_memcpy(void *dest, const void *src, size_t n);

    // typedef struct {
    //     int fd;
    //     const char *mode;
    //     int used;
    // } LWFILE;

    // LWFILE *custom_fopen(const char *filename, const char *mode);
    // int custom_fclose(LWFILE *stream);

#else
    #include <string.h>  // pour strcmp, strncpy, strlen
    #include <stdio.h>   // pour printf
    #include <stdlib.h>  // pour rand

    // #define FILE LWFILE;
#endif

#endif // LIBC_H
