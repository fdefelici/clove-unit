#pragma once
#include <stdbool.h>

void  utils_mkdirs(char* path);
void  utils_rmdir(char* path);
char* utils_cwd();

bool utils_file_exists(const char* path);
void utils_file_delete(const char* path);
char* utils_file_read(const char* path);