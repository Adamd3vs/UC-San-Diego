#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<int>> A(n, vector<int>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            cin >> A[i][j];

    vector<long long> b(n);
    for (int i = 0; i < n; ++i) cin >> b[i];

    vector<vector<int>> clauses;

    // Maxsus hol: 0 <= bi bo'lmagan qatorda barcha koeff nol bo'lsa, sistemaning o'zi unsat
    for (int i = 0; i < n; ++i) {
        bool allZero = true;
        for (int j = 0; j < m; ++j)
            if (A[i][j] != 0) { allZero = false; break; }
        if (allZero && 0LL > b[i]) {
            int V = max(1, m);
            cout << 2 << " " << V << "\n";
            cout << 1 << " 0\n";
            cout << -1 << " 0\n";
            return 0;
        }
    }

    // Har inequality uchun buzuvchi assign-larni taqiqlaymiz
    for (int i = 0; i < n; ++i) {
        vector<int> idx;   // ustunlar
        vector<int> coef;  // koefflar
        for (int j = 0; j < m; ++j) {
            if (A[i][j] != 0) {
                idx.push_back(j);      // 0-based ustun
                coef.push_back(A[i][j]);
            }
        }
        int k = (int)idx.size();
        if (k == 0) continue; // 0 <= bi (buzmaydi), hech nima qo‘shmaymiz

        int total = 1 << k;   // barcha bit kombinatsiyalari
        for (int mask = 0; mask < total; ++mask) {
            long long sum = 0;
            for (int t = 0; t < k; ++t) {
                int val = (mask & (1 << t)) ? 1 : 0;
                sum += 1LL * coef[t] * val;
            }
            if (sum > b[i]) {
                // Bu kombinatsiya taqiqlanadi
                vector<int> cl;
                for (int t = 0; t < k; ++t) {
                    int var = idx[t] + 1;     // SAT variable index
                    int val = (mask & (1 << t)) ? 1 : 0;
                    if (val == 1)
                        cl.push_back(-var);   // x = 1 bo'lishi taqiqlanadi
                    else
                        cl.push_back(var);    // x = 0 bo'lishi taqiqlanadi
                }
                clauses.push_back(cl);
            }
        }
    }

    int V = max(1, m);

    // Agar hech qanday cheklov bo'lmasa (har qanday x mos keladi) – oddiy tautologiya
    if (clauses.empty()) {
        cout << 1 << " " << V << "\n";
        cout << 1 << " -1 0\n";   // (x1 OR ¬x1)
        return 0;
    }

    int C = (int)clauses.size();
    // C <= 5000 bo'lishi kerak, lekin nazariy maksimal 8*n <= 4000, shuning uchun xavfsiz.

    cout << C << " " << V << "\n";
    for (auto &cl : clauses) {
        for (int x : cl) cout << x << " ";
        cout << 0 << "\n";
    }

    return 0;
}
