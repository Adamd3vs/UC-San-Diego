#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    const long long INF = (long long)4e18;
    vector<vector<long long>> dist(n, vector<long long>(n, INF));
    for (int i = 0; i < n; ++i) dist[i][i] = 0;

    for (int i = 0; i < m; ++i) {
        int u, v;
        long long t;
        cin >> u >> v >> t;
        --u; --v;
        dist[u][v] = min(dist[u][v], t);
        dist[v][u] = min(dist[v][u], t);
    }

    int N = 1 << n;
    vector<vector<long long>> dp(N, vector<long long>(n, INF));
    vector<vector<int>> parent(N, vector<int>(n, -1));

    int start = 0;
    dp[1 << start][start] = 0;

    for (int mask = 0; mask < N; ++mask) {
        for (int v = 0; v < n; ++v) {
            if (!(mask & (1 << v))) continue;
            long long cur = dp[mask][v];
            if (cur == INF) continue;
            for (int u = 0; u < n; ++u) {
                if (mask & (1 << u)) continue;
                if (dist[v][u] == INF) continue;
                int nmask = mask | (1 << u);
                long long cand = cur + dist[v][u];
                if (cand < dp[nmask][u]) {
                    dp[nmask][u] = cand;
                    parent[nmask][u] = v;
                }
            }
        }
    }

    int full = N - 1;
    long long best = INF;
    int last = -1;

    for (int v = 0; v < n; ++v) {
        if (v == start) continue;
        if (dp[full][v] == INF || dist[v][start] == INF) continue;
        long long total = dp[full][v] + dist[v][start];
        if (total < best) {
            best = total;
            last = v;
        }
    }

    if (best == INF) {
        cout << -1 << '\n';
        return 0;
    }

    cout << best << '\n';

    vector<int> path;
    int mask = full;
    int v = last;
    while (v != -1) {
        path.push_back(v);
        int pv = parent[mask][v];
        mask ^= (1 << v);
        v = pv;
    }
    reverse(path.begin(), path.end());

    // print visiting order (1..n), WITHOUT repeating start at the end
    for (int i = 0; i < (int)path.size(); ++i) {
        cout << path[i] + 1;
        if (i + 1 < (int)path.size()) cout << ' ';
    }
    cout << '\n';

    return 0;
}
