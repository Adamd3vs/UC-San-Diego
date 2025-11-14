#include <bits/stdc++.h>
using namespace std;

char newColor(char cur, int bit) {
    if (cur == 'R') return bit == 0 ? 'G' : 'B';
    if (cur == 'G') return bit == 0 ? 'R' : 'B';
    return bit == 0 ? 'R' : 'G'; // cur == 'B'
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    string s;
    cin >> s;

    int N = 2 * n;
    vector<vector<int>> g(N), gr(N);

    auto addImp = [&](int u, int v) {
        g[u].push_back(v);
        gr[v].push_back(u);
    };

    // literal (x_i != a)
    auto lit = [&](int i, int a) {
        // i: 0-based vertex, a in {0,1}
        if (a == 0) return 2 * i;       // x_i
        else        return 2 * i + 1;   // ¬x_i
    };

    for (int k = 0; k < m; ++k) {
        int u, v;
        cin >> u >> v;
        --u; --v;
        for (int a = 0; a <= 1; ++a) {
            for (int b = 0; b <= 1; ++b) {
                if (newColor(s[u], a) == newColor(s[v], b)) {
                    int A = lit(u, a);
                    int B = lit(v, b);
                    addImp(A ^ 1, B);
                    addImp(B ^ 1, A);
                }
            }
        }
    }

    vector<bool> used(N, false);
    vector<int> order;
    order.reserve(N);

    function<void(int)> dfs1 = [&](int v) {
        used[v] = true;
        for (int to : g[v]) if (!used[to]) dfs1(to);
        order.push_back(v);
    };

    for (int v = 0; v < N; ++v)
        if (!used[v]) dfs1(v);

    vector<int> comp(N, -1);
    int cid = 0;
    function<void(int,int)> dfs2 = [&](int v, int c) {
        comp[v] = c;
        for (int to : gr[v]) if (comp[to] == -1) dfs2(to, c);
    };

    for (int i = N - 1; i >= 0; --i) {
        int v = order[i];
        if (comp[v] == -1) dfs2(v, cid++);
    }

    vector<int> assign(n);
    for (int i = 0; i < n; ++i) {
        if (comp[2*i] == comp[2*i + 1]) {
            cout << "Impossible\n";
            return 0;
        }
        assign[i] = (comp[2*i] > comp[2*i + 1]) ? 1 : 0;
    }

    string ans(n, 'R');
    for (int i = 0; i < n; ++i)
        ans[i] = newColor(s[i], assign[i]);

    cout << ans << '\n';
    return 0;
}
