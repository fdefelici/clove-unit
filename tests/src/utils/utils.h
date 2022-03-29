#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int exec_cmd(const char* cmd, char** output);
bool file_exists(const char* path);
void file_delete(const char* path);
const char* str_fmt(const char* format, ...);
const char* cmd_fmt(const char* format, ...);