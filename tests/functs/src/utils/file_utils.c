#include "file_utils.h"
#include <clove-unit.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <shlwapi.h>
static void _utils_os_mkdir(const char* path) {
    _mkdir(path);
}

static int _utils_os_rmdir_recurs(const char* dir) {
    int dir_len = strlen(dir);
    int len = dir_len + 2;  // Extra space for double null termination
    char* tempDir = (char*) malloc(len * sizeof(char));
    if (!tempDir) {
        printf("Error: Failed to open directory: %s\n", dir);
        return -1;
    }

    memset(tempDir, 0, len * sizeof(char));
    __clove_memory_memcpy(tempDir, len, dir, dir_len);

    SHFILEOPSTRUCTA fileOp = {
        NULL,
        FO_DELETE,
        tempDir,
        NULL,
        FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT,
        false,
        0,
        "" };

    int ret = SHFileOperationA(&fileOp);
    free(tempDir);
    return ret; // 0 on success, non-zero on failure
}

#else 
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
//#include <sys/stat.h>
#include <sys/types.h>
static void _utils_os_mkdir(const char* path) {
    _mkdir(path, 0775);
}

static int _utils_os_rmdir_recurs(const char* dirPath) {
    DIR* d = opendir(dirPath);
    if (!d) {
        printf("Error: Failed to open directory: %s\n", dirPath);
        return -1;
    }

    struct dirent *entry;
    int ret = 0;

    while ((entry = readdir(d)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char fullPath[PATH_MAX];
            snprintf(fullPath, sizeof(fullPath), "%s/%s", dirPath, entry->d_name);

            if (entry->d_type == DT_DIR) {
                ret = _utils_os_rmdir_recurs(fullPath);
            } 
            else {
                ret = remove(fullPath);
            }

            if (ret != 0) {
                printf("Error: Failed to remove %s\n", fullPath);
                break; // Error occurred
            }
        }
    }

    closedir(d);
    if (ret == 0 && rmdir(dirPath) != 0) {
        printf("Error: Failed to remove directory: %s\n", dirPath);
        return -1;
    }
#endif

char* utils_cwd() {
    //eventually _getcwd (windows) and getcwd (posix) can be used
    return __clove_path_to_absolute(".");
}

void utils_mkdirs(char* path) {
    __clove_path_to_os(path);

    char* current_path = path;
    while(true) {
        char* next_sep_ptr = (char*)__clove_string_strstr(current_path, __CLOVE_PATH_SEPARATOR_STR);
        if (next_sep_ptr == NULL) break;
        
        *next_sep_ptr = '\0';
        //eventually check if path exists to avoid mkdir call
        _utils_os_mkdir(path);
        *next_sep_ptr = __CLOVE_PATH_SEPARATOR;

        current_path = next_sep_ptr + 1; 
    }
}

void utils_rmdir(char* path) {
    _utils_os_rmdir_recurs(path);
}

char* utils_file_read(const char* path) {
    FILE* file = __clove_file_open(path, "rb");
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char* result = (char*)malloc(file_size + 1);
    result[file_size] = '\0';
    size_t bytes_read = fread(result, file_size, 1, file);
    fclose(file);
    return result;
}

bool utils_file_exists(const char* path) {
    struct stat buffer;
    return stat(path, &buffer) == 0;
}

void utils_file_delete(const char* path) {
    if (utils_file_exists(path))
        remove(path);
}


