#include "tlb.h"

TLB::TLB()
    : hits(0), misses(0) {
}

bool TLB::lookup(uint32_t virtualPage, uint32_t& frame) {

    auto it = pageToFrame.find(virtualPage);

    if (it == pageToFrame.end()) {

        misses++;

        return false;
    }

    // TLB hit
    hits++;

    frame = it->second;

    // This page was recently used,
    // so move it to the front.
    makeMostRecentlyUsed(virtualPage);

    return true;
}

void TLB::insert(uint32_t virtualPage, uint32_t frame) {

    // If the page is already present,
    // update its frame and make it MRU.
    auto it = pageToFrame.find(virtualPage);

    if (it != pageToFrame.end()) {

        it->second = frame;

        makeMostRecentlyUsed(virtualPage);

        return;
    }

    // If TLB is full, remove the LRU entry.
    if (pageToFrame.size() >= TLB_SIZE) {

        uint32_t lruPage = lruList.back();

        lruList.pop_back();

        lruPosition.erase(lruPage);

        pageToFrame.erase(lruPage);
    }

    // Insert the new page.
    pageToFrame[virtualPage] = frame;

    lruList.push_front(virtualPage);

    lruPosition[virtualPage] = lruList.begin();
}

void TLB::makeMostRecentlyUsed(uint32_t virtualPage) {

    auto it = lruPosition.find(virtualPage);

    if (it == lruPosition.end()) {
        return;
    }

    // Remove from current position.
    lruList.erase(it->second);

    // Put at the front.
    lruList.push_front(virtualPage);

    // Update its iterator.
    lruPosition[virtualPage] = lruList.begin();
}

uint64_t TLB::getHits() const {
    return hits;
}

uint64_t TLB::getMisses() const {
    return misses;
}

uint64_t TLB::getAccesses() const {
    return hits + misses;
}

double TLB::getHitRate() const {

    uint64_t accesses = getAccesses();

    if (accesses == 0) {
        return 0.0;
    }

    return (100.0 * hits) / accesses;
}