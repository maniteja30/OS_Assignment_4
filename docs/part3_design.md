# Part III Design - TLB

## 1. TLB Configuration

A Translation Lookaside Buffer (TLB) is added to reduce the cost of
repeated page-table lookups.

The assignment specifies:

- TLB entries: 128
- Organization: Fully associative
- Replacement policy: LRU
- TLB lookup cost: 1 cycle

The TLB starts empty.

## 2. TLB Entry

Each TLB entry stores a mapping:

Virtual Page Number -> Physical Frame Number

When a virtual page is found in the TLB, the physical frame can be
obtained without accessing the page table.

## 3. LRU Replacement

The TLB uses Least Recently Used (LRU) replacement.

The most recently used entry is maintained at the front of the LRU
list, while the least recently used entry is maintained at the back.

When a TLB hit occurs, the corresponding entry is moved to the front.

When a new entry is inserted into a full TLB, the entry at the back
is removed.

## 4. Single-Level Page Table + TLB

For a TLB hit:

- TLB lookup = 1 cycle
- Memory access = 200 cycles

Total:

1 + 200 = 201 cycles

For a TLB miss:

- TLB lookup = 1 cycle
- Single-level page-table access = 200 cycles
- Memory access = 200 cycles

Total:

1 + 200 + 200 = 401 cycles

## 5. Two-Level Page Table + TLB

For a TLB hit:

- TLB lookup = 1 cycle
- Memory access = 200 cycles

Total:

1 + 200 = 201 cycles

For a TLB miss:

- TLB lookup = 1 cycle
- Level-1 page-table access = 200 cycles
- Level-2 page-table access = 200 cycles
- Memory access = 200 cycles

Total:

1 + 200 + 200 + 200 = 601 cycles

## 6. TLB Hit Rate

The TLB hit rate is calculated as:

TLB Hit Rate = TLB Hits / Total TLB Accesses × 100

The simulator reports:

- Total TLB accesses
- TLB hits
- TLB misses
- TLB hit rate
- Total cycles

## 7. Development Trace Results

For the development trace containing 10,000 memory accesses:

### Single-Level Page Table + TLB

- Total accesses: 10,000
- TLB hits: 9,932
- TLB misses: 68
- TLB hit rate: 99.32%
- Total cycles: 2,023,600

### Two-Level Page Table + TLB

- Total accesses: 10,000
- TLB hits: 9,932
- TLB misses: 68
- TLB hit rate: 99.32%
- Total cycles: 2,037,200

The two implementations have the same TLB hit rate because the TLB
operates independently of the number of levels in the page table.

The two-level page table requires an additional 200-cycle page-table
access on each TLB miss.