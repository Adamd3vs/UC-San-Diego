#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to;
    long long cap;
    int rev; 
};

struct MaxFlow {
    int n;
    vector<vector<Edge>> adj;

    MaxFlow(int n_) : n(n_), adj(n_) {}

    void add_edge(int u, int v, long long c) {
        Edge a{v, c, (int)adj[v].size()};
        Edge b{u, 0, (int)adj[u].size()};
        adj[u].push_back(a);
        adj[v].push_back(b);
    }

    long long max_flow(int s, int t) {
        long long flow = 0;
        const long long INF = (long long)1e18;

        while (true) {
            vector<int> parent_v(n, -1);
            vector<int> parent_e(n, -1);
            queue<int> q;
            q.push(s);
            parent_v[s] = s;

            while (!q.empty() && parent_v[t] == -1) {
                int u = q.front(); q.pop();
                for (int i = 0; i < (int)adj[u].size(); i++) {
                    Edge &e = adj[u][i];
                    if (e.cap > 0 && parent_v[e.to] == -1) {
                        parent_v[e.to] = u;
                        parent_e[e.to] = i;
                        q.push(e.to);
                        if (e.to == t) break;
                    }
                }
            }

            if (parent_v[t] == -1) break;

            long long add = INF;
            for (int v = t; v != s; v = parent_v[v]) {
                int u = parent_v[v];
                int ei = parent_e[v];
                add = min(add, adj[u][ei].cap);
            }

            for (int v = t; v != s; v = parent_v[v]) {
                int u = parent_v[v];
                int ei = parent_e[v];
                Edge &e = adj[u][ei];
                e.cap -= add;
                adj[e.to][e.rev].cap += add;
            }

            flow += add;
        }
        return flow;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    MaxFlow mf(n); 

    for (int i = 0; i < m; i++) {
        int u, v;
        long long c;
        cin >> u >> v >> c;
        u--; v--;
        mf.add_edge(u, v, c);
    }

    long long ans = mf.max_flow(0, n - 1);
    cout << ans << "\n";

    return 0;
}
