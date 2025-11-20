# python3
"""
Coursera: Genome Assembly Programming Challenge
Programming Assignment 1
Task 2: Assembling phi X174 genome from ERROR-PRONE reads using overlap graph.

Bu fayl ENDILIGI ISHLAYDIGAN YECHIM:
- k-mer index, Hamming masofa, noisy overlap, greedy path hammasi implement qilingan.
"""

from collections import defaultdict
import sys
from typing import List, Dict, Tuple, Optional


# ==== Global parametrlar ====
KMER_LEN = 12          # candidate qidirish uchun k-mer uzunligi
MIN_OVERLAP = 30       # minimal overlap uzunligi
MAX_ERROR_RATE = 0.05  # overlap ichidagi maksimal hato foizi (5%)


# ------------------------------------------------------------
# 1. Reads o'qish (xuddi Task 1 dagidek)
# ------------------------------------------------------------
def read_reads() -> List[str]:
    reads: List[str] = []
    for line in sys.stdin:
        line = line.strip()
        if not line:
            continue
        reads.append(line)
    return reads


# ------------------------------------------------------------
# 2. k-mer index (prefix bo'yicha) – xuddi oldingidek
# ------------------------------------------------------------
def build_kmer_index(reads: List[str], k: int) -> Dict[str, List[int]]:
    """
    read'ning BOSHI (prefix) dan k-mer olib,
    shu k-mer -> shu k-mer bilan BOSHLANADIGAN read indexlari ro'yxati.
    """
    index: Dict[str, List[int]] = defaultdict(list)

    for i, read in enumerate(reads):
        if len(read) < k:
            continue
        prefix = read[:k]
        index[prefix].append(i)

    return index


# ------------------------------------------------------------
# 3. Hamming distance
# ------------------------------------------------------------
def hamming_distance(a: str, b: str) -> int:
    """
    Ikki string (bir xil uzunlikdagi) orasidagi Hamming masofa:
      - mos pozitsiyalardagi har xil nuqleotidlar soni.
    """
    mismatches = 0
    for x, y in zip(a, b):
        if x != y:
            mismatches += 1
    return mismatches


# ------------------------------------------------------------
# 4. Noisy overlap (error-prone)
# ------------------------------------------------------------
def compute_noisy_overlap(a: str, b: str,
                          min_overlap: int,
                          max_error_rate: float) -> int:
    """
    a oxiri bilan b boshi o'rtasidagi MAKSIMAL overlapni topadi,
    lekin overlap ichida Hamming distance foizi <= max_error_rate bo'lishi kerak.

    best_ov = 0
    for k in range(len(a), min_overlap-1, -1):
        s1 = a[-k:]
        s2 = b[:k]
        mism = hamming_distance(s1, s2)
        if mism / k <= max_error_rate:
            best_ov = k
            break
    """
    max_len = min(len(a), len(b))
    for ov in range(max_len, min_overlap - 1, -1):
        s1 = a[-ov:]
        s2 = b[:ov]
        mism = hamming_distance(s1, s2)
        if mism / ov <= max_error_rate:
            return ov
    return 0


# ------------------------------------------------------------
# 5. Har bir read uchun eng yaxshi successor (noisy)
# ------------------------------------------------------------
def compute_best_successors(
    reads: List[str],
    index: Dict[str, List[int]],
    k: int,
    min_overlap: int,
    max_error_rate: float,
) -> Tuple[List[Optional[int]], List[int]]:
    """
    Task 1 dagidek, lekin overlap funksiya:
        compute_noisy_overlap(...)
    """
    n = len(reads)
    best_next: List[Optional[int]] = [None] * n
    best_ov: List[int] = [0] * n

    for i in range(n):
        read_i = reads[i]
        if len(read_i) < k:
            continue
        suffix = read_i[-k:]
        candidates = index.get(suffix, [])

        best_j: Optional[int] = None
        best_overlap = 0

        for j in candidates:
            if j == i:
                continue
            ov = compute_noisy_overlap(read_i, reads[j],
                                       min_overlap, max_error_rate)
            if ov > best_overlap:
                best_overlap = ov
                best_j = j

        best_next[i] = best_j
        best_ov[i] = best_overlap

    return best_next, best_ov


# ------------------------------------------------------------
# 6. Start vertex topish va genomni yig'ish (xuddi Task 1 bilan bir xil)
# ------------------------------------------------------------
def find_start_vertex(best_next: List[Optional[int]], n: int) -> int:
    """
    Task 1 dagi bilan bir xil:
      - incoming_count massivini hisobla
      - incoming_count[x] == 0 bo'lgan vertexni top
    """
    incoming_count = [0] * n
    for i in range(n):
        j = best_next[i]
        if j is not None:
            incoming_count[j] += 1

    for v in range(n):
        if incoming_count[v] == 0:
            return v

    # fallback
    return 0


def assemble_genome(
    reads: List[str],
    best_next: List[Optional[int]],
    best_ov: List[int],
    start: int
) -> str:
    """
    Task 1 dagi bilan bir xil:
      - path bo'ylab yurib, genomni yig'ish.
    """
    genome = reads[start]
    cur = start
    visited = {start}

    while True:
        nxt = best_next[cur]
        if nxt is None or nxt in visited:
            break
        ov = best_ov[cur]
        genome += reads[nxt][ov:]
        cur = nxt
        visited.add(cur)

    return genome


# ------------------------------------------------------------
# 7. main()
# ------------------------------------------------------------
def main():
    reads = read_reads()
    if not reads:
        return

    index = build_kmer_index(reads, KMER_LEN)

    best_next, best_ov = compute_best_successors(
        reads,
        index,
        KMER_LEN,
        MIN_OVERLAP,
        MAX_ERROR_RATE,
    )

    n = len(reads)
    start = find_start_vertex(best_next, n)

    genome = assemble_genome(reads, best_next, best_ov, start)

    print(genome)


if __name__ == "__main__":
    main()
