//#pragma comment(linker, "/STACK:200000000")
//#pragma GCC optimize("O2")
//#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")

#include <bits/stdc++.h>

#define all(v) (v).begin(),(v).end()

#ifndef LOCAL
    #define cerr if(false)cerr
#endif

using namespace std;

template<class T> using V = vector<T>;

template<class T1, class T2> istream& operator>>(istream&, pair<T1, T2>&);

template<class T> istream& operator>>(istream& in, vector<T>& v) {
    for (auto& it : v) {
        in >> it;
    }
    return in;
}

template<class T1, class T2> istream& operator>>(istream& in, pair<T1, T2>& p) {
    return in >> p.first >> p.second;
}

template<class T1, class T2> ostream& operator<<(ostream&, const pair<T1, T2>&);

template<class T> ostream& operator<<(ostream& out, const vector<T>& v) {
    out << "[";
    for (int i = 0; i < v.size(); ++i) {
        out << v[i];
        if (i + 1 != v.size()) {
            out << ", ";
        }
    }
    out << "]";
    return out;
}

template<class T1, class T2> ostream& operator<<(ostream& out, const pair<T1, T2>& p) {
    out << "(" << p.first << ", " << p.second << ")";
    return out;
}

//#include <ext/pb_ds/assoc_container.hpp>
//#include <ext/pb_ds/tree_policy.hpp>
//template<class Key, class Value = __gnu_pbds::null_type>
//using ordered_set = __gnu_pbds::tree<
//    Key,
//    Value,
//    less<Key>,
//    __gnu_pbds::rb_tree_tag,
//    __gnu_pbds::tree_order_statistics_node_update
//>; // find_by_order, order_of_key

typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
typedef pair<long long, long long> pll;
typedef vector<int> vi;
typedef vector<long long> vll;

struct MainInitializer {
    int mainStartTime;

    MainInitializer() {
        ios_base::Init();
        cin.sync_with_stdio(false);
        cout.sync_with_stdio(false);
        cin.tie(nullptr);
        cout.tie(nullptr);
        cout << fixed << setprecision(11);
        cerr << fixed << setprecision(11);

        #ifdef LOCAL
            freopen("input.txt", "r", stdin);
            //freopen("output.txt", "w", stdout);

            this->mainStartTime = clock();
        #endif
    }
    
    ~MainInitializer() {
        #ifdef LOCAL
            cerr << fixed << setprecision(9) << "\nTime = " << (long double)(clock() - this->mainStartTime) / CLOCKS_PER_SEC << endl;
        #endif
    }
} mainInitializer;

const int INF = 1e9 + 1e3;
const long long LINF = 1e18 + 1e3;
const int MOD = 1e9 + 7;



const int SZ = 24;

/////////////////////////////////////
const int SEED = 846;
const int N = 15;
const int CROSS_ITER = 5;
const int CROSS_BEST_ITER = 5;
const bool CROSS_TWO_BEST = true;
const int MUT_ITER = 20;
const int MUT_BEST_ITER = 1;
const int MAX_MUT_CNT = 10;
const int PENALTY = 3;
const int STAT_RATE = 100000;
/////////////////////////////////////


struct Item {
    ll r[SZ], c[SZ];
    int val;

    inline int get(size_t i, size_t j) const {
        return (r[i] >> (j << 1)) & 3;
    }

    inline void set(size_t i, size_t j, int val) {
        ll f = get(i, j) ^ val;
        r[i] ^= f << (j << 1);
        c[j] ^= f << (i << 1);
    }

