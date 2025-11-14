#include <bits/stdc++.h>
using namespace std;

struct BipartiteMatcher {
    int n, m;
    vector<vector<int>> g; 

    vector<int> matchR;    
    vector<int> used;

    BipartiteMatcher(int n_, int m_) : n(n_), m(m_) {
        g.assign(n, {});
        matchR.assign(m, -1);
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
    }

    bool dfs(int u) {
        for (int v : g[u]) {
            if (used[v]) continue;
            used[v] = 1;
            if (matchR[v] == -1 || dfs(matchR[v])) {
                matchR[v] = u;
                return true;
            }
        }
        return false;
    }

    int max_matching(vector<int> &flight_to_crew) {
        int matches = 0;
        flight_to_crew.assign(n, -1);

        for (int u = 0; u < n; u++) {
            used.assign(m, 0);
            if (dfs(u)) {
                matches++;
            }
        }

        for (int v = 0; v < m; v++) {
            if (matchR[v] != -1) {
                int u = matchR[v];
                flight_to_crew[u] = v;
            }
        }
        return matches;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    BipartiteMatcher bm(n, m);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int x;
            cin >> x;
            if (x == 1) {
                bm.add_edge(i, j);
            }
        }
    }

    vector<int> flight_to_crew;
    bm.max_matching(flight_to_crew);

    for (int i = 0; i < n; i++) {
        if (flight_to_crew[i] == -1) cout << -1;
        else cout << (flight_to_crew[i] + 1);
        if (i + 1 < n) cout << ' ';
    }
    cout << "\n";

    return 0;
}
