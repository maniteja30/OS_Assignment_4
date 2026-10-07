#ifndef TWO_LEVEL_PAGE_TABLE_H
#define TWO_LEVEL_PAGE_TABLE_H

#include <cstdint>
#include <unordered_map>
#include <vector>

using namespace std;

class TwoLevelPageTable {
private:

    // Level 1:
    // L1 index -> Level 2 table
    unordered_map<uint32_t, unordered_map<uint32_t, uint32_t>> pageTable;

    // Number of accesses made to each virtual page
    unordered_map<uint32_t, uint64_t> pageAccessCount;

    // Next free physical frame
    uint32_t nextFreeFrame;

public:

    TwoLevelPageTable();

    // Returns the physical frame corresponding to a virtual page
    uint32_t getFrame(uint32_t virtualPage);

    // Number of distinct virtual pages mapped
    uint32_t getUsedFrameCount() const;

    // Number of Level-1 entries allocated
    uint32_t getLevel1EntriesAllocated() const;

    // Number of Level-2 entries allocated
    uint32_t getLevel2EntriesAllocated() const;

    // Total number of page-table entries allocated
    uint32_t getTotalEntriesAllocated() const;

    // Number of page-table entries accessed
    uint32_t getTotalEntriesAccessed() const;

    // Access count for a particular page
    uint64_t getPageAccessCount(uint32_t virtualPage) const;

    // Return all page access counts
    vector<pair<uint32_t, uint64_t>> getPageAccessCounts() const;
};

#endif