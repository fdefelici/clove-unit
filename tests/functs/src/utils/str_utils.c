#include "utils/str_utils.h"
#include <clove-unit.h>
#include <string.h>

//NOTE: Dangerous if in future will run test multithreaded
//     Eventually improve with mutex or https://en.cppreference.com/w/c/atomic/atomic_flag
const char* str_fmt(const char* format, ...) {
    static char result[1024];
    va_list args;
    va_start(args, format);
    __clove_string_vsprintf(result, sizeof(result), format, args);
    va_end(args);
    return result;
}

void str_split(const char* str, char delim, __clove_vector_t* out_lines) {
    __CLOVE_VECTOR_INIT(out_lines, char*);
    char* source = __clove_string_strdup(str);
    char delim_str[2] = {delim, '\0'};

    #ifdef _WIN32
    char* context;
    char *token = strtok_s(source, delim_str, &context);
    #else 
    char *token = strtok(source, delim_str);
    #endif //_WIN32

    while (token != NULL) {
        __CLOVE_VECTOR_ADD(out_lines, char*, token);
        
        #ifdef _WIN32
        token = strtok_s(NULL, delim_str, &context);
        #else 
        token = strtok(NULL, delim_str);
        #endif //_WIN32
    }
}

