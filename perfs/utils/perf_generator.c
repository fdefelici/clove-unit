#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef _WIN32
#define strtok_s(str, delimiters, context) strtok_r(str, delimiters, context)
#define strncpy_s(strDest, numberOfElements, strSource, count) strncpy(strDest, strSource, count) == NULL
#define strcpy_s(dest, dest_size, src) strcpy(dest, src) == NULL
#define _strdup strdup
#endif


void write_suite_header(const char* name, FILE* file) {
    fprintf(file, "#define CLOVE_SUITE_NAME %s\n", name);
    fputs("#include \"clove-unit.h\"\n", file);
}

void write_suite_test(const char* name, FILE* file) {
    fprintf(file, "CLOVE_TEST(%s) {\n", name);
    fputs("\tCLOVE_PASS();\n", file);
    fputs("}\n", file);
}

const char* base_path(const char* path) {
    const char* last_addr = strrchr((const char*)path, '\\');
    int bytes_count;

    if (!last_addr) {
        bytes_count = strlen(path);
    } else {
        bytes_count = last_addr - path;
    }
    int size = bytes_count + 1; // +1 take into account null terminator

    char* base_path = (char*)malloc(sizeof(char) * size);
    strncpy_s(base_path, size, path, bytes_count);
    return base_path;
}


int main(int argc, char* argv[]) {

    char* path = base_path(argv[0]);
    char* file_path = (char*)calloc(strlen(path) + 2 + strlen("perf_test.c"), sizeof(char));
    strcat(file_path, path);
    strcat(file_path, "\\perf_test.c");

    FILE *file = fopen(file_path, "w");

    write_suite_header("Suite1", file);
    char buffer[10];
    for(int i=0; i < 10000; ++i) {
        sprintf(buffer, "Test%d", i);
        write_suite_test(buffer, file);
    }

    fclose(file);
    free(path);
    free(file_path);
    return 0;
}