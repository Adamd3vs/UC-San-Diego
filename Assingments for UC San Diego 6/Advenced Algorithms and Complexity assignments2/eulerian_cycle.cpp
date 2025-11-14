#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<int>> adj(n + 1);
    vector<int> indeg(n + 1, 0), outdeg(n + 1, 0);

    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        outdeg[u]++;
        indeg[v]++;
    }

    // Check Euler condition
    for (int v = 1; v <= n; ++v) {
        if (indeg[v] != outdeg[v]) {
            cout << 0 << "\n";
            return 0;
        }
    }

    if (m == 0) {
        // no edges -> no non-trivial Euler cycle
        cout << 0 << "\n";
        return 0;
    }

    int start = 1;
    for (int v = 1; v <= n; ++v) {
        if (outdeg[v] > 0) {
            start = v;
            break;
        }
    }

    // Hierholzer
    vector<int> it(n + 1, 0);
    vector<int> circuit;
    circuit.reserve(m + 1);

    stack<int> st;
    st.push(start);

    while (!st.empty()) {
        int v = st.top();
        if (it[v] < (int)adj[v].size()) {
            int u = adj[v][it[v]++];
            st.push(u);
        } else {
            circuit.push_back(v);
            st.pop();
        }
    }

    if ((int)circuit.size() != m + 1) {
        cout << 0 << "\n";
        return 0;
    }

    reverse(circuit.begin(), circuit.end());

    cout << 1 << "\n";
    // We need m vertices v1..vm, edges (v1,v2)...(vm,v1)
    for (int i = 0; i < m; ++i) {
        if (i) cout << " ";
        cout << circuit[i];
    }
    cout << "\n";
    return 0;
}
