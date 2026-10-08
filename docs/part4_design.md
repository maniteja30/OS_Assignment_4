# Part IV: Finite Physical Memory

## 1. Objective

Part IV removes the assumption that the number of physical frames is greater than the number of virtual pages.

The simulator experiments with different physical memory sizes and uses an LRU (Least Recently Used) page replacement policy.

The assignment specifies:

- Physical memory sizes such as 0.5 times the virtual memory size, 0.25 times the virtual memory size, and so on.
- Disk access cost = 20,000,000 cycles.
- The first set of pages loaded into initially free frames is assumed to come for free.
- Disk access cost is counted only when a page is replaced.
- LRU replacement policy.
- Report page fault rate and total execution cycles.

---

## 2. Virtual Memory Configuration

The virtual address space is 32 bits.

Page size:

    4 KB = 2^12 bytes

Therefore, the number of virtual pages is:

    2^32 / 2^12 = 2^20 = 1,048,576 pages

Example physical memory configurations are therefore:

| Fraction of Virtual Memory | Number of Frames |
|----------------------------|------------------|
| 0.5                        | 524,288          |
| 0.25                       | 262,144          |
| 0.125                      | 131,072          |
| 0.0625                     | 65,536           |

---

## 3. Physical Memory Management

The `PhysicalMemory` class maintains:

- A mapping from virtual pages to physical frames.
- A mapping from physical frames to virtual pages.
- An LRU list.
- LRU positions for efficient updates.
- The number of page faults.
- The number of page replacements.

Initially, all physical frames are free.

When a page is accessed:

1. If the page is already present in physical memory, it is a page hit.
2. The page is moved to the most-recently-used position.
3. If the page is not present, a page fault occurs.
4. If a free frame is available, the page is placed in that frame.
5. If all frames are occupied, the least recently used page is removed.
6. The new page is placed in the freed frame.

The first pages loaded into free frames do not incur disk access cost.

---

## 4. LRU Replacement

The LRU list stores pages in usage order:

- Front: Most Recently Used (MRU)
- Back: Least Recently Used (LRU)

On every page hit, the page is moved to the front.

When a replacement is required, the page at the back of the list is selected for replacement.

This implements the required LRU replacement policy.

---

## 5. Cycle Calculation

Each memory access costs:

    200 cycles

A page replacement additionally requires a disk access:

    20,000,000 cycles

Therefore:

    Total Cycles =
        (Total Memory Accesses × 200)
        + (Page Replacements × 20,000,000)

No disk cost is added for the initial pages loaded into free frames.

---

## 6. Page Fault Rate

The page fault rate is calculated as:

    Page Fault Rate =
        (Number of Page Faults / Total Memory Accesses) × 100

A page fault occurs whenever the requested page is not currently present in physical memory.

---

## 7. Development Testing

A smaller development trace is used during implementation:

    traces/small/gcc_small.txt

The simulator accepts the trace file and number of physical frames through command-line arguments:

    ./memory_simulator_part4 <trace_file> <number_of_frames>

Development tests were performed using 32, 64, and 68 physical frames to verify free-frame allocation and LRU replacement behavior.

For the 10,000-access development trace:

| Frames | Page Faults | Replacements | Fault Rate | Total Cycles |
|-------:|------------:|-------------:|-----------:|-------------:|
| 32     | 85          | 53           | 0.85%      | 1,062,000,000 |
| 64     | 68          | 4            | 0.68%       | 82,000,000 |
| 68     | 68          | 0            | 0.68%       | 2,000,000 |

The results verify that:

- Initially free frames are used before replacement.
- LRU replacement occurs when all frames are occupied.
- Disk cost is added only for actual replacements.
- Page fault rate and total cycle calculations are consistent with the implementation.