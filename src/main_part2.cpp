#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "trace.h"
#include "two_level_page_table.h"
#include "config.h"

using namespace std;

int main(int argc, char* argv[]) {

    string tracePath = (argc > 1)
        ? argv[1]
        : "traces/small/gcc_small.txt";

    ifstream file(tracePath);

    if (!file.is_open()) {
        cerr << "Error: could not open the trace file" << endl;
        return 1;
    }

    TwoLevelPageTable pageTable;

    string line;

    // Skip the header.
    getline(file, line);

    uint64_t totalAccesses = 0;

    uint64_t totalCycles = 0;

    uint64_t hash = 0;

    while (getline(file, line)) {

        MemoryAccess access;

        if (!parseTraceLine(line, access)) {
            cerr << "Error parsing line: " << line << endl;
            continue;
        }

        // 4 KB page size.
        uint32_t virtualPage =
            access.address / PAGE_SIZE;

        // Get physical frame for this virtual page.
        uint32_t frame =
            pageTable.getFrame(virtualPage);

        /*
         * Two-level page-table walk:
         *
         * Level-1 page-table access = 200 cycles
         * Level-2 page-table access = 200 cycles
         * Actual memory access       = 200 cycles
         *
         * Total = 600 cycles/access
         */
        totalCycles +=
            MEMORY_ACCESS_CYCLES * 3;

        // Same hash function used in Part I.
        hash = hash * 31 + frame;

        totalAccesses++;
    }

    file.close();

    cout << "===== Part II: Two-Level Page Table =====\n\n";

    cout << "Total Accesses: "
         << totalAccesses << endl;

    cout << "Frames used: "
         << pageTable.getUsedFrameCount() << endl;

    cout << "Level-1 Entries Allocated: "
         << pageTable.getLevel1EntriesAllocated()
         << endl;

    cout << "Level-2 Entries Allocated: "
         << pageTable.getLevel2EntriesAllocated()
         << endl;

    cout << "Total Page Table Entries Allocated: "
         << pageTable.getTotalEntriesAllocated()
         << endl;

    cout << "Total Page Table Entries Accessed: "
         << pageTable.getTotalEntriesAccessed()
         << endl;

    double pageTableUtilization =
        (100.0 * pageTable.getTotalEntriesAccessed())
        / pageTable.getTotalEntriesAllocated();

    cout << "Page Table Utilization: "
         << pageTableUtilization
         << "%" << endl;

    cout << "Total Cycles: "
         << totalCycles << endl;

    cout << "Final Hash: "
         << hash << endl;


    /*
     * Part II-B:
     * Page-access distribution
     */

    vector<pair<uint32_t, uint64_t>> pageCounts =
        pageTable.getPageAccessCounts();

    sort(
        pageCounts.begin(),
        pageCounts.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second;
        }
    );

    uint64_t top10Accesses = 0;
    uint64_t top20Accesses = 0;
    uint64_t top50Accesses = 0;

    size_t totalPages = pageCounts.size();

    size_t top10Pages =
        (totalPages * 10 + 99) / 100;

    size_t top20Pages =
        (totalPages * 20 + 99) / 100;

    size_t top50Pages =
        (totalPages * 50 + 99) / 100;

    for (size_t i = 0; i < totalPages; i++) {

        if (i < top10Pages) {
            top10Accesses += pageCounts[i].second;
        }

        if (i < top20Pages) {
            top20Accesses += pageCounts[i].second;
        }

        if (i < top50Pages) {
            top50Accesses += pageCounts[i].second;
        }
    }

    cout << "\nPage-access distribution:\n";

    cout << "Top 10% of pages: "
         << top10Accesses
         << " accesses ("
         << (100.0 * top10Accesses / totalAccesses)
         << "%)\n";

    cout << "Top 20% of pages: "
         << top20Accesses
         << " accesses ("
         << (100.0 * top20Accesses / totalAccesses)
         << "%)\n";

    cout << "Top 50% of pages: "
         << top50Accesses
         << " accesses ("
         << (100.0 * top50Accesses / totalAccesses)
         << "%)\n";


    cout << "\nTop 10 most accessed pages:\n";

    int limit =
        min(10, static_cast<int>(pageCounts.size()));

    for (int i = 0; i < limit; i++) {

        cout << "Page: "
             << pageCounts[i].first
             << ", Accesses: "
             << pageCounts[i].second
             << endl;
    }

    return 0;
}