#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to;
    int rev;
    int cap;
};

struct MaxFlow {
    int N;
    vector<vector<Edge>> G;
    MaxFlow(int n) : N(n), G(n) {}

    void add_edge(int u, int v, int c) {
        Edge a{v, (int)G[v].size(), c};
        Edge b{u, (int)G[u].size(), 0};
        G[u].push_back(a);
        G[v].push_back(b);
    }

    int bfs(int s, int t, vector<int>& par_v, vector<int>& par_e) {
        fill(par_v.begin(), par_v.end(), -1);
        fill(par_e.begin(), par_e.end(), -1);
        queue<int> q;
        q.push(s);
        par_v[s] = s;

        while (!q.empty() && par_v[t] == -1) {
            int u = q.front(); q.pop();
            for (int i = 0; i < (int)G[u].size(); i++) {
                Edge &e = G[u][i];
                if (e.cap > 0 && par_v[e.to] == -1) {
                    par_v[e.to] = u;
                    par_e[e.to] = i;
                    q.push(e.to);
                    if (e.to == t) break;
                }
            }
        }

        if (par_v[t] == -1) return 0;

        int add = INT_MAX;
        for (int v = t; v != s; v = par_v[v]) {
            int u = par_v[v];
            int ei = par_e[v];
            add = min(add, G[u][ei].cap);
        }
        for (int v = t; v != s; v = par_v[v]) {
            int u = par_v[v];
            int ei = par_e[v];
            Edge &e = G[u][ei];
            e.cap -= add;
            G[e.to][e.rev].cap += add;
        }
        return add;
    }

    int max_flow(int s, int t) {
        int flow = 0;
        vector<int> par_v(N), par_e(N);
        while (true) {
            int pushed = bfs(s, t, par_v, par_e);
            if (!pushed) break;
            flow += pushed;
        }
        return flow;
    }
};

bool strictly_less(const vector<int>& a, const vector<int>& b) {
    int k = (int)a.size();
    for (int i = 0; i < k; i++) {
        if (a[i] >= b[i]) return false;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<vector<int>> price(n, vector<int>(k));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < k; j++) {
            cin >> price[i][j];
        }
    }

    
    int S = 2*n;
    int T = 2*n + 1;
    MaxFlow mf(2*n + 2);

    for (int i = 0; i < n; i++) {
        mf.add_edge(S, i, 1);
    }

    for (int j = 0; j < n; j++) {
        mf.add_edge(n + j, T, 1);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            if (strictly_less(price[i], price[j])) {
                mf.add_edge(i, n + j, 1);
            }
        }
    }

    int maxMatching = mf.max_flow(S, T);
    int answer = n - maxMatching;
    cout << answer << "\n";

    return 0;
}
