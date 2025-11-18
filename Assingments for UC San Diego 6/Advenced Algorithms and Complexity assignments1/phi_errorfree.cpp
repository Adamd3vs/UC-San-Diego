// phiX174_overlap_errorfree.cpp
// Assembling phi X174 genome from ERROR-FREE reads using overlap graph.

#include <bits/stdc++.h>
using namespace std;

// ====== Settings ======
const int KMER = 12;       // real datasetda 12, sample uchun avtomatik moslashamiz

// Map: k-mer -> list of read indices that contain it
using KmerIndex = unordered_map<string, vector<int>>;

// Build index of all k-mers in all reads
KmerIndex build_kmer_index(const vector<string> &reads, int k) {
    KmerIndex idx;
    for (int i = 0; i < (int)reads.size(); ++i) {
        const string &r = reads[i];
        int n = (int)r.size();
        if (n < k) continue;
        for (int pos = 0; pos + k <= n; ++pos) {
            string sub = r.substr(pos, k);
            idx[sub].push_back(i);
        }
    }
    return idx;
}

// Exact overlap of suffix(a) and prefix(b)
// returns maximum overlap length >= min_overlap (else 0)
int exact_overlap(const string &a, const string &b, int min_overlap) {
    int n = (int)a.size();
    int m = (int)b.size();
    int best = 0;

    for (int len = min_overlap; len <= min(n, m); ++len) {
        bool ok = true;
        for (int i = 0; i < len; ++i) {
            if (a[n - len + i] != b[i]) {
                ok = false;
                break;
            }
        }
        if (ok) best = len;
    }
    return best;
}

// Choose, for each read i, the best outgoing edge (i -> j) with max overlap
// Using k-mer index: har bir k-merdagi list bo'yicha hamma (i,j) juftliklarni ko'ramiz.
void choose_best_successor(const vector<string> &reads,
                           const KmerIndex &idx,
                           int min_overlap,
                           vector<int> &best_to,
                           vector<int> &best_ov) {
    int n = (int)reads.size();
    best_to.assign(n, -1);
    best_ov.assign(n, 0);

    // Har bir k-mer uchun shu k-merga ega bo'lgan readlar juftligini tekshiramiz
    for (const auto &kv : idx) {
        const auto &vec = kv.second;
        int sz = (int)vec.size();
        if (sz < 2) continue;

        for (int a = 0; a < sz; ++a) {
            int i = vec[a];
            const string &ri = reads[i];
            for (int b = 0; b < sz; ++b) {
                if (a == b) continue;
                int j = vec[b];
                const string &rj = reads[j];

                int ov = exact_overlap(ri, rj, min_overlap);
                if (ov > best_ov[i]) {
                    best_ov[i] = ov;
                    best_to[i] = j;
                }
            }
        }
    }
}

// Build greedy path from best successors
vector<int> build_greedy_path(const vector<int> &succ) {
    int n = (int)succ.size();
    vector<int> indeg(n, 0);
    for (int i = 0; i < n; ++i) {
        if (succ[i] != -1) indeg[succ[i]]++;
    }

    // Start from node with indeg == 0 (if any)
    int start = 0;
    for (int i = 0; i < n; ++i) {
        if (indeg[i] == 0) {
            start = i;
            break;
        }
    }

    vector<int> path;
    vector<int> used(n, 0);
    int cur = start;
    while (cur != -1 && !used[cur]) {
        used[cur] = 1;
        path.push_back(cur);
        cur = succ[cur];
    }

    return path;
}

// Spell genome string from path and overlaps
string spell_genome(const vector<string> &reads,
                    const vector<int> &path,
                    const vector<int> &best_ov) {
    if (path.empty()) return "";

    string genome = reads[path[0]];

    for (size_t i = 1; i < path.size(); ++i) {
        int u = path[i - 1];
        int v = path[i];
        int ov = best_ov[u];  // succ[u] bilan bo'lgan overlap

        if (ov < 0) ov = 0;
        if (ov > (int)reads[v].size()) ov = (int)reads[v].size();

        genome += reads[v].substr(ov);
    }

    return genome;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<string> reads;
    string s;
    while (cin >> s) {
        if (!s.empty())
            reads.push_back(s);
    }

    if (reads.empty()) return 0;

    int read_len = (int)reads[0].size();
    int kmer = min(KMER, read_len);  // sample: 3, real: 12

    // Build k-mer index
    KmerIndex idx = build_kmer_index(reads, kmer);

    // Overlap threshold:
    // sample uchun MIN_OV = 1; real datasetda read_len=100 bo'lsa → 50
    int MIN_OV = max(1, read_len / 2);

    vector<int> succ, best_ov;
    succ.resize(reads.size());
    choose_best_successor(reads, idx, MIN_OV, succ, best_ov);

    vector<int> path = build_greedy_path(succ);
    string genome = spell_genome(reads, path, best_ov);

    cout << genome << "\n";
    return 0;
}
