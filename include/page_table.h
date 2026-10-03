#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H
#include <vector>
#include<cstdint>
#include<unordered_map>

using namespace std;

class PageTable{
    private:
        unordered_map<uint32_t, uint32_t> pageToFrame;

        // Stores the number of accesses to each virtual page
        unordered_map<uint32_t, uint64_t> pageAccessCount;

        uint32_t nextFreeFrame;

    public:
        PageTable();

        uint32_t getFrame(uint32_t virtualPage);

        uint32_t getUsedFrameCount() const;

        uint64_t getPageAccessCount(uint32_t virtualPage) const;

        vector<pair<uint32_t, uint64_t>> getPageAccessCounts() const;
};

#endif