#pragma once

typedef struct __clove_vector_t __clove_vector_t;

const char* str_fmt(const char* format, ...);
void str_split(const char* str, char delim, __clove_vector_t* out_lines);
