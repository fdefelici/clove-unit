#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct __clove_vector_t __clove_vector_t;

int exec_cmd(const char* cmd, char** output);
bool file_exists(const char* path);
void file_delete(const char* path);
const char* str_fmt(const char* format, ...);
const char* cmd_fmt(const char* format, ...);
void str_split(const char* str, char delim, __clove_vector_t* out_lines);
char* read_file(const char* path);