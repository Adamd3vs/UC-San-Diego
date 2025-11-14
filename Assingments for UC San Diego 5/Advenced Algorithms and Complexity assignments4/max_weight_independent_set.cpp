#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> adj;
vector<array<long long,2>> dp;
vector<long long> w;
vector<bool> used;

void dfs(int v, int p) {
    used[v] = true;
    dp[v][1] = w[v]; // take v
    dp[v][0] = 0;    // skip v

    for (int u : adj[v]) {
        if (u == p) continue;
        dfs(u, v);
        dp[v][1] += dp[u][0];
        dp[v][0] += max(dp[u][0], dp[u][1]);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    adj.assign(n + 1, {});
    dp.assign(n + 1, {0, 0});
    w.assign(n + 1, 0);
    used.assign(n + 1, false);

    for (int i = 1; i <= n; ++i) cin >> w[i];

    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    dfs(1, -1);
    cout << max(dp[1][0], dp[1][1]) << '\n';
    return 0;
}
