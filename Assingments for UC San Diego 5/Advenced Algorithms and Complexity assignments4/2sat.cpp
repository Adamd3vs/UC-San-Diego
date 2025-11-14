#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int V, C;
    if (!(cin >> V >> C)) return 0;

    int N = 2 * V;
    vector<vector<int>> g(N), gr(N);

    auto lit_to_node = [&](int lit) {
        int v = abs(lit) - 1;   // variable index 0-based
        int node = 2 * v;       // x_v
        if (lit < 0) node ^= 1; // ¬x_v
        return node;
    };

    for (int i = 0; i < C; ++i) {
        int a, b;
        cin >> a >> b;
        int A = lit_to_node(a);
        int B = lit_to_node(b);
        int NA = A ^ 1;
        int NB = B ^ 1;

        // (a OR b) == (¬a -> b) AND (¬b -> a)
        g[NA].push_back(B);
        gr[B].push_back(NA);
        g[NB].push_back(A);
        gr[A].push_back(NB);
    }

    vector<bool> used(N, false);
    vector<int> order;
    order.reserve(N);

    function<void(int)> dfs1 = [&](int v) {
        used[v] = true;
        for (int to : g[v])
            if (!used[to]) dfs1(to);
        order.push_back(v);
    };

    for (int v = 0; v < N; ++v)
        if (!used[v]) dfs1(v);

    vector<int> comp(N, -1);
    int comp_cnt = 0;

    function<void(int,int)> dfs2 = [&](int v, int c) {
        comp[v] = c;
        for (int to : gr[v])
            if (comp[to] == -1) dfs2(to, c);
    };

    for (int i = N - 1; i >= 0; --i) {
        int v = order[i];
        if (comp[v] == -1) dfs2(v, comp_cnt++);
    }

    vector<int> assign(V);
    for (int i = 0; i < V; ++i) {
        if (comp[2*i] == comp[2*i + 1]) {
            cout << "UNSATISFIABLE\n";
            return 0;
        }
        assign[i] = (comp[2*i] > comp[2*i + 1]) ? 1 : 0;
    }

    cout << "SATISFIABLE\n";
    for (int i = 0; i < V; ++i) {
        if (assign[i] == 1) cout << (i + 1);
        else                cout << -(i + 1);
        if (i + 1 < V) cout << ' ';
    }
    cout << '\n';

    return 0;
}
