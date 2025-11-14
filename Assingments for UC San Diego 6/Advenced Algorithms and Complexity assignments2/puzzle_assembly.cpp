#include <bits/stdc++.h>
using namespace std;

struct Piece {
    string up, left, down, right;
};

int N = 5;
vector<Piece> pieces;
vector<int> gridPos;     // index of piece in each cell (0..24)
vector<bool> usedPiece;  // used flags

Piece parse_piece(const string& line) {
    // line format: (up,left,down,right)
    string s;
    for (char c : line) {
        if (c != '(' && c != ')' && c != ' ')
            s.push_back(c);
    }
    // now s = "up,left,down,right"
    vector<string> tokens;
    string cur;
    for (char c : s) {
        if (c == ',') {
            tokens.push_back(cur);
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    if (!cur.empty()) tokens.push_back(cur);
    if (tokens.size() != 4) {
        // very unlikely if input is valid
        cerr << "Parse error\n";
        exit(1);
    }
    Piece p{tokens[0], tokens[1], tokens[2], tokens[3]};
    return p;
}

bool fits(int pos, int idx) {
    int r = pos / N;
    int c = pos % N;
    const Piece &p = pieces[idx];

    // border constraints
    if (r == 0 && p.up != "black") return false;
    if (r == N - 1 && p.down != "black") return false;
    if (c == 0 && p.left != "black") return false;
    if (c == N - 1 && p.right != "black") return false;

    // match with top neighbor
    if (r > 0) {
        int upIdx = gridPos[(r - 1) * N + c];
        if (upIdx == -1) return false;
        const Piece &top = pieces[upIdx];
        if (top.down != p.up) return false;
    }

    // match with left neighbor
    if (c > 0) {
        int leftIdx = gridPos[r * N + (c - 1)];
        if (leftIdx == -1) return false;
        const Piece &lp = pieces[leftIdx];
        if (lp.right != p.left) return false;
    }

    return true;
}

bool dfs(int pos) {
    if (pos == N * N) {
        // Found solution, print it
        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < N; ++c) {
                int idx = gridPos[r * N + c];
                const Piece &p = pieces[idx];
                cout << "(" << p.up << "," << p.left << "," << p.down << "," << p.right << ")";
                if (c + 1 < N) cout << ";";
            }
            cout << "\n";
        }
        return true;
    }

    for (int i = 0; i < (int)pieces.size(); ++i) {
        if (usedPiece[i]) continue;
        if (!fits(pos, i)) continue;

        usedPiece[i] = true;
        gridPos[pos] = i;
        if (dfs(pos + 1)) return true;
        usedPiece[i] = false;
        gridPos[pos] = -1;
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    while (getline(cin, line)) {
        if (line.empty()) continue;
        pieces.push_back(parse_piece(line));
    }

    int total = (int)pieces.size();
    if (total != 25) {
        // dataset uchun 25 bo‘lishi kerak
        // baribir urinamiz
    }

    gridPos.assign(N * N, -1);
    usedPiece.assign(total, false);

    dfs(0);  // yechimni topgach, o‘zi print qiladi
    return 0;
}
