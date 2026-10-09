#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "trace.h"
#include "page_table.h"
#include "config.h"
using namespace std;
int main(int argc, char* argv[]){
    string tracePath = (argc > 1) ? argv[1] : "traces/small/gcc_small.txt";

    ifstream file(tracePath);
    if(!file.is_open()){
        cerr << "Error: couldnot open the trace file" << endl;
        return 1;
    }

    PageTable pageTable;

    string line;
    getline(file, line);

    uint64_t totalAccesses = 0;
    uint64_t totalCycles = 0;

    uint64_t hash = 0;

    while(getline(file, line)){
        MemoryAccess access;
        if(!parseTraceLine(line, access)){
            cerr << "Error parsing line: " << line << endl;
            continue;
        }

        uint32_t virtualPage = access.address/PAGE_SIZE;

        uint32_t frame =  pageTable.getFrame(virtualPage);

        totalCycles += MEMORY_ACCESS_CYCLES * 2;
        hash = hash * 31 + frame;

        totalAccesses++;

        // cout << "Address: " << access.address << ", Page: " << virtualPage << ", Frame: " << frame << endl;
    }
    file.close();

    cout << "Total Accesses: " << totalAccesses << endl;
    cout << "Frames used: " << pageTable.getUsedFrameCount() << endl;
    uint64_t entriesAccessed = pageTable.getUsedFrameCount();

    double pageTableUtilization = (100.0 * entriesAccessed) / PAGE_TABLE_ENTRIES;

    cout << "Page Table Utilization: " << pageTableUtilization << "%" << endl;
    cout << "Total Cycles: " << totalCycles << endl;
    cout << "Final Hash: " << hash << endl;
    vector<pair<uint32_t, uint64_t>> pageCounts = pageTable.getPageAccessCounts();
    sort(pageCounts.begin(), pageCounts.end(), [](const auto& a, const auto& b) {return a.second > b.second;});

    uint64_t top10Accesses = 0;
    uint64_t top20Accesses = 0;
    uint64_t top50Accesses = 0;

    size_t totalPages = pageCounts.size();

    size_t top10Pages = (totalPages * 10 + 99) / 100;
    size_t top20Pages = (totalPages * 20 + 99) / 100;
    size_t top50Pages = (totalPages * 50 + 99) / 100;

    for(size_t i = 0; i < totalPages; i++){

        if(i < top10Pages){
            top10Accesses += pageCounts[i].second;
        }

        if(i < top20Pages){
            top20Accesses += pageCounts[i].second;
        }

        if(i < top50Pages){
            top50Accesses += pageCounts[i].second;
        }
    }

    cout << "\nPage-access distribution:\n";

    cout << "Top 10% of pages: "
        << top10Accesses << " accesses ("
        << (100.0 * top10Accesses / totalAccesses)
        << "%)\n";

    cout << "Top 20% of pages: "
        << top20Accesses << " accesses ("
        << (100.0 * top20Accesses / totalAccesses)
        << "%)\n";

    cout << "Top 50% of pages: "
        << top50Accesses << " accesses ("
        << (100.0 * top50Accesses / totalAccesses)
        << "%)\n";

    cout << "\nTop 10 most accessed pages:\n";

    int limit = min(10, static_cast<int>(pageCounts.size()));

    for(int i = 0; i < limit; i++){
        cout << "Page: " << pageCounts[i].first << ", Accesses: " << pageCounts[i].second << endl;
    }

    return 0;
}