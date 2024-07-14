#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

const double EPS = 1e-9;

int n, m;
const int N = 5, M = 5;
double a[M][N + 1];


int read_equations(const char *file_name) {
    ifstream fin(file_name);
    if (!fin) {
        cout << "Error: cannot open the file " << file_name << endl;
        return 1;
    }

    fin >> m >> n;
    for (int i = 0; i < M; i++)
        for (int j = 0; j <= N; j++)
            fin >> a[i][j];

    return 0;
}

void swap_rows(int i, int j) {
    for (int k = 0; k <= N; k++)
        swap(a[i][k], a[j][k]);
}

int find_pivot(int col) {
    int pivot = col;
    for (int i = col + 1; i < M; i++)
        if (fabs(a[i][col]) > fabs(a[pivot][col]))
            pivot = i;
    return pivot;
}

void print_matrix();

bool gauss() {
    for (int i = 0; i < N; i++) {
        int pivot = find_pivot(i);
        if (fabs(a[pivot][i]) < EPS)
            return false;
        if (pivot != i)
            swap_rows(i, pivot);
        double div = a[i][i];
        for (int j = i + 1; j <= N; j++)
            a[i][j] /= div;
        a[i][i] = 1;
        for (int j = 0; j < M; j++) {
            if (j == i)
                continue;
            double mult = a[j][i];
            for (int k = i + 1; k <= N; k++)
                a[j][k] -= mult * a[i][k];
            a[j][i] = 0;
        }
        cout << "Matrix after elimination of variable x" << (i + 1) << ":" << endl;
        print_matrix();
        cout << endl;
    }
    return true;
}


void solve() {
    if (!gauss()) {
        cout << "The system has no solutions." << endl;
        return;
    }
    cout << "The system has a unique solution:" << endl;
    for (int i = 0; i < N; i++)
        cout << "x" << (i + 1) << " = " << a[i][n] << endl;
}

void print_matrix() {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++)
            cout << a[i][j] << ' ';
        cout << a[i][N] << endl;
    }
}



int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <file_name>" << endl;
        return 1;
    }

    if (read_equations(argv[1]))
        return 1;

    cout << "The input matrix:" << endl;
    print_matrix();
    cout << endl;

    solve();

    return 0;
}