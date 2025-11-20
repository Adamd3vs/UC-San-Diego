# python3

import sys
from collections import defaultdict

K = 12
MIN_OVERLAP = 30
MAX_ERROR_RATE = 0.05     # 5%


def read_reads():
    return [line.strip() for line in sys.stdin if line.strip()]


def build_kmer_index(reads, k):
    idx = defaultdict(list)
    for i, r in enumerate(reads):
        for pos in range(len(r) - k + 1):
            idx[r[pos:pos+k]].append(i)
    return idx


def approx_overlap(a, b, min_ov):
    max_len = min(len(a), len(b))
    best = 0
    for l in range(max_len, min_ov - 1, -1):
        mism = 0
        for i in range(l):
            if a[len(a)-l+i] != b[i]:
                mism += 1
        if mism <= MAX_ERROR_RATE * l:
            return l
    return best


def choose_best_successor(i, reads, kidx):
    r = reads[i]
    suf = r[-K:]
    if suf not in kidx:
        return (-1, 0)

    best = (-1, 0)

    for j in kidx[suf]:
        if j == i:
            continue
        ov = approx_overlap(r, reads[j], MIN_OVERLAP)
        if ov > best[1]:
            best = (j, ov)
    return best


def assemble(reads):
    n = len(reads)
    kidx = build_kmer_index(reads, K)

    used = set()
    path = []
    cur = 0
    used.add(cur)
    path.append(cur)

    while True:
        nxt, ov = choose_best_successor(cur, reads, kidx)
        if nxt == -1 or nxt in used:
            break
        used.add(nxt)
        path.append(nxt)
        cur = nxt

    genome = reads[path[0]]
    for i in range(1, len(path)):
        a = reads[path[i-1]]
        b = reads[path[i]]
        ov = approx_overlap(a, b, MIN_OVERLAP)
        genome += b[ov:]

    return genome


def main():
    reads = read_reads()
    print(assemble(reads))


if __name__ == "__main__":
    main()
