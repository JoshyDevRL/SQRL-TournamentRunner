#include "../include/shared_memory.h"

SharedMemory::SharedMemory(const std::string& name, size_t size) : name(name), size(size), hMapFile(nullptr), pBuf(nullptr) {
    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // Use paging file
        NULL,                    // Default security
        PAGE_READWRITE,          // Read/write access
        0,                       // Maximum object size (high-order DWORD)
        size,                    // Maximum object size (low-order DWORD)
        name.c_str());       // Name of mapping object

    if (hMapFile == NULL) {
        std::cerr << "Could not create file mapping object: " << GetLastError() << std::endl;
    }

    pBuf = (char*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, size);
    if (pBuf == NULL) {
        std::cerr << "Could not map view of file: " << GetLastError() << std::endl;
        CloseHandle(hMapFile);
    }
}

SharedMemory::~SharedMemory() {
    Close();
}

char* SharedMemory::GetPBuf() {
    return pBuf;
}

void SharedMemory::Close() {
    if (pBuf) {
        UnmapViewOfFile(pBuf);
        pBuf = nullptr;
    }
    if (hMapFile) {
        CloseHandle(hMapFile);
        hMapFile = nullptr;
    }
}
