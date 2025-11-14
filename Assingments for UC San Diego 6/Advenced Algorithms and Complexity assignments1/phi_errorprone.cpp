#include <bits/stdc++.h>
using namespace std;

// suffix(a) va prefix(b) orasida, mismatches/len <= 5% shart
// bajariladigan eng katta overlap uzunligini topamiz.
int best_overlap_fuzzy(const string& a, const string& b) {
    int L = (int)a.size();
    int min_ov = max(1, L / 2); // 100 -> 50

    for (int len = L; len >= min_ov; --len) {
        int start = L - len;
        int max_mism = len / 20; // 5% (floor)
        int mism = 0;
        bool ok = true;
        for (int k = 0; k < len; ++k) {
            if (a[start + k] != b[k]) {
                ++mism;
                if (mism > max_mism) {
                    ok = false;
                    break;
                }
            }
        }
        if (ok) return len; // eng katta overlapni topdik
    }
    return 0;
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

    int n = (int)reads.size();
    int L = (int)reads[0].size();

    vector<int> succ(n, -1);
    vector<int> ovlen(n, 0);

    // Har juftlik uchun eng yaxshi fuzzy overlap
    for (int i = 0; i < n; ++i) {
        int bestLen = 0;
        int bestJ = -1;
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            int len = best_overlap_fuzzy(reads[i], reads[j]);
            if (len > bestLen) {
                bestLen = len;
                bestJ = j;
            }
        }
        succ[i] = bestJ;
        ovlen[i] = bestLen;
    }

    // indegree
    vector<int> indeg(n, 0);
    for (int i = 0; i < n; ++i) {
        if (succ[i] != -1)
            indeg[succ[i]]++;
    }

    // start vertex
    int start = 0;
    for (int i = 0; i < n; ++i) {
        if (indeg[i] == 0) {
            start = i;
            break;
        }
    }

    vector<int> order;
    vector<bool> used(n, false);
    int cur = start;
    for (int step = 0; step < n; ++step) {
        if (used[cur]) break;
        used[cur] = true;
        order.push_back(cur);
        int nxt = succ[cur];
        if (nxt == -1) break;
        cur = nxt;
    }

    // Qolganlarini ham ulab yuboramiz (har ehtimolga qarshi)
    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            cur = i;
            while (!used[cur]) {
                used[cur] = true;
                order.push_back(cur);
                int nxt = succ[cur];
                if (nxt == -1) break;
                cur = nxt;
            }
        }
    }

    // Superstring
    string genome = reads[order[0]];
    for (int k = 1; k < (int)order.size(); ++k) {
        int prev = order[k - 1];
        int v = order[k];
        int len = best_overlap_fuzzy(reads[prev], reads[v]);
        genome += reads[v].substr(len);
    }

    cout << genome << "\n";
    return 0;
}
