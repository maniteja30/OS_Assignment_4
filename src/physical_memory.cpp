#include "physical_memory.h"

PhysicalMemory::PhysicalMemory(uint32_t numberOfFrames)
    : totalFrames(numberOfFrames),
      nextFreeFrame(0),
      pageFaults(0),
      replacements(0) {
}

bool PhysicalMemory::accessPage(
    uint32_t virtualPage,
    uint32_t& frame,
    bool& replacementOccurred
) {
    replacementOccurred = false;

    auto it = pageToFrame.find(virtualPage);

    if (it != pageToFrame.end()) {
        frame = it->second;
        makeMostRecentlyUsed(virtualPage);
        return true;
    }

    pageFaults++;

    if (nextFreeFrame < totalFrames) {
        frame = nextFreeFrame;
        nextFreeFrame++;

        insertIntoFreeFrame(virtualPage, frame);

        return false;
    }

    frame = replaceLRU(virtualPage);
    replacementOccurred = true;

    return false;
}
void PhysicalMemory::insertIntoFreeFrame(
    uint32_t virtualPage,
    uint32_t frame
) {

    pageToFrame[virtualPage] = frame;
    frameToPage[frame] = virtualPage;

    lruList.push_front(virtualPage);
    lruPosition[virtualPage] = lruList.begin();
}

uint32_t PhysicalMemory::replaceLRU(
    uint32_t virtualPage
) {

    // The page at the back is the least recently used.
    uint32_t oldPage = lruList.back();

    uint32_t frame = pageToFrame[oldPage];

    // Remove old page from the mappings.
    pageToFrame.erase(oldPage);
    frameToPage.erase(frame);

    // Remove old page from the LRU list.
    lruPosition.erase(oldPage);
    lruList.pop_back();

    // Insert the new page into the same frame.
    pageToFrame[virtualPage] = frame;
    frameToPage[frame] = virtualPage;

    lruList.push_front(virtualPage);
    lruPosition[virtualPage] = lruList.begin();

    replacements++;

    return frame;
}

void PhysicalMemory::makeMostRecentlyUsed(
    uint32_t virtualPage
) {

    auto it = lruPosition.find(virtualPage);

    if (it == lruPosition.end()) {
        return;
    }

    lruList.erase(it->second);

    lruList.push_front(virtualPage);

    lruPosition[virtualPage] = lruList.begin();
}

uint64_t PhysicalMemory::getPageFaults() const {
    return pageFaults;
}

uint64_t PhysicalMemory::getReplacements() const {
    return replacements;
}

uint32_t PhysicalMemory::getTotalFrames() const {
    return totalFrames;
}

uint32_t PhysicalMemory::getUsedFrames() const {
    return static_cast<uint32_t>(pageToFrame.size());
}

double PhysicalMemory::getPageFaultRate(
    uint64_t totalAccesses
) const {

    if (totalAccesses == 0) {
        return 0.0;
    }

    return (100.0 * pageFaults) / totalAccesses;
}