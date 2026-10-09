#include <iostream>
#include <fstream>
#include <string>

#include "trace.h"
#include "page_table.h"
#include "tlb.h"
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

    PageTable pageTable;
    TLB tlb;

    string line;

    // Skip header
    getline(file, line);

    uint64_t totalAccesses = 0;
    uint64_t totalCycles = 0;

    while (getline(file, line)) {

        MemoryAccess access;

        if (!parseTraceLine(line, access)) {
            cerr << "Error parsing line: " << line << endl;
            continue;
        }

        uint32_t virtualPage =
            access.address / PAGE_SIZE;

        uint32_t frame;

        // First check the TLB.
        if (tlb.lookup(virtualPage, frame)) {

            // TLB hit:
            // TLB lookup = 1 cycle
            // Actual memory access = 200 cycles
            totalCycles += TLB_ACCESS_CYCLES
                         + MEMORY_ACCESS_CYCLES;

        }
        else {

            // TLB miss.
            //
            // Walk the single-level page table.
            frame = pageTable.getFrame(virtualPage);

            // Add the new mapping to the TLB.
            tlb.insert(virtualPage, frame);

            // TLB lookup = 1 cycle
            // Page table access = 200 cycles
            // Actual memory access = 200 cycles
            totalCycles += TLB_ACCESS_CYCLES
                         + MEMORY_ACCESS_CYCLES
                         + MEMORY_ACCESS_CYCLES;
        }

        totalAccesses++;
    }

    file.close();

    cout << "===== Part III-A: Single-Level Page Table + TLB =====\n\n";

    cout << "Total Accesses: "
         << totalAccesses << endl;

    cout << "Frames used: "
         << pageTable.getUsedFrameCount() << endl;

    cout << "TLB Accesses: "
         << tlb.getAccesses() << endl;

    cout << "TLB Hits: "
         << tlb.getHits() << endl;

    cout << "TLB Misses: "
         << tlb.getMisses() << endl;

    cout << "TLB Hit Rate: "
         << tlb.getHitRate()
         << "%" << endl;

    cout << "Total Cycles: "
         << totalCycles << endl;

    return 0;
}