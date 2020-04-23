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





vi deltaOneSolution = {3,11,12,14,15,23,27,31,35,36,37,40,41,42,44,45,46,47,49,50,52,53,56,70,72,73,76,79,80,84,85,90,92,94,96,97,98,105,107,109,110,112,118,121,122,124,125,128,136,139,142,143,144,147,149,153,154,156,158,159,165,167,168,172,174,176,177,179,181,182,184,185,188,194,199,200,201,202,203,205,206,208,212,216,218,224,225,232,233,234,237,239,240,241,243,245,248,249,251,252,254,255,257,258,260,268,270,272,273,275,277,280,286,291,295,299,300,308,311,314,318,319,323,328,330,332,333,335,342,343,347,349,350,351,352,355,359,360,364,365,369,370,371,376,379,381,386,392,394,395,397,401,403,411,413,414,417,418,420,421,422,423,427,428,431,435,437,438,439,442,444,445,448,454,459,464,466,467,468,471,474,478,479,480,486,488,490,492,494,499,501,505,509,513,519,524,525,526,527,531,536,537,546,550,552,553,557,559,562,568,573,575,576,580,581,583,584,586,587,589,590,593,597,598,599,604,605,617,618,621,622,623,624,627,628,631,634,637,638,639,642,643,644,645,648,650,652,653,654,655,660,661,667,670,675,678,682,683,684,690,692,695,697,703,706,716,718,721,722,728,729,733,737,738,739,740,742,744,747,753,754,757,758,761,762,766,767,768,772,773,775,776,781,782,785,786,791,796,801,803,804,806,809,810,811,812,813,815,817,819,821,822,825,830,838,839,841,842,845,847,848,850,851,852,857,862,863,864,867,869,874,875,876,880,881,883,884,885,887,888,895,896,901,902,904,906,909,911,913,915,917,918,920,925,928,930,931,932,933,937,942,944,953,954,958,959,960,962,963,969,972,973,977,981,983,990,993,994,995,997,1002,1003,1004,1008,1009,1011,1013,1020,1021,1029,1030,1031,1035,1036,1041,1043,1048,1054,1055,1060,1061,1063,1065,1066,1070,1071,1074,1075,1076,1077,1078,1079,1082,1084,1087,1088,1092,1096,1097,1099,1100,1102,1103,1104,1105,1109,1111,1115,1116,1127,1129,1132,1137,1144,1145,1146,1147,1153,1157,1159,1160,1161,1162,1163,1164,1165,1166,1167,1171,1172,1178,1179,1183,1184,1186,1187,1190,1193,1194,1195,1200,1201,1205,1208,1209,1210,1211,1213,1214,1217,1218,1219,1220,1222,1227,1228,1232,1233,1235,1236,1238,1240,1241,1242,1246,1247,1257,1258,1259,1261,1263,1266,1269,1270,1271,1274,1275,1276,1282,1284,1285,1289,1290,1293,1298,1299,1303,1307,1309,1313,1314,1315,1316,1318,1320,1323,1325,1327,1328,1329,1333,1338,1339,1340,1343,1345,1346,1351,1355,1357,1359,1362,1368,1380,1381,1384,1385,1387,1389,1391,1395,1397,1408,1410,1412,1419,1420,1427,1429,1431,1432,1433,1435,1441,1445,1450,1453,1455,1457,1462,1465,1471,1472,1473,1475,1476,1478,1479,1483,1484,1486,1487,1489,1494,1501,1502,1503,1506,1507,1510,1513,1514,1516,1519,1524,1525,1527,1530,1531,1533,1535,1536,1537,1538,1541,1547,1549,1551,1552,1555,1556,1560,1562,1563,1565,1566,1575,1576,1577,1579,1580,1581,1583,1587,1588,1591,1594,1599,1603,1604,1605,1606,1608,1612,1613,1614,1615,1617,1618,1620,1621,1627,1631,1635,1636,1637,1638,1640,1644,1648,1649,1650,1656,1657,1661,1662,1664,1665,1669,1671,1673,1677,1678,1684,1685,1689,1692,1693,1694,1695,1696,1703,1704,1710,1712,1713,1714,1716,1718,1721,1724,1725,1729,1731,1736,1738,1740,1742,1744,1745,1747,1748,1750,1756,1757,1758,1759,1760,1762,1764,1765,1766,1767,1769,1770,1771,1772,1774,1775,1785,1786,1787,1789,1792,1795,1796,1797,1799,1801,1804,1807,1810,1811,1818,1825,1826,1828,1829,1830,1832,1833,1834,1839,1840,1842,1843,1844,1845,1848,1851,1854,1855,1857,1858,1859,1861,1863,1865,1867,1870,1872,1874,1876,1878,1879,1882,1884,1885,1887,1889,1891,1894,1897,1901,1902,1903,1908,1909,1910,1911,1914,1915,1917,1921,1924,1925,1927,1928,1930,1932,1939,1941,1944,1947,1949,1950,1951,1952,1955,1956,1961,1962,1963,1964,1966,1968,1972,1973,1976,1977,1979,1980,1984,1989,1990,1992,1993,1995,1999,2001,2003,2004,2008,2012,2013,2018,2019,2020,2021,2022,2024,2029,2030,2033,2036,2037,2040,2041,2044,2046,2047};



