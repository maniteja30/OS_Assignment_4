
#include "trace.h"

#include <charconv>
#include <string>

bool parseTraceLine(const std::string& line, MemoryAccess& access) {
    const size_t comma1 = line.find(',');
    if (comma1 == std::string::npos) {
        return false;
    }

    const size_t comma2 = line.find(',', comma1 + 1);
    if (comma2 == std::string::npos) {
        return false;
    }

    if (line.find(',', comma2 + 1) != std::string::npos) {
        return false;
    }

    uint64_t address = 0;

    const char* begin = line.data();
    const char* addressEnd = begin + comma1;

    auto result = std::from_chars(begin, addressEnd, address);
    if (result.ec != std::errc{} || result.ptr != addressEnd) {
        return false;
    }

    if (comma2 != comma1 + 2 || line.size() != comma2 + 2) {
        return false;
    }

    access.address = static_cast<uint32_t>(address);
    access.type = line[comma1 + 1];
    access.operation = line[comma2 + 1];

    return (access.type == 'I' || access.type == 'D') &&
           (access.operation == 'R' || access.operation == 'W');
}