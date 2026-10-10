import csv
import glob
import os
from collections import Counter

import matplotlib.pyplot as plt


TRACE_DIR = "lab4_benchmarks"
FIGURE_PATH = "report/figures/page_access_distribution.png"
CSV_PATH = "report/page_distribution_summary.csv"
PAGE_SIZE = 4096


def parse_address(value):
    value = value.strip()
    try:
        return int(value, 0)
    except ValueError:
        return int(value, 10)


def analyze_trace(path):
    frequencies = Counter()
    total_accesses = 0

    with open(path, "r", newline="") as file:
        reader = csv.reader(file)
        next(reader, None)  # Skip the header.

        for row in reader:
            if not row or len(row) < 1:
                continue

            address = parse_address(row[0])
            vpn = (address & 0xFFFFFFFF) // PAGE_SIZE
            frequencies[vpn] += 1
            total_accesses += 1

    sorted_counts = sorted(frequencies.values(), reverse=True)
    distinct_pages = len(sorted_counts)

    cumulative_accesses = []
    running_total = 0

    for count in sorted_counts:
        running_total += count
        cumulative_accesses.append(
            100.0 * running_total / total_accesses
        )

    cumulative_page_percentages = [
        100.0 * i / distinct_pages
        for i in range(1, distinct_pages + 1)
    ]

    top_10_percent_pages = max(1, (distinct_pages + 9) // 10)
    top_10_accesses = sum(sorted_counts[:top_10_percent_pages])
    top_10_access_percent = 100.0 * top_10_accesses / total_accesses

    return {
        "name": os.path.basename(path).replace("_memoryTrace.txt", ""),
        "accesses": total_accesses,
        "distinct_pages": distinct_pages,
        "top_10_percent_pages_access_percent": top_10_access_percent,
        "x": cumulative_page_percentages,
        "y": cumulative_accesses,
    }


trace_paths = sorted(glob.glob(os.path.join(TRACE_DIR, "*_memoryTrace.txt")))

if not trace_paths:
    raise SystemExit(
        f"No benchmark traces found in {TRACE_DIR!r}. "
        "Check the directory and filenames."
    )

results = [analyze_trace(path) for path in trace_paths]

plt.figure(figsize=(10, 7))

for result in results:
    plt.plot(
        result["x"],
        result["y"],
        label=result["name"],
        linewidth=1.8,
    )

plt.plot([0, 100], [0, 100], linestyle="--", label="Uniform access reference")
plt.xlabel("Cumulative percentage of distinct virtual pages")
plt.ylabel("Cumulative percentage of memory accesses")
plt.title("Cumulative Memory Access Distribution Across Benchmarks")
plt.xlim(0, 100)
plt.ylim(0, 100)
plt.grid(True, alpha=0.3)
plt.legend(fontsize=8)
plt.tight_layout()
plt.savefig(FIGURE_PATH, dpi=200)
plt.close()

with open(CSV_PATH, "w", newline="") as file:
    writer = csv.writer(file)
    writer.writerow([
        "Benchmark",
        "Total accesses",
        "Distinct virtual pages",
        "Accesses served by hottest 10% of pages (%)",
    ])

    for result in results:
        writer.writerow([
            result["name"],
            result["accesses"],
            result["distinct_pages"],
            f'{result["top_10_percent_pages_access_percent"]:.6f}',
        ])

print(f"Benchmarks analyzed: {len(results)}")
for result in results:
    print(
        f'{result["name"]}: '
        f'{result["accesses"]:,} accesses, '
        f'{result["distinct_pages"]:,} distinct pages, '
        f'top 10% of pages account for '
        f'{result["top_10_percent_pages_access_percent"]:.2f}% of accesses'
    )

print(f"\nGraph saved to: {FIGURE_PATH}")
print(f"Summary saved to: {CSV_PATH}")
