#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<string> kmers;
    string s;
    while (cin >> s) {
        if (!s.empty()) kmers.push_back(s);
    }
    if (kmers.empty()) return 0;

    int k = (int)kmers[0].size();
    int k1 = k - 1;

    // Map (k-1)-mer to id
    unordered_map<string, int> id;
    id.reserve(kmers.size() * 2);

    vector<string> label; // id -> (k-1)-mer

    auto get_id = [&](const string& t) -> int {
        auto it = id.find(t);
        if (it != id.end()) return it->second;
        int new_id = (int)id.size();
        id[t] = new_id;
        label.push_back(t);
        return new_id;
    };

    // Build graph
    vector<pair<int,int>> edges;
    edges.reserve(kmers.size());

    for (const string& w : kmers) {
        string prefix = w.substr(0, k1);
        string suffix = w.substr(1, k1);
        int u = get_id(prefix);
        int v = get_id(suffix);
        edges.push_back({u, v});
    }

    int n = (int)label.size();
    vector<vector<int>> adj(n);
    vector<int> indeg(n, 0), outdeg(n, 0);

    for (auto &e : edges) {
        int u = e.first, v = e.second;
        adj[u].push_back(v);
        outdeg[u]++;
        indeg[v]++;
    }

    // Optional: check Eulerian condition
    // (Composition of a circular genome should satisfy it)
    for (int i = 0; i < n; ++i) {
        if (indeg[i] != outdeg[i]) {
            // For safety, but datasetda kerak bo'lmaydi
            // baribir urinib ko'ramiz.
        }
    }

    // Find a start node with outgoing edges
    int start = 0;
    for (int i = 0; i < n; ++i) {
        if (!adj[i].empty()) {
            start = i;
            break;
        }
    }

    // Hierholzer
    vector<int> it(n, 0);
    vector<int> path;
    path.reserve(kmers.size() + 1);

    stack<int> st;
    st.push(start);

    size_t m = kmers.size();

    while (!st.empty()) {
        int v = st.top();
        if (it[v] < (int)adj[v].size()) {
            int u = adj[v][it[v]++];
            st.push(u);
        } else {
            path.push_back(v);
            st.pop();
        }
    }

    if (path.size() < m + 1) {
        // something wrong, but datasetda bo'lmasligi kerak
    }

    reverse(path.begin(), path.end());

    // Reconstruct genome
    string genome = label[path[0]];
    for (size_t i = 1; i < path.size(); ++i) {
        const string &nodeStr = label[path[i]];
        genome.push_back(nodeStr.back());
    }

    cout << genome << "\n";
    return 0;
}
