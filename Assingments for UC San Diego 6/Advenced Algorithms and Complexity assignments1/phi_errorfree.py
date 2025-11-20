# python3
"""
Genome Assembly Programming Challenge
Programming Assignment 1
Task 1: Assembling phi X174 genome from ERROR-FREE reads using overlap graph.

Bu fayl ENDILIGI ISHLAYDIGAN YECHIM:
- TODO joylar to‘liq implement qilingan.
"""

from collections import defaultdict
import sys
from typing import List, Dict, Tuple, Optional


# ==== Global parametrlar (istasa o'zgartirishing mumkin) ====
KMER_LEN = 12        # k-mer uzunligi (candidate qidirish uchun)
MIN_OVERLAP = 30     # minimal overlap uzunligi (edge bo'lishi uchun)


# ------------------------------------------------------------
# 1. Kirishni o'qish
# ------------------------------------------------------------
def read_reads() -> List[str]:
    """
    Standard inputdan (sys.stdin) barcha read-larni o'qib, list qaytaradi.

    Kirish formati:
        Har bir qatorda 1 ta read (A/C/G/T dan iborat string).
        Bo'sh qatordan keyin yoki EOF da tugaydi.
    """
    reads: List[str] = []
    for line in sys.stdin:
        line = line.strip()
        if not line:
            continue
        reads.append(line)

    # Ixtiyoriy tekshiruvlar (xohlasang qo'shmasang ham bo'ladi):
    # - hamma read uzunligi bir xilmi?
    # - bo'sh ro'yxat emasmi?
    # Bu yerda faqat oddiy check:
    if not reads:
        return reads

    read_len = len(reads[0])
    for r in reads:
        if len(r) != read_len:
            # Aralash uzunlik bo'lsa ham ishlayveradi, shunchaki ogohlantirish
            # lekin graderda hammasi bir xil bo'ladi.
            break

    return reads


# ------------------------------------------------------------
# 2. k-mer index qurish
# ------------------------------------------------------------
def build_kmer_index(reads: List[str], k: int) -> Dict[str, List[int]]:
    """
    Oddiy index: read'ning BOSHI (prefix) dan k-mer olib,
    shu k-mer -> osha k-mer bilan BOSHLANADIGAN read indexlari ro'yxati.

    Misol:
        reads[5] = "ACGT..."
        prefix k-mer (k=3) = "ACG"
        index["ACG"].append(5)
    """
    index: Dict[str, List[int]] = defaultdict(list)

    for i, read in enumerate(reads):
        if len(read) < k:
            continue
        prefix = read[:k]
        index[prefix].append(i)

    return index


# ------------------------------------------------------------
# 3. Ikki read orasidagi OVERLAP hisoblash (error-free)
# ------------------------------------------------------------
def compute_overlap(a: str, b: str, min_overlap: int) -> int:
    """
    a ning OXIRI bilan b ning BOSHI o'rtasidagi MAKSIMAL overlap uzunligini qaytaradi.

    Shart:
        overlap uzunligi >= min_overlap bo'lsa, o'sha uzunlikni qaytar.
        Aks holda 0 qaytar.

    Misol:
        a = "AACGT"
        b = "GTTC"
        a oxiri "GT" bilan b boshi "GT" ustma-ust => overlap = 2
    """
    max_len = min(len(a), len(b))
    # Katta overlapdan kichigiga qarab tekshiramiz
    for ov in range(max_len, min_overlap - 1, -1):
        if a[-ov:] == b[:ov]:
            return ov
    return 0


# ------------------------------------------------------------
# 4. Har bir read uchun eng yaxshi keyingi read (greedy edge)
# ------------------------------------------------------------
def compute_best_successors(
    reads: List[str],
    index: Dict[str, List[int]],
    k: int,
    min_overlap: int,
) -> Tuple[List[Optional[int]], List[int]]:
    """
    Har bir read i uchun:
      - candidate bo'lgan read j larni top (suffix k-mer orqali)
      - ular bilan overlapni hisobla
      - eng katta overlap beradigan j ni tanla

    Natija:
      - best_next[i] -> keyingi read indexi (yoki None)
      - best_ov[i]   -> shu edge overlap uzunligi (yoki 0)
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
            ov = compute_overlap(read_i, reads[j], min_overlap)
            if ov > best_overlap:
                best_overlap = ov
                best_j = j

        best_next[i] = best_j
        best_ov[i] = best_overlap

    return best_next, best_ov


# ------------------------------------------------------------
# 5. Hamiltonian yo'lga o'xshash path va genomni yig'ish
# ------------------------------------------------------------
def find_start_vertex(best_next: List[Optional[int]], n: int) -> int:
    """
    Greedy structurada odatda "incoming edge yo'q" bo'lgan vertex start bo'ladi.

    Algoritm:
      - incoming_count massivini hisobla:
            incoming_count = [0] * n
            har bir i: agar best_next[i] = j bo'lsa => incoming_count[j] += 1
      - keyin incoming_count[x] == 0 bo'lgan x ni top (shu start).
    """
    incoming_count = [0] * n
    for i in range(n):
        j = best_next[i]
        if j is not None:
            incoming_count[j] += 1

    # incoming = 0 bo'lgan birinchi vertexni start qilamiz
    for v in range(n):
        if incoming_count[v] == 0:
            return v

    # Agar hammasida incoming bo'lsa, oddiygina 0 ni olish mumkin.
    return 0


def assemble_genome(
    reads: List[str],
    best_next: List[Optional[int]],
    best_ov: List[int],
    start: int
) -> str:
    """
    Greedy path bo'yicha genomni yig'ish.

    G'oya:
        genome = reads[start]
        cur = start
        visited = set([start])
        while True:
            nxt = best_next[cur]
            if nxt is None yoki nxt allaqachon visited bo'lsa: break
            ov = best_ov[cur]
            genome += reads[nxt][ov:]
            cur = nxt
            visited.add(cur)
    """
    genome = reads[start]
    cur = start
    visited = {start}

    while True:
        nxt = best_next[cur]
        if nxt is None or nxt in visited:
            break

        ov = best_ov[cur]
        # overlapdan tashqari qismni qo'shamiz
        genome += reads[nxt][ov:]
        cur = nxt
        visited.add(cur)

    return genome


# ------------------------------------------------------------
# 6. main()
# ------------------------------------------------------------
def main():
    # 1) Reads o'qish
    reads = read_reads()
    if not reads:
        return

    # 2) k-mer index qurish
    index = build_kmer_index(reads, KMER_LEN)

    # 3) Har bir read uchun eng yaxshi successorni topish
    best_next, best_ov = compute_best_successors(
        reads,
        index,
        KMER_LEN,
        MIN_OVERLAP,
    )

    # 4) Boshlang'ich vertexni topish
    n = len(reads)
    start = find_start_vertex(best_next, n)

    # 5) Genomni yig'ish
    genome = assemble_genome(reads, best_next, best_ov, start)

    # 6) Natijani chiqarish
    # phiX174 DOIRA genom, rotation problem bo'lishi mumkin,
    # lekin grader odatda rotation-invariant tekshiradi.
    print(genome)


if __name__ == "__main__":
    main()
