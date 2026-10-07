# Part II Design - Two-Level Page Table

## 1. Address Decomposition

The virtual address space is 32 bits and the page size is 4 KB.

Since:

2^32 / 2^12 = 2^20

there are 20 virtual-page-number bits.

The 20-bit virtual page number is divided into two 10-bit indices:

- Level-1 index: 10 bits
- Level-2 index: 10 bits

The remaining 12 bits of the virtual address are the page offset.

Therefore:

32-bit virtual address
= 10-bit Level-1 index
+ 10-bit Level-2 index
+ 12-bit page offset

## 2. Two-Level Page Table

The Level-1 table contains up to 1024 entries.

Each Level-1 entry refers to a Level-2 page table.

Each Level-2 table contains up to 1024 entries.

A Level-2 table is allocated only when a virtual page belonging
to that Level-1 region is encountered.

This avoids allocating all 1024 Level-2 tables in advance.

## 3. Frame Allocation

The same sequential first-free-frame allocation scheme from Part I
is used.

The first newly encountered virtual page receives frame 0, the next
new virtual page receives frame 1, and so on.

An already mapped virtual page keeps its existing frame.

Using the same allocation policy ensures that the virtual-page-to-
physical-frame mapping is the same as in Part I.

## 4. Cycle Calculation

Without a TLB, every memory access requires:

- Level-1 page-table access = 200 cycles
- Level-2 page-table access = 200 cycles
- Actual memory access = 200 cycles

Therefore:

Cycles per memory access = 600 cycles

## 5. Page-Table Utilization

For the two-level page table, allocated entries include:

- Allocated Level-1 entries
- Allocated Level-2 entries

For each distinct virtual page encountered:

- One Level-1 entry is involved
- One Level-2 entry is allocated

The simulator reports the number of page-table entries allocated
and the number of page-table entries accessed during the simulation.

## 6. Hash

The same hash function from Part I is used:

H_new = H_old × 31 + frame_number

The initial value is:

H_0 = 0

The purpose of using the same hash function and the same frame
allocation policy is to verify whether the two-level page table
produces the same virtual-page-to-physical-frame mapping as the
single-level page table.

## 7. Expected Hash Comparison

If both implementations assign the same physical frame to every
virtual page, the hash computed over the same trace should match.

Therefore the Part II hash is compared directly with the Part I hash.