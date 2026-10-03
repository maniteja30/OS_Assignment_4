# Part I Design

## 1. Virtual Address and Page Size

The virtual address space is 32 bits, giving a total virtual address
space of 2^32 bytes.

The page size is 4 KB = 2^12 bytes.

Therefore, the virtual address is divided into:

- 20-bit Virtual Page Number (VPN)
- 12-bit Page Offset

The virtual page number is calculated as:

VPN = address / 4096

## 2. Physical Frame Allocation Scheme

For Part I, the number of physical frames is assumed to be greater
than the number of virtual pages, so page replacement is not required.

A sequential first-free-frame allocation scheme is used.

Initially:

nextFreeFrame = 0

When a virtual page is accessed for the first time:

1. Assign the current value of nextFreeFrame to the page.
2. Increment nextFreeFrame.
3. Store the page-to-frame mapping.

If the virtual page has already been mapped, its existing frame is
returned.

For example:

Virtual Page 1476 -> Frame 0
Virtual Page 2000 -> Frame 1
Virtual Page 1476 -> Frame 0
Virtual Page 3000 -> Frame 2

Thus, a virtual page always keeps the same physical frame during
the simulation.

## 3. Cycle Calculation

The memory access time is 200 cycles.

For Part I, each memory reference requires:

- One memory access to obtain the page-table entry: 200 cycles
- One memory access to access the requested data: 200 cycles

Therefore:

Cycles per access = 200 + 200 = 400 cycles

For N memory accesses:

Total cycles = N × 400

## 4. Page-Table Utilization

A 32-bit virtual address with a 4 KB page size requires:

2^(32-12) = 2^20

page-table entries.

Page-table utilization is calculated as:

Utilization =
(Number of page-table entries accessed /
 Number of page-table entries allocated) × 100

Only distinct virtual pages that occur in the trace count as
accessed page-table entries.

## 5. Hash Function

For every memory access, the frame number is incorporated into a
running hash.

The selected hash function is:

H_new = H_old × 31 + frame_number

The hash is stored as a 64-bit unsigned integer, so arithmetic
wraps around modulo 2^64.

The initial hash value is:

H_0 = 0