    void eval() {
        int seqEq = 0;
        for (int i = 0; i < SZ; ++i) {
            for (int j = 2; j < SZ; ++j) {
                int val = get(i, j);
                if (val < 2 && get(i, j - 1) == val && get(i, j - 2) == val) {
                    ++seqEq;
                }

                val = get(j, i);
                if (val < 2 && get(j - 1, i) == val && get(j - 2, i) == val) {
                    ++seqEq;
                }
            }
        }

        int emptyCnt = 0;
        int twelveEq = 0;
        for (int i = 0; i < SZ; ++i) {
            int cntR[] = {0, 0}, cntC[] = {0, 0};
            for (int j = 0; j < SZ; ++j) {
                int val = get(i, j);
                if (val < 2) {
                    ++cntR[val];
                } else {
                    ++emptyCnt;
                }

                val = get(j, i);
                if (val < 2) {
                    ++cntC[val];
                }
            }
            if (cntR[0] > 12 || cntR[1] > 12) {
                ++twelveEq;
            }
            if (cntC[0] > 12 || cntC[1] > 12) {
                ++twelveEq;
            }
        }

        V<ll> fullR, fullC;
        for (int i = 0; i < SZ; ++i) {
            if ((r[i] & 187649984473770) == 0) {
                fullR.push_back(r[i]);
            }
            if ((c[i] & 187649984473770) == 0) {
                fullC.push_back(c[i]);
            }
        }
        int eq = (fullR.size() - unordered_set<ll>(all(fullR)).size()) + (fullC.size() - unordered_set<ll>(all(fullC)).size());

        this->val = emptyCnt + PENALTY * (seqEq + twelveEq + eq);
    }
};

bool operator<(const Item& x, const Item& y) {
    return x.val < y.val;
}

bool operator==(const Item& x, const Item& y) {
    return x.r == y.r;
}

ostream& operator<<(ostream& out, const Item& x) {
    out << "Item { " << x.val << endl;
    for (int i = 0; i < SZ; ++i) {
        for (int j = 0; j < SZ; ++j) {
            out << x.get(i, j) << ' ';
        }
        out << endl;
    }
    out << "}";
    return out;
}



mt19937 gen(SEED);
vi okX, okY;

Item mutate(const Item& x) {
    Item res(x);
    int iterNum = gen() % MAX_MUT_CNT;
    for (int iter = 0; iter <= iterNum; ++iter) {
        auto r = gen();
        int i = r % okX.size(), val = (r >> 16) % 3;
        res.set(okX[i], okY[i], val);
    }
    res.eval();
    return res;
}

Item cross(const Item& x, const Item& y) {
    Item res(x);
    int r = gen() % (SZ + 1);
    for (int i = r; i < SZ; ++i) {
        for (int j = 0; j < SZ; ++j) {
            res.set(i, j, y.get(i, j));
        }
    }
    res.eval();
    return res;
}


V<vi> table(SZ, vi(SZ));

Item generate() {
    Item res;
    for (int i = 0; i < SZ; ++i) {
        for (int j = 0; j < SZ; ++j) {
            res.set(i, j, (table[i][j] == -1 ? gen() % 3 : table[i][j]));
        }
    }
    res.eval();
    return res;
}



void solve() {
    cout << SEED << ' ' << N << ' ' << CROSS_ITER << ' ' << CROSS_BEST_ITER << ' ' << CROSS_TWO_BEST << ' ' << MUT_ITER << ' ' << MUT_BEST_ITER << ' ' << MAX_MUT_CNT << ' ' << PENALTY << ' ' << STAT_RATE << endl;

    cin >> table;
    for (int i = 0; i < SZ; ++i) {
        for (int j = 0; j < SZ; ++j) {
            if (table[i][j] == -1) {
                okX.push_back(i);
                okY.push_back(j);
            }
        }
    }

    V<Item> data(N);
    for (int i = 0; i < N; ++i) {
        data[i] = generate();
    }

    for (ll iteration = 0; ; ++iteration) {
        sort(all(data));
        unique(all(data));
        data.resize(N);

        if (iteration % STAT_RATE == 0) {
            cout << "Iteration " << iteration << "\n" << data.front() << endl;
        }

        for (int iter = 0; iter < CROSS_ITER; ++iter) {
            int x = gen() % N, y = gen() % N;
            if (x != y) {
                data.push_back(cross(data[x], data[y]));
            }
        }
        for (int iter = 0; iter < CROSS_BEST_ITER; ++iter) {
            data.push_back(cross(data.front(), data[1 + gen() % (N - 1)]));
        }
        if (CROSS_TWO_BEST) {
            data.push_back(cross(data[0], data[1]));
        }

        for (int iter = 0; iter < MUT_ITER; ++iter) {
            data.push_back(mutate(data[gen() % N]));
        }
        for (int iter = 0; iter < MUT_BEST_ITER; ++iter) {
            data.push_back(mutate(data.front()));
        }
    }
}


int main() {

    solve();
    
    return 0;
}

