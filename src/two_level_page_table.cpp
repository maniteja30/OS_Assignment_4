#include "two_level_page_table.h"

TwoLevelPageTable::TwoLevelPageTable()
    : nextFreeFrame(0) {
}

uint32_t TwoLevelPageTable::getFrame(uint32_t virtualPage) {

    // Count this virtual-page access.
    pageAccessCount[virtualPage]++;

    // Split the 20-bit virtual page number into:
    // 10-bit Level-1 index
    // 10-bit Level-2 index
    uint32_t level1Index = virtualPage >> 10;
    uint32_t level2Index = virtualPage & 0x3FF;

    // Check whether a Level-2 table already exists.
    auto level1It = pageTable.find(level1Index);

    if (level1It == pageTable.end()) {

        // First access to this Level-1 region.
        // Allocate a new Level-2 table.
        pageTable[level1Index] =
            unordered_map<uint32_t, uint32_t>();

        level1It = pageTable.find(level1Index);
    }

    auto& level2Table = level1It->second;

    // Check whether the virtual page is already mapped.
    auto level2It = level2Table.find(level2Index);

    if (level2It != level2Table.end()) {
        return level2It->second;
    }

    // First access to this virtual page.
    // Allocate the next free physical frame.
    uint32_t frame = nextFreeFrame;
    nextFreeFrame++;

    level2Table[level2Index] = frame;

    return frame;
}

uint32_t TwoLevelPageTable::getUsedFrameCount() const {

    uint32_t count = 0;

    for (const auto& level1Entry : pageTable) {
        count += static_cast<uint32_t>(
            level1Entry.second.size()
        );
    }

    return count;
}

uint32_t TwoLevelPageTable::getLevel1EntriesAllocated() const {

    return static_cast<uint32_t>(pageTable.size());
}

uint32_t TwoLevelPageTable::getLevel2EntriesAllocated() const {

    uint32_t count = 0;

    for (const auto& level1Entry : pageTable) {
        count += static_cast<uint32_t>(
            level1Entry.second.size()
        );
    }

    return count;
}

uint32_t TwoLevelPageTable::getTotalEntriesAllocated() const {

    return getLevel1EntriesAllocated()
         + getLevel2EntriesAllocated();
}

uint32_t TwoLevelPageTable::getTotalEntriesAccessed() const {

    // Every allocated Level-1 entry is accessed by at least
    // one virtual page in the trace.
    //
    // Every allocated Level-2 entry corresponds to a virtual
    // page that was accessed.
    //
    // Therefore, all allocated entries are accessed at least once.

    return getTotalEntriesAllocated();
}

uint64_t TwoLevelPageTable::getPageAccessCount(
    uint32_t virtualPage) const {

    auto it = pageAccessCount.find(virtualPage);

    if (it != pageAccessCount.end()) {
        return it->second;
    }

    return 0;
}

vector<pair<uint32_t, uint64_t>>
TwoLevelPageTable::getPageAccessCounts() const {

    vector<pair<uint32_t, uint64_t>> result;

    for (const auto& entry : pageAccessCount) {
        result.push_back(entry);
    }

    return result;
}