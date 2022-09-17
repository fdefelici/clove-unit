#include "utils.h"
#include <clove-unit.h>

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

FILE* pipe_open(const char* cmd, const char* mode);
int   pipe_close(FILE* pipe);
#ifdef _WIN32
FILE* pipe_open(const char* cmd, const char* mode) {
    return _popen(cmd, mode);
}

int pipe_close(FILE* pipe) {
    return _pclose(pipe);
}

const char* cmd_fmt(const char* format, ...) {
    //Windows shell need to wrap command between "<command to be run>" when inside exists args that need "" to protect againts spaces
    //So an example of cmd would be:  ""my path/exec" -r json -f "my path/json""
    const size_t size = strlen(format) + 2 + 1;
    char* format_ext = (char*)malloc(size);
    __clove_string_sprintf(format_ext, size, "\"%s\"", format);

    static char result[1024];
    va_list args;
    va_start(args, format);
    __clove_string_vsprintf(result, sizeof(result), format_ext, args);
    va_end(args);
    free(format_ext);
    return result;
}
#else 
#include <sys/wait.h>
FILE* pipe_open(const char* cmd, const char* mode) {
    return popen(cmd, mode);
}
int pipe_close(FILE* pipe) {
    int status = pclose(pipe);
    return WEXITSTATUS(status);
}
const char* cmd_fmt(const char* format, ...) {
    static char result[1024];
    va_list args;
    va_start(args, format);
    __clove_string_vsprintf(result, sizeof(result), format, args);
    va_end(args);
    return result;
}
#endif

/* Example of execution silenting output
const char* cmd = RES_PRJ01_EXEC_PATH" -v > nul 2>&1";
int result = system(cmd);
CLOVE_INT_EQ(0, result);
*/

int exec_cmd(const char* cmd, char** output) {
    FILE* pipe = pipe_open(cmd, "r");
    if (pipe == NULL) {
        printf("Error: %s\n", strerror(errno));
        return -100;
    }

    if (output != NULL) {
        size_t size = 512;
        char* buffer_head = (char*)calloc(size, sizeof(char));
        char* buffer_chunk = buffer_head;

        size_t fetch_size = size;
        size_t fetch_size_current = fetch_size;
        size_t bytes_read = 0;
        size_t bytes_count = 0;
        while(fgets(buffer_chunk, fetch_size_current, pipe)) {
            bytes_read = strlen(buffer_chunk);
            buffer_chunk += bytes_read;
            bytes_count += bytes_read;

            fetch_size_current = fetch_size_current - bytes_read;
            if (fetch_size_current == 1) {
                size *= 2;
                buffer_head = (char*)realloc(buffer_head, size);
                buffer_chunk = buffer_head + bytes_count;
                fetch_size_current = fetch_size;
            }
        }   
        *output = buffer_head;
    }
    int result_code = pipe_close(pipe);
    return result_code;
}

bool file_exists(const char* path) {
    struct stat buffer;
    return stat(path, &buffer) == 0;
}

/*
bool file_exists(const char* path) {
    FILE* file = __clove_file_open(path, "r");
    if (!file) return false;
    fclose(file);
    return true;
}
 */

void file_delete(const char* path) {
    if (file_exists(path))
        remove(path);
}

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

