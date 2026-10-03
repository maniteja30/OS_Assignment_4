#ifndef TRACE_H
#define TRACE_H

#include <cstdint>
#include <string>

struct MemoryAccess{
    uint32_t address;
    char type;
    char operation;
};

bool parseTraceLine(const std:: string& line, MemoryAccess& access);

#endif