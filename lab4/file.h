#pragma once
#include <windows.h>
#include <fstream>
#include <string>

constexpr int MAX_MSG_LEN = 20;

constexpr const char* MUTEX_NAME = "mutex";
constexpr const char* EMPTY_NAME = "empty";
constexpr const char* FULL_NAME  = "full";
constexpr const char* READY_NAME = "ready";

struct Header
{
    long capacity;
    long head;
    long tail;
};

inline int recordOffset(long index)
{
    return sizeof(Header) + index * MAX_MSG_LEN;
}

inline void createHeaderFile(const std::string& name, long capacity)
{
    std::ofstream f(name, std::ios::binary | std::ios::trunc);

    Header h{capacity, 0, 0};
    f.write(reinterpret_cast<const char*>(&h), sizeof(h));
    char empty[MAX_MSG_LEN] = {'\0'};

    for (long i = 0; i < capacity; ++i)
        f.write(empty, MAX_MSG_LEN);
    f.flush();
}

inline void pushMessage(const std::string& name, const char* msg) {
    std::fstream f(name, std::ios::in | std::ios::out | std::ios::binary);

    Header h;
    f.seekg(0);
    f.read(reinterpret_cast<char*>(&h), sizeof(h));

    f.seekp(recordOffset(h.tail));
    f.write(msg, MAX_MSG_LEN);
    h.tail = (h.tail + 1) % h.capacity;

    f.seekp(0);
    f.write(reinterpret_cast<const char*>(&h), sizeof(h));
    f.flush();
}

inline void popMessage(const std::string& name, char* msg) {
    std::fstream f(name, std::ios::in | std::ios::out | std::ios::binary);

    Header h;
    f.seekg(0);
    f.read(reinterpret_cast<char*>(&h), sizeof(h));

    f.seekg(recordOffset(h.head));
    f.read(msg, MAX_MSG_LEN);

    h.head = (h.head + 1) % h.capacity;
    f.seekp(0);
    f.write(reinterpret_cast<const char*>(&h), sizeof(h));
    f.flush();
}
