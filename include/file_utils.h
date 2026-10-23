//
// Created by jacobmawby on 09/09/2026.
//

#ifndef MINECRAFT_IN_C_FILE_UTILS_H
#define MINECRAFT_IN_C_FILE_UTILS_H
#include <stdlib.h>
#include <stdio.h>

static inline char* read_file(const char* path) {

    FILE *fptr = fopen(path, "r");
    if (fptr == nullptr) {
        printf("File Loading Failed \n");
        return nullptr;
    }
    int file_size = 0;
    char* buffer = malloc(sizeof(char));

    int c;
    while ((c = fgetc(fptr)) != EOF) {

        char *new_buffer = realloc(buffer, file_size + 2);

        if (new_buffer == nullptr) {
            free(buffer);
            free(new_buffer);
            printf("failed to add new char to file");
            return nullptr;
        }

        buffer = new_buffer;
        buffer[file_size++] = (char)c;
    }

    // If file is empty
    if (buffer == nullptr) {
        // So we can put the null terminator
        buffer = malloc(1);
        if (buffer == nullptr) {
            fclose(fptr);
            return nullptr;
        }
    }

    buffer[file_size] = '\0';

    fclose(fptr);
    return buffer;
}

#endif //MINECRAFT_IN_C_FILE_UTILS_H
