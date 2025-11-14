#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;

vector<double> gauss_solve(vector<vector<double>> A, vector<double> b) {
    int n = (int)A.size();
    for (int i = 0; i < n; i++) {
        A[i].push_back(b[i]); 
    }

    int row = 0;
    for (int col = 0; col < n && row < n; col++) {
        int sel = row;
        for (int i = row; i < n; i++) {
            if (fabs(A[i][col]) > fabs(A[sel][col]))
                sel = i;
        }
        if (fabs(A[sel][col]) < EPS) {
            continue;
        }
        swap(A[sel], A[row]);

        double div = A[row][col];
        for (int j = col; j <= n; j++) {
            A[row][j] /= div;
        }

        for (int i = 0; i < n; i++) {
            if (i == row) continue;
            double factor = A[i][col];
            if (fabs(factor) < EPS) continue;
            for (int j = col; j <= n; j++) {
                A[i][j] -= factor * A[row][j];
            }
        }

        row++;
    }

    vector<double> x(n, 0.0);
    for (int i = 0; i < n; i++) {
        int pivot_col = -1;
        for (int j = 0; j < n; j++) {
            if (fabs(A[i][j]) > EPS) {
                pivot_col = j;
                break;
            }
        }
        if (pivot_col == -1) {
            continue;
        }
        x[pivot_col] = A[i][n];
    }
    return x;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    if (n == 0) {
        return 0;
    }

    vector<vector<double>> A(n, vector<double>(n));
    vector<double> b(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> A[i][j];
        }
        cin >> b[i];
    }

    vector<double> x = gauss_solve(A, b);

    cout << fixed << setprecision(6);
    for (int i = 0; i < n; i++) {
        if (i) cout << ' ';
        cout << x[i];
    }
    cout << "\n";

    return 0;
}