const int MAXN = 2048;
const ll NEED = 8778048582380;
vll NUMS;

const ld PENALTY = 1.0;


struct Item {
    V<char> a;
    ll sum;
    ld val;

    Item() {
        this->a.resize(MAXN);
    }

    void eval() {
        //this->val = 1.0 - sqrt(this->sum <= NEED ? (NEED - this->sum) / ((ld)NEED) : (this->sum - NEED) * PENALTY);
        this->val = 1.0 - (this->sum <= NEED ? (NEED - this->sum) / ((ld)NEED) : (this->sum - NEED) * PENALTY);
    }

    void calc() {
        this->sum = 0;
        for (int i = 0; i < MAXN; ++i) {
            if (this->a[i]) {
                this->sum += NUMS[i];
            }
        }
        this->eval();
    }
};

bool operator<(const Item& x, const Item& y) {
    return x.val < y.val;
}


const int HEURISTIC_NUM = 2;

Item getHeuristicSolution(int n) {
    Item res;
    if (n == 0) {

    } else if (n == 1) {
        map<ll, int> indByNum;
        for (int i = 0; i < MAXN; ++i) {
            indByNum[NUMS[i]] = i;
        }
        
        ll sum = 0;
        for (auto it = indByNum.rbegin(); it != indByNum.rend(); ++it) {
            if (sum + it->first <= NEED) {
                sum += it->first;
                res.a[it->second] = true;
            }
        }
    } else if (n == 2) {
        for (const auto& it : deltaOneSolution) {
            res.a[it] = true;
        }
    }

    res.calc();
    return res;
}

/////////////////////////////////////
const int N = 1000;

const int CROSS_ITER = 0;

const int CROSS_BEST_ITER = 10;

const int MUT_ITER = 100;

const int MUT_BEST_ITER = 0;

const int MIN_MUT_CNT = 1;
const int MAX_MUT_CNT = 100 + 1;
/////////////////////////////////////


mt19937 gen(123);
V<Item> data;


void cross(int x, int y) {
    if (x == y) {
        return;
    }

    data.emplace_back();
    auto& back = data.back();
    
    int pt = gen() % (MAXN - 1) + 1;
    for (int i = 0; i < pt; ++i) {
        back.a[i] = data[x].a[i];
    }
    for (int i = pt; i < MAXN; ++i) {
        back.a[i] = data[y].a[i];
    }
    back.calc();
}


void mutate(int x) {
    data.push_back(data[x]);
    auto& back = data.back();

    int cnt = (gen() % (MAX_MUT_CNT - MIN_MUT_CNT)) + MIN_MUT_CNT;
    for (int iter = 0; iter < cnt; ++iter) {
        int i = gen() % MAXN;
        if (back.a[i]) {
            back.sum -= NUMS[i];
        } else {
            back.sum += NUMS[i];
        }
        back.a[i] ^= 1;
    }
    back.eval();
}


void solve() {
    NUMS.resize(MAXN); cin >> NUMS;

    data.resize(N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < MAXN; ) {
            auto x = gen();
            for (int k = 0; k < 32; ++k, ++j) {
                data[i].a[j] = bool(x & (1 << k));
            }
        }
        data[i].calc();
    }

    for (int i = 0; i < HEURISTIC_NUM; ++i) {
        data[i] = getHeuristicSolution(i);
    }

    for (int iteration = 0; ; ++iteration) {
        auto best = max_element(all(data));
        int bestInd = best - data.begin();

        if (best->sum == NEED) {
            swap(data.front(), *best);
            break;
        }

        if (iteration % 10000 == 0) {
            cout << iteration << " " << NEED - best->sum << endl;
        }

        
        nth_element(data.rbegin(), data.rbegin() + (data.size() - N), data.rend());
        data.resize(N);

        for (int iter = 0; iter < CROSS_ITER; ++iter) {
            cross(gen() % N, gen() % N);
        }
        for (int iter = 0; iter < CROSS_BEST_ITER; ++iter) {
            cross(bestInd, gen() % N);
        }

        for (int iter = 0; iter < MUT_ITER; ++iter) {
            mutate(gen() % N);
        }
        for (int iter = 0; iter < MUT_BEST_ITER; ++iter) {
            mutate(bestInd);
        }
    }


    for (int i = 0; i < MAXN; ++i) {
        if (data.front().a[i]) {
            cout << i << ",";
        }
    }
    cout << endl;

}


int main() {

    solve();
    
    return 0;
}

