#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    if (!(cin >> k)) return 0;

    int n = 1 << (k - 1);           // number of vertices
    int mask = n - 1;
    vector<int> ptr(n, 0);
    vector<int> path;
    path.reserve((1 << k) + 1);

    // Hierholzer on implicit de Bruijn graph
    stack<int> st;
    st.push(0);  // start from 0...0

    while (!st.empty()) {
        int v = st.top();
        if (ptr[v] < 2) {
            int bit = ptr[v]++;
            int u = ((v << 1) & mask) | bit;
            st.push(u);
        } else {
            path.push_back(v);
            st.pop();
        }
    }

    reverse(path.begin(), path.end()); // vertices along Eulerian cycle

    // Build linear de Bruijn string: length = 2^k + k - 1
    string ans;
    ans.reserve((1 << k) + k);

    // first vertex: k-1 bits
    int first = path[0];
    for (int i = k - 2; i >= 0; --i) {
        ans.push_back(((first >> i) & 1) ? '1' : '0');
    }

    // for each next vertex, append its last bit
    for (size_t i = 1; i < path.size(); ++i) {
        int v = path[i];
        ans.push_back((v & 1) ? '1' : '0');
    }

    cout << ans << "\n";
    return 0;
}
