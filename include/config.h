#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>

constexpr uint64_t PAGE_SIZE = 4096;
constexpr uint64_t PAGE_TABLE_ENTRIES = 1ULL << 20;
constexpr uint64_t MEMORY_ACCESS_CYCLES = 200;
constexpr uint64_t TLB_ACCESS_CYCLES = 1;

#endif