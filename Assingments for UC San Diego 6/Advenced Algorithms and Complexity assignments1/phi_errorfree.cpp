#include <bits/stdc++.h>
using namespace std;

// Eng uzun overlapni hisoblaydi: suffix(a) va prefix(b) orasida,
// maksimal uzunlikni qaytaradi (mismatch ruxsat ETILMAYDI).
int best_overlap_exact(const string& a, const string& b) {
    int L = (int)a.size();
    int min_ov = max(1, L / 2); // masalan, 100 -> 50

    int best = 0;
    for (int len = L; len >= min_ov; --len) {
        int start = L - len;
        bool ok = true;
        for (int k = 0; k < len; ++k) {
            if (a[start + k] != b[k]) {
                ok = false;
                break;
            }
        }
        if (ok) return len; // eng katta uzunlikdan pastga qarab ketayapmiz
    }
    return best;
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

    // Har bir read uchun eng yaxshi (max overlap) ketma-ket readni tanlaymiz
    vector<int> succ(n, -1);
    vector<int> ovlen(n, 0);

    for (int i = 0; i < n; ++i) {
        int bestLen = 0;
        int bestJ = -1;
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            int len = best_overlap_exact(reads[i], reads[j]);
            if (len > bestLen) {
                bestLen = len;
                bestJ = j;
            }
        }
        succ[i] = bestJ;
        ovlen[i] = bestLen;
    }

    // Path/cycleni topamiz: indegree hisoblaymiz
    vector<int> indeg(n, 0);
    for (int i = 0; i < n; ++i) {
        if (succ[i] != -1)
            indeg[succ[i]]++;
    }

    // Boshlang'ich vertex: indegree = 0 bo'lgan biror vertex, bo'lmasa 0
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

    // Agar ba'zi readlar qolib ketsa, ularga ham ketma-ketlik qo‘shib yuboramiz
    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            // yangi chain boshlaymiz
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

    // Superstringni yig‘ish
    string genome = reads[order[0]];
    for (int k = 1; k < (int)order.size(); ++k) {
        int prev = order[k - 1];
        int v = order[k];
        int len = best_overlap_exact(reads[prev], reads[v]); // ehtiyot uchun qayta hisoblaymiz
        genome += reads[v].substr(len);
    }

    cout << genome << "\n";
    return 0;
}
