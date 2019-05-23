#include "bits/stdc++.h"
#include <mpi.h>


using namespace std;

typedef vector<vector<double>> Matrix;
typedef vector<double> Vector;

const int kSeed = 34;
const int kMaxVal = 20;
const int kRoot = 0;


template<class T>
ostream& operator<<(ostream& os, const vector<T>& a) {
    for (const auto& it : a) {
        os << it << ' ';
    }
    return os;
}

template<class T>
ostream& operator<<(ostream& os, const vector<vector<T>>& a) {
    for (const auto& it : a) {
        os << it << endl;
    }
    return os;
}


Matrix getMatrix(size_t n, int seed) {
    Matrix res(n, Vector(2 * n + 11));
    mt19937 gen(seed);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            res[i][j] = int(gen()) % kMaxVal - kMaxVal / 2;
        }
        res[i][n + i] = 1;
    }
    return res;
}

Vector getVector(size_t n, int seed) {
    Vector res(n, 0);
    mt19937 gen(seed);
    for (size_t i = 0; i < n; ++i) {
        res[i] = int(gen()) % kMaxVal - kMaxVal / 2;
    }
    return res;
}




double operator*(const Vector& a, const Vector& b) {
    return inner_product(a.begin(), a.end(), b.begin(), 0.0);
}

Vector operator*(const Matrix& a, const Vector& x) {
    Vector res;
    for (const auto& it : a) {
        res.push_back(it * x);
    }
    return res;
}

Matrix operator*(const Matrix& a, const Matrix& b) {
    Matrix res(a.size(), Vector(b.front().size()));
    for (size_t i = 0; i < a.size(); ++i) {
        for (size_t j = 0; j < b.front().size(); ++j) {
            for (size_t k = 0; k < b.size(); ++k) {
                res[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return res;
}

double calculateDistance(const Vector& a, const Vector& b) {
    double res = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        res += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return res;
}


int main() {
    size_t n = 1e3; // 20; 10; 7; 5,6
    
    MPI_Init(NULL, NULL);
    int p;
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    int r;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    
    Matrix a;
    if (r == kRoot) {
        a = getMatrix(n, kSeed);
    }
    
    auto time = MPI_Wtime();
    size_t l = 2 * n / p + 1;
    
    if (r != kRoot) {
        a.assign(n, Vector(l));
    }
    for (auto& it : a) {
        MPI_Scatter(it.data(), l, MPI_DOUBLE,
                    it.data(), l, MPI_DOUBLE,
                    kRoot, MPI_COMM_WORLD);
    }
    
    Vector buf(n);
    for (size_t i = 0; i < n; ++i) {
        if (r == i / l) {
            for (size_t j = 0; j < n; ++j) {
                buf[j] = a[j][i % l];
            }
        }
        MPI_Bcast(buf.data(), n, MPI_DOUBLE, i / l, MPI_COMM_WORLD);
        for (size_t j = 0; j < l; ++j) {
            a[i][j] /= buf[i];
        }
        for (size_t j = 0; j < n; ++j) {
            if (j != i) {
                for (size_t k = 0; k < l; ++k) {
                    a[j][k] -= a[i][k] * buf[j];
                }
            }
        }
    }
    
    for (auto& it : a) {
        MPI_Gather(it.data(), l, MPI_DOUBLE,
                   it.data(), l, MPI_DOUBLE,
                   kRoot, MPI_COMM_WORLD);
        if (r == kRoot) {
            copy_n(it.data() + n, n, it.data());
            it.resize(n);
        }
    }
    
    if (r == kRoot) {
        cerr << MPI_Wtime() - time << endl;
        Vector b = getVector(n, 1 + kSeed);
        Vector x = a * b;
        a = getMatrix(n, kSeed);
        for (auto& it : a) {
            it.resize(n);
        }
        cerr << calculateDistance(a * x, b) << endl;
    }
    
    MPI_Finalize();
    
    return 0;
}
