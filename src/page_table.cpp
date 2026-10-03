#include "page_table.h"

PageTable::PageTable() : nextFreeFrame(0){}

uint32_t PageTable::getFrame(uint32_t virtualPage){

    pageAccessCount[virtualPage]++;

    auto it = pageToFrame.find(virtualPage);

    if(it != pageToFrame.end()){
        return it->second;
    }

    uint32_t frame = nextFreeFrame;
    nextFreeFrame++;

    pageToFrame[virtualPage] = frame;

    return frame;
}

uint32_t PageTable::getUsedFrameCount() const{
    return static_cast<uint32_t>(pageToFrame.size());
}

uint64_t PageTable::getPageAccessCount(uint32_t virtualPage) const{
    auto it = pageAccessCount.find(virtualPage);

    if(it != pageAccessCount.end()){
        return it->second;
    }

    return 0;
}

std::vector<std::pair<uint32_t, uint64_t>>
PageTable::getPageAccessCounts() const {

    std::vector<std::pair<uint32_t, uint64_t>> result;

    for (const auto& entry : pageAccessCount) {
        result.push_back(entry);
    }

    return result;
}