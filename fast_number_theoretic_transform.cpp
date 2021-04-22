namespace ntt {
// https://www.sciencedirect.com/science/article/pii/S0747717113001181

    const int MAX_SIZE_LB = 0;

    static_assert(MAX_SIZE_LB > 1, "MAX_SIZE_LB not set");

#if 0
    typedef uint32_t num;
    typedef uint64_t dnum;
    const int WORD_WIDTH = 32;

    //const num MOD = 754974721, PRIMITIVE_ROOT = 11;  // MAX_SIZE_LB <= 24
    const num MOD = 998244353, PRIMITIVE_ROOT = 3;  // MAX_SIZE_LB <= 23
    //const num MOD = 1012924417, PRIMITIVE_ROOT = 5;  // MAX_SIZE_LB <= 21
    //const num MOD = 1053818881, PRIMITIVE_ROOT = 7;  // MAX_SIZE_LB <= 20
    //const num MOD = 1056440321, PRIMITIVE_ROOT = 3;  // MAX_SIZE_LB <= 19
    //const num MOD = 1073479681, PRIMITIVE_ROOT = 11;  // MAX_SIZE_LB <= 18
#else
    typedef uint64_t num;
    typedef __uint128_t dnum;
    const int WORD_WIDTH = 64;

    const num MOD = 4611686018326724609, PRIMITIVE_ROOT = 3;  // MAX_SIZE_LB <= 25
    //const num MOD = 4611686018058289153, PRIMITIVE_ROOT = 5;  // MAX_SIZE_LB <= 25
    //const num MOD = 4611686016649003009, PRIMITIVE_ROOT = 17;  // MAX_SIZE_LB <= 25
    //const num MOD = 4611686015709478913, PRIMITIVE_ROOT = 3;  // MAX_SIZE_LB <= 25
    //const num MOD = 4611686013092233217, PRIMITIVE_ROOT = 3;  // MAX_SIZE_LB <= 25
#endif

    const num DMOD = MOD << 1;

    inline uint32_t prod(uint32_t a, uint32_t b) {
        const uint64_t r = (uint64_t)a * b;
        return r >= MOD ? r % MOD : r;
    }

    inline uint64_t prod(uint64_t a, uint64_t b) {
        const uint64_t x = a * b - ((uint64_t)((long double)a * b / MOD) - 1) * MOD;
        return x >= DMOD ? x - DMOD : (x >= MOD ? x - MOD : x);
    }

    num binpow(num x, num n) {
        num res = 1;
        for (; n; n >>= 1, x = ntt::prod(x, x))
            if (n & 1) res = ntt::prod(res, x);
        return res;
    }

    const int MAX_SIZE = 1 << MAX_SIZE_LB;

    num rootPows[MAX_SIZE],
        rootPowsConj[MAX_SIZE],
        twoPowsInv[MAX_SIZE_LB + 1],
        twoPowsInvConj[MAX_SIZE_LB + 1];

    void init_() {
        static bool wasInitialized = false;
        if (wasInitialized) return;
        wasInitialized = true;
        num r = binpow(PRIMITIVE_ROOT, MOD >> MAX_SIZE_LB);
        rootPows[MAX_SIZE / 2 - 1] = binpow(r, MOD - 2);
        for (int i = MAX_SIZE / 2; i < MAX_SIZE; ++i) {
            rootPows[i] = prod(rootPows[i - 1], r);
            rootPowsConj[i] = ((dnum)rootPows[i] << WORD_WIDTH) / MOD;
        }
        for (int i = MAX_SIZE / 2 - 1; i; --i) {
            rootPows[i] = rootPows[i << 1];
            rootPowsConj[i] = rootPowsConj[i << 1];
        }
        num twoInv = binpow(2, MOD - 2);
        for (int i = 0; i <= MAX_SIZE_LB; ++i) {
            twoPowsInv[i] = i ? prod(twoPowsInv[i - 1], twoInv) : 1;
            twoPowsInvConj[i] = ((dnum)twoPowsInv[i] << WORD_WIDTH) / MOD;
        }
    }

    typedef vector<num> Poly;

    bool isPowerOfTwo(uint64_t n) {
        return n && !(n & (n - 1));
    }

    void ntt(Poly& a, bool inv = false) {
        init_();
        int n = a.size();
        assert(isPowerOfTwo(n) && n <= MAX_SIZE);
        if (inv)
            reverse(a.begin() + 1, a.end());
        for (int i = 1, j = 0, bit; i < n; ++i) {
            bit = n >> __builtin_ctz(i);
            j -= n - bit - (bit >> 1);
            if (i < j) swap(a[i], a[j]);
        }
        for (int len = 2, hlen = 1; len <= n; len <<= 1, hlen <<= 1)
            for (int i = 0; i < n; i += len)
                for (int j = 0; j < hlen; ++j) {
                    num x = a[i + j];
                    const num y = a[i + j + hlen];
                    const num t = rootPows[j + hlen] * y - (num)((dnum)rootPowsConj[j + hlen] * y >> WORD_WIDTH) * MOD;
                    if (x >= DMOD) x -= DMOD;
                    a[i + j] = x + t;
                    a[i + j + hlen] = x - t + DMOD;
                }
        if (inv) {
            const int nLb = __builtin_ctz(n);
            const num nInv = twoPowsInv[nLb];
            const dnum nInvConj = twoPowsInvConj[nLb];
            for (int i = 0; i < n; ++i)
                a[i] = nInv * a[i] - (num)(nInvConj * a[i] >> WORD_WIDTH) * MOD;
        }
        for (int i = 0; i < n; ++i) {
            if (a[i] >= DMOD) a[i] -= DMOD;
            if (a[i] >= MOD) a[i] -= MOD;
        }
    }

    int binceil(int n) {
         int r = 1 << (31 - __builtin_clz(n));
         return r << (r != n);
    }

    void trim(Poly& a) {
        for (; !a.empty() && a.back() == 0; a.pop_back());
    }

    void mult(Poly& a, Poly& b, Poly& res) {
        trim(a);
        trim(b);
        if (a.empty() || b.empty()) {
            res.resize(0);
            return;
        }
        int as = a.size(), bs = b.size();
        int n = binceil(as + bs - 1);
        a.resize(n);
        fill(a.begin() + as, a.end(), 0);
        ntt(a);
        if (&a != &b) {
            b.resize(n);
            fill(b.begin() + bs, b.end(), 0);
            ntt(b);
        }
        for (int i = 0; i < n; ++i)
            res[i] = prod(a[i], b[i]);
        ntt(res, true);
        res.resize(as + bs - 1);
    }

    void assign_(Poly& a, Poly& b) {
        a.resize(b.size());
        copy(b.begin(), b.end(), a.begin());
    }

    void binpow(Poly& a, uint64_t k, uint32_t maxsz = -1) {
        assert(k);
        auto trim_ = [&maxsz] (Poly& x) {
            x.resize(min<uint32_t>(x.size(), maxsz));
            trim(x);
        };
        trim_(a);
        if (a.empty() || isPowerOfTwo(k)) {
            if (!a.empty())
                while (k >>= 1) {
                    mult(a, a, a);
                    trim_(a);
                }
            return;
        }
        static Poly sq;
        assign_(sq, a);
        --k;
        while (true) {
            if (k & 1) {
                static Poly tmp;
                if (k != 1) assign_(tmp, sq);
                mult(a, sq, a);
                trim_(a);
                if (k != 1) assign_(sq, tmp);
            }
            if (k >>= 1) {
                mult(sq, sq, sq);
                trim_(sq);
            } else break;
        }
    }
}

