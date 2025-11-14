#include <bits/stdc++.h>
using namespace std;

// x_{v,p}: vertex v at position p
int var_id(int v, int p, int n) {
    return (v - 1) * n + p; // 1-based
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<bool>> adj(n + 1, vector<bool>(n + 1, false));
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        if (u == v) continue;
        adj[u][v] = adj[v][u] = true;
    }

    vector<vector<int>> clauses;

    // 1) Har vertex kamida bir pozitsiyada bo‘lsin
    for (int v = 1; v <= n; ++v) {
        vector<int> cl;
        for (int p = 1; p <= n; ++p)
            cl.push_back(var_id(v, p, n));
        clauses.push_back(cl);
    }

    // 2) Vertex bir martadan ko‘p chiqmasin
    for (int v = 1; v <= n; ++v) {
        for (int p = 1; p <= n; ++p) {
            for (int q = p + 1; q <= n; ++q) {
                vector<int> cl;
                cl.push_back(-var_id(v, p, n));
                cl.push_back(-var_id(v, q, n));
                clauses.push_back(cl);
            }
        }
    }

    // 3) Har pozitsiyada kimdir turishi kerak
    for (int p = 1; p <= n; ++p) {
        vector<int> cl;
        for (int v = 1; v <= n; ++v)
            cl.push_back(var_id(v, p, n));
        clauses.push_back(cl);
    }

    // 4) Bir pozitsiyada ikki vertex bo‘lmasin
    for (int p = 1; p <= n; ++p) {
        for (int u = 1; u <= n; ++u) {
            for (int v = u + 1; v <= n; ++v) {
                vector<int> cl;
                cl.push_back(-var_id(u, p, n));
                cl.push_back(-var_id(v, p, n));
                clauses.push_back(cl);
            }
        }
    }

    // 5) Ketma-ket pozitsiyadagilar orasida qirra bo‘lishi shart.
    // Qirra yo‘q bo‘lsa, bunday kombinatsiyani taqiqlaymiz.
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            if (adj[i][j]) continue;
            for (int p = 1; p <= n - 1; ++p) {
                // i at p, j at p+1 taqiqlansin
                {
                    vector<int> cl;
                    cl.push_back(-var_id(i, p, n));
                    cl.push_back(-var_id(j, p + 1, n));
                    clauses.push_back(cl);
                }
                // j at p, i at p+1 taqiqlansin
                {
                    vector<int> cl;
                    cl.push_back(-var_id(j, p, n));
                    cl.push_back(-var_id(i, p + 1, n));
                    clauses.push_back(cl);
                }
            }
        }
    }

    int V = n * n;
    int C = (int)clauses.size();

    cout << C << " " << V << "\n";
    for (auto &cl : clauses) {
        for (int x : cl) cout << x << " ";
        cout << 0 << "\n";
    }

    return 0;
}
