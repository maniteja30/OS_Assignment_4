#ifndef PHYSICAL_MEMORY_H
#define PHYSICAL_MEMORY_H

#include <cstdint>
#include <list>
#include <unordered_map>
#include <vector>

using namespace std;

class PhysicalMemory {
private:

    // Maximum number of physical frames.
    uint32_t totalFrames;

    // virtual page -> physical frame
    unordered_map<uint32_t, uint32_t> pageToFrame;

    // physical frame -> virtual page
    unordered_map<uint32_t, uint32_t> frameToPage;

    // LRU list of virtual pages.
    // Front = most recently used.
    // Back = least recently used.
    list<uint32_t> lruList;

    // Position of each virtual page in the LRU list.
    unordered_map<uint32_t, list<uint32_t>::iterator> lruPosition;

    // Next free frame.
    uint32_t nextFreeFrame;

    // Statistics.
    uint64_t pageFaults;
    uint64_t replacements;

public:

    explicit PhysicalMemory(uint32_t numberOfFrames);

    // Returns the frame if the page is already in memory.
    // Returns false if the page causes a page fault.
    bool accessPage(
        uint32_t virtualPage,
        uint32_t& frame,
        bool& replacementOccurred
    );

    // Statistics.
    uint64_t getPageFaults() const;
    uint64_t getReplacements() const;
    uint32_t getTotalFrames() const;
    uint32_t getUsedFrames() const;

    double getPageFaultRate(uint64_t totalAccesses) const;

private:

    // Insert a page into a free frame.
    void insertIntoFreeFrame(
        uint32_t virtualPage,
        uint32_t frame
    );

    // Replace the least recently used page.
    uint32_t replaceLRU(
        uint32_t virtualPage
    );

    // Move a page to the MRU position.
    void makeMostRecentlyUsed(
        uint32_t virtualPage
    );
};

#endif