template<class T, class Op> struct Segtree {
    size_t n;
    vector<T> t;
    Op op;

    Segtree(size_t n, const T& x = T()) : n(n), t(n << 1) {
        fill_n(t.begin() + n, n, x);
        build();
    }

    template<class It> Segtree(It b, It e) : n(distance(b, e)), t(n << 1) {
        copy(b, e, t.begin() + n);
        build();
    }

    void build() {
        for (size_t i = n - 1; i; --i) t[i] = op(t[i << 1], t[i << 1 | 1]);
    }

    void set(size_t i, const T& x) {
        for (t[i += n] = x; i >>= 1; ) t[i] = op(t[i << 1], t[i << 1 | 1]);
    }
    
    T get(size_t l, size_t r) {
        T resl, resr;
        bool okl = false, okr = false;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) resl = okl ? op(resl, t[l++]) : t[l++], okl = true;
            if (r & 1) resr = okr ? op(t[--r], resr) : t[--r], okr = true;
        }
        return okl && okr ? op(resl, resr) : (okl ? resl : resr);
    }
};

