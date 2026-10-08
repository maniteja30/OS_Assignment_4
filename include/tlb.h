#ifndef TLB_H
#define TLB_H

#include <cstdint>
#include <list>
#include <unordered_map>

using namespace std;

class TLB {
private:

    static constexpr uint32_t TLB_SIZE = 128;

    // virtual page -> physical frame
    unordered_map<uint32_t, uint32_t> pageToFrame;

    // Most recently used page is at the front.
    // Least recently used page is at the back.
    list<uint32_t> lruList;

    // Stores the position of each page in the LRU list.
    unordered_map<uint32_t, list<uint32_t>::iterator> lruPosition;

public:

    TLB();

    // Returns true if the page is present in the TLB.
    // The corresponding frame is returned through frame.
    bool lookup(uint32_t virtualPage, uint32_t& frame);

    // Insert a new virtual-page -> physical-frame mapping.
    void insert(uint32_t virtualPage, uint32_t frame);

    // Statistics
    uint64_t getHits() const;
    uint64_t getMisses() const;
    uint64_t getAccesses() const;
    double getHitRate() const;

private:

    uint64_t hits;
    uint64_t misses;

    // Move an existing page to the MRU position.
    void makeMostRecentlyUsed(uint32_t virtualPage);
};

#endif