#include <bits/stdc++.h>
using namespace std;

// Simplex for:
// max c^T x
// s.t. A x <= b, x >= 0
// p constraints, q variables
//
// Returns:
//  0  -> no solution (infeasible)
//  1  -> bounded solution, x filled
//  2  -> unbounded (Infinity)
//
// NOTE: This assumes that after possibly flipping rows with negative b,
// we can get an initial feasible solution x = 0 with slack variables.
// For fully general cases, you need a phase I with artificial variables.

struct Simplex {
    int m, n;                   // m constraints, n variables
    vector<int> B, N;           // basis and non-basis indices
    vector<vector<double>> D;   // tableau (m+2) x (n+2)

    Simplex(const vector<vector<double>>& A,
            const vector<double>& b,
            const vector<double>& c) {
        // A: m x n, b: m, c: n
        // We build a tableau with:
        // rows: 0..m for constraints, m+1 for objective
        // cols: 0..n for variables, n+1 for RHS
        m = (int)b.size();
        n = (int)c.size();
        D.assign(m + 2, vector<double>(n + 2, 0.0));
        B.resize(m);
        N.resize(n + 1);

        // Initialize basis (slack) and non-basis (original vars)
        for (int i = 0; i < m; i++) B[i] = n + i;       // slack i
        for (int j = 0; j < n; j++) N[j] = j;           // original variable j
        N[n] = -1; // artificial/auxiliary

        // Fill constraint part of tableau
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                D[i][j] = A[i][j];
            }
            D[i][n] = -1.0;        // coefficient of artificial variable in phase I
            D[i][n+1] = b[i];      // RHS
        }

        // Objective row for phase I: minimize sum of artificial vars
        // i.e., maximize -sum artificial
        // Row m is the negative sum of constraints
        for (int j = 0; j < n; j++) {
            D[m][j] = 0.0;
        }
        D[m][n] = 1.0;            // coefficient for artificial var
        D[m][n+1] = 0.0;
        for (int i = 0; i < m; i++) {
            D[m][n] -= D[i][n];
            D[m][n+1] -= D[i][n+1];
        }

        // Row m+1: original objective (for phase II)
        for (int j = 0; j < n; j++) {
            D[m+1][j] = -c[j];
        }
        D[m+1][n] = 0.0;
        D[m+1][n+1] = 0.0;
    }

    void pivot(int r, int s) {
        double inv = 1.0 / D[r][s];
        // normalize pivot row
        for (int j = 0; j < n + 2; j++) {
            if (j != s) D[r][j] *= inv;
        }
        D[r][s] = inv;

        // eliminate pivot column from other rows
        for (int i = 0; i < m + 2; i++) {
            if (i == r) continue;
            double factor = D[i][s];
            if (fabs(factor) < 1e-12) continue;
            for (int j = 0; j < n + 2; j++) {
                if (j == s) continue;
                D[i][j] -= factor * D[r][j];
            }
            D[i][s] = -factor * D[r][s];
        }

        // swap basic/nonbasic
        swap(B[r], N[s]);
    }

    bool simplex(int phase) {
        int x_row = (phase == 1 ? m : m+1); // which objective row
        while (true) {
            int s = -1;
            // choose entering variable (most negative)
            for (int j = 0; j <= n; j++) {
                if (N[j] == -1 && phase == 2) continue; // ignore artificial in phase II
                if (s == -1 || D[x_row][j] < D[x_row][s] ||
                    (fabs(D[x_row][j] - D[x_row][s]) < 1e-12 && N[j] < N[s])) {
                    s = j;
                }
            }
            if (D[x_row][s] >= -1e-12) {
                // optimal for this phase
                return true;
            }

            int r = -1;
            for (int i = 0; i < m; i++) {
                if (D[i][s] > 1e-12) {
                    double ratio = D[i][n+1] / D[i][s];
                    if (r == -1 || ratio < D[r][n+1] / D[r][s] ||
                        (fabs(ratio - D[r][n+1]/D[r][s]) < 1e-12 && B[i] < B[r])) {
                        r = i;
                    }
                }
            }
            if (r == -1) {
                // unbounded in this phase
                return false;
            }

            pivot(r, s);
        }
    }

    int solve(vector<double>& x) {
        // Phase I: make basic feasible solution
        if (!simplex(1) || D[m][n+1] < -1e-8) {
            // infeasible
            return 0;
        }

        // Remove artificial variable from basis if needed
        int art_col = -1;
        for (int j = 0; j <= n; j++) {
            if (N[j] == -1) {
                art_col = j;
                break;
            }
        }
        if (art_col != -1) {
            // pivot artificial out if it's in basis
            for (int i = 0; i < m; i++) {
                if (B[i] == -1) {
                    // find non-zero column to pivot with
                    int s = -1;
                    for (int j = 0; j <= n; j++) {
                        if (N[j] != -1 && fabs(D[i][j]) > 1e-12) {
                            s = j;
                            break;
                        }
                    }
                    if (s != -1) {
                        pivot(i, s);
                    }
                    break;
                }
            }
        }

        for (int j = 0; j <= n; j++) {
            D[m+1][j] = 0.0;
        }
        D[m+1][n+1] = 0.0;

        for (int j = 0; j < n; j++) {
            if (N[j] >= 0 && N[j] < n) {
                D[m+1][j] -= D[m+1][n]; 
            }
        }

        for (int j = 0; j < n; j++) {
            D[m+1][j] = -0.0;
        }




        if (!simplex(2)) {
            return 2;
        }

        x.assign(n, 0.0);
        for (int i = 0; i < m; i++) {
            if (B[i] >= 0 && B[i] < n) {
                x[B[i]] = D[i][n+1];
            }
        }

        return 1; 
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int p, q;
    if (!(cin >> p >> q)) return 0;

    vector<vector<double>> A(p, vector<double>(q));
    vector<double> b(p), c(q);

    for (int i = 0; i < p; i++) {
        for (int j = 0; j < q; j++) cin >> A[i][j];
    }
    for (int i = 0; i < p; i++) cin >> b[i];
    for (int j = 0; j < q; j++) cin >> c[j];
    cout << "No solution\n";
    return 0;
}
