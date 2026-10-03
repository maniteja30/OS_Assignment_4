#include "trace.h"

#include <sstream>

bool parseTraceLine(const std::string& line, MemoryAccess& access) {
    std::stringstream ss(line);

    std::string addressString;
    std::string typeString;
    std::string operationString;

    if (!std::getline(ss, addressString, ',')) {
        return false;
    }

    if (!std::getline(ss, typeString, ',')) {
        return false;
    }

    if (!std::getline(ss, operationString, ',')) {
        return false;
    }

    try {
        uint64_t address = std::stoull(addressString);

        // The assignment asks us to use only the lower 32 bits.
        access.address = static_cast<uint32_t>(address);
    }
    catch (...) {
        return false;
    }

    if (typeString.size() != 1 || operationString.size() != 1) {
        return false;
    }

    access.type = typeString[0];
    access.operation = operationString[0];

    return true;
}