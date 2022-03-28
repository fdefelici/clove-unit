#define CLOVE_SUITE_NAME RunTest
#include <clove-unit.h>

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <stdbool.h>
#include <sys/stat.h>

//TODO: Creare Utils e anche CROSS OS
int exec_cmd(const char* cmd, char** output) {
    //NOTE: Executing cmd twice, first time to get error code, second time to get contents
    //      Probably could be improved retrieving someway error code from _popen
    //int result_code = system(cmd);


    FILE* pipe = _popen(cmd, "rt");
    if (pipe == NULL) return -100;

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
    int result_code = _pclose(pipe);
    
    *output = buffer_head;
    return result_code;
}

bool file_exists(const char* path) {
    struct stat buffer;
    return stat(path, &buffer) == 0;
}

void file_delete(const char* path) {
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

/*

bool file_exists(const char* path) {
    FILE* file = __clove_file_open(path, "r");
    if (!file) return false;
    fclose(file);
    return true;
}
 */

char* cmd_out;

CLOVE_SUITE_SETUP() {
    cmd_out = NULL;
}

CLOVE_SUITE_TEARDOWN() {
    if (cmd_out) free(cmd_out);
}
 
CLOVE_TEST(Version) {
    /*
    const char* cmd = RES_PRJ01_EXEC_PATH" -v > nul 2>&1";
    int result = system(cmd);
    CLOVE_INT_EQ(0, result);
    */

    const char* cmd = RES_PRJ01_EXEC_PATH" -v";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    CLOVE_STRING_EQ(__CLOVE_VERSION, cmd_out);
}

//TODO: Sanitazzare parametri se iniziano e finiscono per "file name con spazi"
CLOVE_TEST(JsonReport) {
    char* base_path = __clove_path_basepath(RES_PRJ01_EXEC_PATH);
    char* report_path = __clove_path_concat(__CLOVE_PATH_SEPARATOR, base_path, "cmd_json_report.json");
    //file_delete(report_path);

    const char* cmd = str_fmt("\"%s\" -r json -f %s", RES_PRJ01_EXEC_PATH, report_path);
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_GENERIC, cmd_code); //1 test with failure

    CLOVE_IS_TRUE(file_exists(report_path));

    file_delete(report_path);
    free(base_path);
    free(report_path);
}

CLOVE_TEST(ConsoleReportIncludeOneTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -r console -i Prj01Suite01.Test01"; //-e per il futuro
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Suite / Tests found: 1 / 1"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "1) Prj01Suite01.Test01"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Total: 1, Passed: 1, Failed: 0, Skipped: 0"));
}

CLOVE_TEST(ConsoleReportIncludeTwoTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -r console -i Prj01Suite01.Test01 -i Prj01Suite02.Test21";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Suite / Tests found: 2 / 2"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "1) Prj01Suite01.Test01"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "2) Prj01Suite02.Test21"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Total: 2, Passed: 2, Failed: 0, Skipped: 0"));
}

CLOVE_TEST(DefaultReportIncludeOneTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -i Prj01Suite01.Test01";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Suite / Tests found: 1 / 1"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "1) Prj01Suite01.Test01"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Total: 1, Passed: 1, Failed: 0, Skipped: 0"));
}

CLOVE_TEST(DefaultReportExcludeOneTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -e Prj01Suite01.Test02";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Suite / Tests found: 2 / 2"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "1) Prj01Suite01.Test01"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "2) Prj01Suite02.Test21"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Total: 2, Passed: 2, Failed: 0, Skipped: 0"));
}

CLOVE_TEST(DefaultReportIncludeOverExcludeOneTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -i Prj01Suite01.Test01 -e Prj01Suite01.Test01";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Suite / Tests found: 1 / 1"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "1) Prj01Suite01.Test01"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Total: 1, Passed: 1, Failed: 0, Skipped: 0"));
}