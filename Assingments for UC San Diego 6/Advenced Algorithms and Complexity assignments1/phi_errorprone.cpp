// phiX174_overlap_errorprone.cpp
// Assembling phi X174 genome from ERROR-PRONE reads using overlap graph.

#include <bits/stdc++.h>
using namespace std;

const double MAX_ERROR_RATE = 0.05; // 5% in overlap

// Approximate overlap: suffix(a) vs prefix(b)
// returns longest length >= min_overlap with error_rate <= max_error_rate
int approx_overlap(const string &a, const string &b,
                   int min_overlap,
                   double max_error_rate) {
    int n = (int)a.size();
    int m = (int)b.size();
    int maxLen = min(n, m);

    // eng uzun overlapdan pastga qarab tushamiz
    for (int len = maxLen; len >= min_overlap; --len) {
        int mism = 0;
        int max_mism = (int)floor(max_error_rate * len);

        // suffix(a, len) va prefix(b, len) ni solishtiramiz,
        // mismatch > max_mism bo'lsa darhol to'xtaymiz
        for (int i = 0; i < len; ++i) {
            if (a[n - len + i] != b[i]) {
                ++mism;
                if (mism > max_mism) break;
            }
        }

        if (mism <= max_mism) {
            return len;  // eng katta uzunlikni topdik, darhol qaytamiz
        }
    }
    return 0;
}

// Har bir read i uchun eng yaxshi successor j ni tanlaymiz (O(n^2))
void choose_best_successor(const vector<string> &reads,
                           int min_overlap,
                           double max_error_rate,
                           vector<int> &best_to,
                           vector<int> &best_ov) {
    int n = (int)reads.size();
    best_to.assign(n, -1);
    best_ov.assign(n, 0);

    for (int i = 0; i < n; ++i) {
        const string &ri = reads[i];
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            const string &rj = reads[j];

            int ov = approx_overlap(ri, rj, min_overlap, max_error_rate);
            if (ov > best_ov[i]) {
                best_ov[i] = ov;
                best_to[i] = j;
            }
        }
    }
}

// Greedy path following successor links
vector<int> build_greedy_path(const vector<int> &succ) {
    int n = (int)succ.size();
    vector<int> indeg(n, 0);
    for (int i = 0; i < n; ++i) {
        if (succ[i] != -1) indeg[succ[i]]++;
    }

    // indegree 0 bo'lgan tugundan boshlaymiz (bo'lmasa 0)
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
        path.push_back(cur);
        used[cur] = 1;
        cur = succ[cur];
    }

    return path;
}

// Greedy pathdan genome yig'ish (best_ov dan foydalanamiz)
string spell_genome(const vector<string> &reads,
                    const vector<int> &path,
                    const vector<int> &best_ov) {
    if (path.empty()) return "";
    string genome = reads[path[0]];

    for (size_t k = 1; k < path.size(); ++k) {
        int u = path[k - 1];
        int v = path[k];

        int ov = best_ov[u];  // succ[u] bilan overlap
        if (ov < 0) ov = 0;
        if (ov > (int)reads[v].size()) ov = (int)reads[v].size();

        genome += reads[v].substr(ov);
    }

    return genome;
}

// Circular genome: oxiridan boshi bilan ustma-ust keladigan qismini kesib tashlaymiz
// KMP prefix-function bilan O(n) da eng uzun borderni topamiz.
string reduce_circular(const string &s) {
    int n = (int)s.size();
    if (n == 0) return s;

    vector<int> pi(n, 0);
    for (int i = 1; i < n; ++i) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) {
            j = pi[j - 1];
        }
        if (s[i] == s[j]) ++j;
        pi[i] = j;
    }

    int border = pi[n - 1];   // eng uzun prefix==suffix uzunligi
    if (border > 0) {
        int L = n - border;   // bitta aylananing uzunligi
        return s.substr(0, L);
    }
    return s; // border bo'lmasa o'zi bir aylana deb qabul qilamiz
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

    // error-prone dataset: har bir read 100bp, 1ta xato.
    // Minimal overlapni ~50 bp atrofida olish odatiy.
    int MIN_OV = max(1, read_len / 2);  // 100 bo'lsa → 50

    vector<int> succ, best_ov;
    choose_best_successor(reads, MIN_OV, MAX_ERROR_RATE, succ, best_ov);

    vector<int> path = build_greedy_path(succ);
    string genome = spell_genome(reads, path, best_ov);

    // Circular genome uchun ortiqcha suffixni kesib tashlaymiz
    genome = reduce_circular(genome);

    cout << genome << "\n";
    return 0;
}
