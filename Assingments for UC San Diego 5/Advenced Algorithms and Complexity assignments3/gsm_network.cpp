#include <bits/stdc++.h>
using namespace std;

// x_{v,c} : vertex v (1..n), color c (1..3)
int var_id(int v, int c, int n) {
    return 3 * (v - 1) + c; // 1-based index
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<pair<int,int>> edges;
    edges.reserve(m);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        if (u == v) continue;
        edges.push_back({u, v});
    }

    vector<vector<int>> clauses;

    // 1) Har vertex kamida bitta rang olsin
    for (int v = 1; v <= n; ++v) {
        vector<int> cl;
        cl.push_back(var_id(v, 1, n));
        cl.push_back(var_id(v, 2, n));
        cl.push_back(var_id(v, 3, n));
        clauses.push_back(cl);
    }

    // 2) Har vertexda ikki xil rang bo‘lishi taqiqlanadi
    for (int v = 1; v <= n; ++v) {
        for (int c1 = 1; c1 <= 3; ++c1) {
            for (int c2 = c1 + 1; c2 <= 3; ++c2) {
                vector<int> cl;
                cl.push_back(-var_id(v, c1, n));
                cl.push_back(-var_id(v, c2, n));
                clauses.push_back(cl);
            }
        }
    }

    // 3) Har bir qirra uchlari bir xil rang bo‘la olmaydi
    for (auto &e : edges) {
        int u = e.first, v = e.second;
        for (int c = 1; c <= 3; ++c) {
            vector<int> cl;
            cl.push_back(-var_id(u, c, n));
            cl.push_back(-var_id(v, c, n));
            clauses.push_back(cl);
        }
    }

    int V = 3 * n;
    int C = (int)clauses.size();

    cout << C << " " << V << "\n";
    for (auto &cl : clauses) {
        for (int x : cl) cout << x << " ";
        cout << 0 << "\n";
    }

    return 0;
}
