#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double BIG = 1e9;

bool gauss_solve(const vector<vector<double>>& A_in,
                 const vector<double>& b_in,
                 vector<double>& x) {
    int n = (int)A_in.size();
    if (n == 0) {
        x.clear();
        return true;
    }
    int m = (int)A_in[0].size(); 
    vector<vector<double>> A = A_in;
    vector<double> b = b_in;

    for (int i = 0; i < n; i++) {
        A[i].push_back(b[i]);
    }

    int row = 0;
    for (int col = 0; col < m && row < n; col++) {
        int sel = row;
        for (int i = row; i < n; i++) {
            if (fabs(A[i][col]) > fabs(A[sel][col])) sel = i;
        }
        if (fabs(A[sel][col]) < EPS)
            continue; 

        swap(A[sel], A[row]);

        double div = A[row][col];
        for (int j = col; j <= m; j++)
            A[row][j] /= div;

        for (int i = 0; i < n; i++) {
            if (i == row) continue;
            double factor = A[i][col];
            if (fabs(factor) < EPS) continue;
            for (int j = col; j <= m; j++)
                A[i][j] -= factor * A[row][j];
        }
        row++;
    }

    for (int i = 0; i < n; i++) {
        bool all_zero = true;
        for (int j = 0; j < m; j++) {
            if (fabs(A[i][j]) > EPS) {
                all_zero = false;
                break;
            }
        }
        if (all_zero && fabs(A[i][m]) > EPS) {
            return false; 
        }
    }

    x.assign(m, 0.0);
    for (int i = 0; i < n; i++) {
        int pivot_col = -1;
        for (int j = 0; j < m; j++) {
            if (fabs(A[i][j]) > EPS) {
                pivot_col = j;
                break;
            }
        }
        if (pivot_col == -1) continue;
        x[pivot_col] = A[i][m];
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<double>> A(n, vector<double>(m));
    vector<double> b(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) cin >> A[i][j];
    }
    for (int i = 0; i < n; i++) cin >> b[i];

    vector<double> c(m);
    for (int j = 0; j < m; j++) cin >> c[j];

    vector<vector<double>> ineqA;
    vector<double> ineqb;

    for (int i = 0; i < n; i++) {
        ineqA.push_back(A[i]);
        ineqb.push_back(b[i]);
    }

    for (int j = 0; j < m; j++) {
        vector<double> row(m, 0.0);
        row[j] = -1.0;
        ineqA.push_back(row);
        ineqb.push_back(0.0);
    }

    {
        vector<double> row(m, 0.0);
        for (int j = 0; j < m; j++) row[j] = 1.0;
        ineqA.push_back(row);
        ineqb.push_back(BIG);
    }

    int k = (int)ineqA.size();        
    int last_idx = k - 1;             

    double best_val = -1e100;
    vector<double> best_x;
    bool found = false;
    bool best_uses_last = false;

    for (int mask = 0; mask < (1 << k); mask++) {
        if (__builtin_popcount(mask) != m) continue;

        vector<vector<double>> eqA;
        vector<double> eqb;
        eqA.reserve(m);
        eqb.reserve(m);
        bool uses_last = false;

        for (int i = 0; i < k; i++) {
            if (mask & (1 << i)) {
                eqA.push_back(ineqA[i]);
                eqb.push_back(ineqb[i]);
                if (i == last_idx) uses_last = true;
            }
        }

        vector<double> x;
        if (!gauss_solve(eqA, eqb, x)) continue; 

        bool ok = true;
        for (int i = 0; i < k; i++) {
            double lhs = 0.0;
            for (int j = 0; j < m; j++) lhs += ineqA[i][j] * x[j];
            if (lhs > ineqb[i] + 1e-3) {
                ok = false;
                break;
            }
        }
        if (!ok) continue;

        double val = 0.0;
        for (int j = 0; j < m; j++) val += c[j] * x[j];

        if (!found || val > best_val + 1e-6) {
            found = true;
            best_val = val;
            best_x = x;
            best_uses_last = uses_last;
        }
    }

    cout << fixed << setprecision(15);

    if (!found) {
        cout << "No solution\n";
    } else if (best_uses_last) {
        cout << "Infinity\n";
    } else {
        cout << "Bounded solution\n";
        for (int j = 0; j < m; j++) {
            if (j) cout << ' ';
            cout << best_x[j];
        }
        cout << "\n";
    }

    return 0;
}
