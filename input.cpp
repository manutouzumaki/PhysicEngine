#include "input.h"
#include <Windows.h>

FileResult read_file(const char* filepath)
{
    FileResult result = {};
    HANDLE file_handle = CreateFileA(filepath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (file_handle != INVALID_HANDLE_VALUE) {
        LARGE_INTEGER file_size;
        if (GetFileSizeEx(file_handle, &file_size)) {
            Assert(file_size.QuadPart <= 0xFFFFFFFF);
            result.data = malloc((size_t)file_size.QuadPart + 1);
            result.size = (size_t)file_size.QuadPart;
            if (ReadFile(file_handle, result.data, (DWORD)result.size, 0, 0)) {
                u8* last_byte = (u8*)result.data + result.size;
                *last_byte = 0;
                return result;
            }
        }
    }
    FileResult zeroResult = {};
    return zeroResult;
}
