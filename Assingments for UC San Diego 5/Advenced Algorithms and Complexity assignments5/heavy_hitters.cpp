#include <bits/stdc++.h>
using namespace std;

struct Item {
    uint32_t id;
    uint32_t val; 
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;

    long long t;
    cin >> t;

    vector<Item> A(N), B(N);

    // List A: good counts
    for (int i = 0; i < N; ++i) {
        uint32_t k, g;
        cin >> k >> g;
        A[i].id = k;
        A[i].val = g;
    }

    // List B: bad counts
    for (int i = 0; i < N; ++i) {
        uint32_t k, b;
        cin >> k >> b;
        B[i].id = k;
        B[i].val = b;
    }

    int q;
    cin >> q;
    vector<uint32_t> qry(q);
    for (int i = 0; i < q; ++i) cin >> qry[i];

    for (int qi = 0; qi < q; ++qi) {
        uint32_t id = qry[qi];

        long long g = 0, b = 0;

        for (int i = 0; i < N; ++i) {
            if (A[i].id == id) {
                g = A[i].val;
                break;
            }
        }

        for (int i = 0; i < N; ++i) {
            if (B[i].id == id) {
                b = B[i].val;
                break;
            }
        }

        long long diff = g - b;
        int nice = (diff >= t) ? 1 : 0;

        cout << nice;
        if (qi + 1 < q) cout << ' ';
    }
    cout << '\n';

    return 0;
}
