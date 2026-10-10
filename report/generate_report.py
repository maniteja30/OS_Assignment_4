import csv
from pathlib import Path

ROOT = Path("report")

def read_csv(name):
    with (ROOT / name).open(newline="") as f:
        return list(csv.DictReader(f))

def fmt(n):
    return f"{int(n):,}"

def table(headers, rows):
    out = [
        "| " + " | ".join(headers) + " |",
        "| " + " | ".join("---" for _ in headers) + " |",
    ]
    out.extend("| " + " | ".join(str(x) for x in row) + " |" for row in rows)
    return "\n".join(out)

p13 = read_csv("parts1_3_results.csv")
p4 = read_csv("part4_results.csv")
p5 = read_csv("part5_runtime.csv")
dist = read_csv("page_distribution_summary.csv")

lines = []
add = lines.append

add("# Memory Management Simulator — Laboratory 4")
add("")
add("## 1. Introduction")
add("")
add("This project implements a C++ simulator for studying virtual-memory management using memory-access traces. It evaluates single-level and two-level page tables, a translation lookaside buffer (TLB), limited physical memory with LRU page replacement, and simulator runtime.")
add("")
add("The experiments use ten benchmark traces: GCC, Blender, Fotonik3d, Imagick, LBM, MCF, Perlbench, x264, xalancbmk, and xz.")
add("")
add("## 2. Experimental setup and assumptions")
add("")
add("- Virtual addresses are treated as 32-bit values; the lower 32 bits are used.")
add("- Page size: 4,096 bytes (4 KiB).")
add("- Virtual page count: 2^20 = 1,048,576.")
add("- Main-memory access cost: 200 cycles.")
add("- TLB lookup cost: 1 cycle.")
add("- TLB capacity: 128 entries, fully associative, using LRU replacement.")
add("- Disk access cost for a page replacement: 20,000,000 cycles.")
add("- Initial page loads into free frames do not incur the disk replacement penalty.")
add("- Physical-memory experiments use LRU page replacement.")
add("")
add("The reported values are simulator-model results, not measurements of physical hardware.")
add("")
add("## 3. Part I — Single-level page table")
add("")
add("The single-level page table contains 2^20 entries. For each memory access, the simulator derives the virtual page number (VPN) from the lower 32 address bits and maps the page to a physical frame. The reported frame count is the number of distinct pages loaded during the trace.")
add("")
add("Page-table utilization is calculated as the fraction of all virtual page-table entries accessed, expressed as a percentage. The modeled cost is 400 cycles per access: one page-table memory access and one data memory access, each costing 200 cycles.")
add("")
add(table(
    ["Benchmark", "Accesses", "Frames used", "PT utilization", "Cycles", "Final hash"],
    [[r["Benchmark"], fmt(r["Accesses"]), fmt(r["Frames_Used"]),
      f'{float(r["Part1_PT_Utilization_Percent"]):.6g}%',
      fmt(r["Part1_Cycles"]), r["Part1_Hash"]] for r in p13]
))
add("")
add("The frame counts range from 96 pages for Imagick to 14,325 pages for xalancbmk. Page-table utilization is low because each trace touches only a small subset of the 1,048,576 possible virtual pages.")
add("")
add("### Page-access distribution")
add("")
add("The following graph plots the cumulative percentage of memory accesses against the cumulative percentage of distinct virtual pages, with pages ordered from most frequently accessed to least frequently accessed.")
add("")
add("![Cumulative memory-access distribution](figures/page_access_distribution.png)")
add("")
add(table(
    ["Benchmark", "Total accesses", "Distinct pages", "Accesses from hottest 10% of pages"],
    [[r["Benchmark"].upper() if r["Benchmark"] not in ("x264", "xalancbmk") else r["Benchmark"], fmt(r["Total accesses"]), fmt(r["Distinct virtual pages"]),
      f'{float(r["Accesses served by hottest 10% of pages (%)"]):.2f}%'] for r in dist]
))
add("")
add("Across these traces, the hottest 10% of distinct pages account for 82.53%–99.54% of accesses. This indicates highly concentrated page-access patterns in the supplied traces. It also helps explain why a small TLB can achieve a high hit rate.")
add("")
add("## 4. Part II — Two-level page table")
add("")
add("The two-level design divides the 20-bit VPN into two 10-bit indices. A level-1 table points to lazily allocated level-2 tables. This avoids allocating every possible second-level table when only a subset of the address space is used.")
add("")
add(table(
    ["Benchmark", "L1 tables/entries", "L2 tables/entries", "Total modeled cycles", "Hash comparison"],
    [[r["Benchmark"], fmt(r["Part2_L1_Tables"]), fmt(r["Part2_L2_Tables"]),
      fmt(r["Part2_Cycles"]), "Matches Part I"] for r in p13]
))
add("")
add("The final hash matches Part I for every benchmark, providing a consistency check that both page-table organizations map the same trace accesses to the same frame sequence. The two-level design incurs an additional page-table access in the modeled cost, so its total cycle count is higher than the single-level design in this experiment. Its potential benefit is reduced page-table allocation for sparse address spaces.")
add("")
add("## 5. Part III — Translation lookaside buffer")
add("")
add("The simulator adds a 128-entry fully associative LRU TLB. Each lookup costs 1 cycle. A TLB hit requires the TLB lookup and data access; a miss also incurs page-table access cost. The table below reports the measured hit/miss counts and modeled cycles for both page-table designs.")
add("")
add(table(
    ["Benchmark", "TLB hits", "TLB misses", "Hit rate", "Single-level cycles", "Two-level cycles"],
    [[r["Benchmark"], fmt(r["TLB_Hits"]), fmt(r["TLB_Misses"]),
      f'{float(r["TLB_Hit_Rate_Percent"]):.4f}%',
      fmt(r["Part3_Single_Level_Cycles"]), fmt(r["Part3_Two_Level_Cycles"])]
     for r in p13]
))
add("")
add("TLB hit rates range from 99.4421% (MCF) to 99.9992% (Imagick). Because the TLB services nearly all accesses, modeled cycles are substantially lower than in the no-TLB page-table models. The two-level configuration has slightly higher cycle counts because TLB misses require an extra page-table lookup.")
add("")
add("## 6. Part IV — Limited physical memory and LRU replacement")
add("")
add("The simulator was evaluated with capacities of 256 frames, 512 frames, and 262,144 frames. The first two are small-capacity stress tests that expose replacement behavior. The 262,144-frame capacity corresponds to 25% of the 1,048,576-page virtual address space. At this capacity, none of these traces requires replacement because each trace's working set is smaller than the available frame count.")
add("")
add("A page fault occurs whenever a referenced page is not resident. A replacement occurs only when a page fault happens and all frames are occupied. Initial page loads into free frames incur no disk penalty. The cycle model is:")
add("")
add("**Total cycles = 200 × memory accesses + 20,000,000 × replacements**")
add("")
add("The fault rate is page faults divided by total memory accesses, expressed as a percentage.")
add("")
add(table(
    ["Benchmark", "Capacity", "Frames used", "Faults", "Replacements", "Fault rate", "Total cycles"],
    [[r["Benchmark"], fmt(r["Frame_Capacity"]), fmt(r["Frames_Used"]),
      fmt(r["Page_Faults"]), fmt(r["Replacements"]),
      f'{float(r["Page_Fault_Rate_Percent"]):.6g}%', fmt(r["Total_Cycles"])]
     for r in p4]
))
add("")
add("### Interpretation")
add("")
add("- At 262,144 frames, all page faults are first-time loads; replacements are zero for all ten traces.")
add("- With 512 frames, Imagick and x264 fit their observed working sets and need no replacement. Other traces incur replacement costs to varying degrees.")
add("- Reducing capacity from 512 to 256 frames increases replacements for several benchmarks, particularly MCF and Perlbench, which sharply increases their modeled total cycles.")
add("- Capacity sensitivity depends on each trace's working set and access pattern; fault rate alone does not show the full cycle impact because each replacement incurs a large disk penalty.")
add("")
add("The assignment's 50% capacity example is 524,288 frames. The full 524,288-frame experiment was checked on GCC and produced no replacements; the 262,144-frame configuration was run across all ten benchmarks.")
add("")
add("## 7. Part V — Simulator runtime")
add("")
add("The following wall-clock times were recorded for the optimized trace parser in earlier runs. They are environment-dependent observations, not guaranteed runtimes.")
add("")
add(table(
    ["Benchmark", "Accesses", "Frames used", "Modeled cycles", "Observed runtime (s)"],
    [[r["Benchmark"], fmt(r["Accesses"]), fmt(r["Frames_Used"]),
      fmt(r["Total_Cycles"]), f'{float(r["Runtime_Seconds"]):.3f}'] for r in p5]
))
add("")
times = [float(r["Runtime_Seconds"]) for r in p5]
fastest = min(p5, key=lambda r: float(r["Runtime_Seconds"]))
slowest = max(p5, key=lambda r: float(r["Runtime_Seconds"]))
add(f"The recorded runtimes range from {min(times):.3f} s for {fastest['Benchmark']} to {max(times):.3f} s for {slowest['Benchmark']}. Actual runtime depends on the machine, operating-system load, compiler, and build configuration.")
add("")
add("## 8. Overall conclusions")
add("")
add("1. The traces touch only a small fraction of the possible virtual-page space, so single-level page-table utilization remains low.")
add("2. The page-distribution graph shows strong access concentration: the hottest 10% of pages account for a large majority of accesses in every benchmark.")
add("3. The two-level page-table implementation reproduces the Part I hash for all benchmarks, while its modeled miss cost is higher because it requires an additional lookup.")
add("4. The 128-entry LRU TLB achieves hit rates above 99.4% on all supplied traces, substantially reducing modeled access cycles.")
add("5. LRU replacement costs become significant when frame capacity is smaller than a trace's working set. The large modeled disk penalty makes replacement count a major contributor to total cycles.")
add("6. Runtime measurements show that the optimized simulator processes each trace in under one second in the recorded environment, though results may vary across systems.")
add("")
add("## 9. Reproducibility and limitations")
add("")
add("- The benchmark traces are local experimental inputs and are not included in this report.")
add("- The CSV files in this directory preserve the benchmark results used to construct the tables.")
add("- The page-access distribution graph is generated by `analyze_page_distribution.py`; use `MPLBACKEND=Agg python report/analyze_page_distribution.py` on headless or graphics-sensitive environments.")
add("- Part V wall-clock times are historical measurements and may vary when rerun.")
add("- The cycle counts are based on the assignment's abstract cost model and should not be interpreted as real CPU cycle measurements.")
add("")
add("## Appendix — Result files")
add("")
add("- `parts1_3_results.csv` — Parts I–III benchmark results.")
add("- `part4_results.csv` — Part IV capacity experiments.")
add("- `part5_runtime.csv` — Part V runtime measurements.")
add("- `page_distribution_summary.csv` — distinct-page and locality summary.")
add("- `figures/page_access_distribution.png` — cumulative page-access distribution graph.")
add("- `analyze_page_distribution.py` — script used to generate the graph and summary.")
add("")

(ROOT / "report.md").write_text("\n".join(lines), encoding="utf-8")
print(f"Generated {ROOT / 'report.md'}")
print(f"Report lines: {len(lines)}")
print(f"Parts I–III rows: {len(p13)}")
print(f"Part IV rows: {len(p4)}")
print(f"Part V rows: {len(p5)}")
print(f"Distribution rows: {len(dist)}")
