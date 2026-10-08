#include <iostream>
#include <fstream>
#include <string>

#include "trace.h"
#include "physical_memory.h"
#include "config.h"

using namespace std;

int main(int argc, char* argv[]) {

    if (argc != 3) {
        cerr << "Usage: ./memory_simulator_part4 <trace_file> <number_of_frames>"
             << endl;
        return 1;
    }

    string traceFile = argv[1];
    uint32_t numberOfFrames = stoul(argv[2]);

    ifstream file(traceFile);

    if (!file.is_open()) {
        cerr << "Error: could not open the trace file" << endl;
        return 1;
    }

    PhysicalMemory physicalMemory(numberOfFrames);

    string line;

    // Skip header.
    getline(file, line);

    uint64_t totalAccesses = 0;
    uint64_t totalCycles = 0;

    while (getline(file, line)) {

        MemoryAccess access;

        if (!parseTraceLine(line, access)) {
            cerr << "Error parsing line: "
                 << line << endl;
            continue;
        }

        uint32_t virtualPage =
            access.address / PAGE_SIZE;

        uint32_t frame;

        bool replacementOccurred = false;

        bool pageHit = physicalMemory.accessPage(virtualPage,frame,replacementOccurred);

        // Every memory request takes 200 cycles.
        totalCycles += MEMORY_ACCESS_CYCLES;

        // Disk cost is added only when this access actually
        // caused a page replacement.
        if (replacementOccurred) {
            totalCycles += DISK_ACCESS_CYCLES;
        }

        totalAccesses++;
    }

    file.close();

    cout << "===== Part IV: Finite Physical Memory =====\n\n";

    cout << "Total Accesses: "
         << totalAccesses << endl;

    cout << "Physical Frames: "
         << physicalMemory.getTotalFrames()
         << endl;

    cout << "Frames Used: "
         << physicalMemory.getUsedFrames()
         << endl;

    cout << "Page Faults: "
         << physicalMemory.getPageFaults()
         << endl;

    cout << "Page Replacements: "
         << physicalMemory.getReplacements()
         << endl;

    cout << "Page Fault Rate: "
         << physicalMemory.getPageFaultRate(totalAccesses)
         << "%" << endl;

    cout << "Total Cycles: "
         << totalCycles << endl;

    return 0;
}