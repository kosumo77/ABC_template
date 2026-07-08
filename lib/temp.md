# ICPC典型知識まとめ

チーム用リファレンス。早解き向けのテンプレ＆典型コードを中心に、抜けがちな部分も含めて網羅的にまとめる。

---

## 目次

1. [基礎・テンプレート](#1-基礎テンプレート)
2. [データ構造](#2-データ構造)
3. [グラフ](#3-グラフ)
4. [動的計画法（DP）](#4-動的計画法dp)
5. [数学](#5-数学)
6. [文字列](#6-文字列)
7. [二分探索・しゃくとり法](#7-二分探索しゃくとり法)
8. [その他の典型テクニック](#8-その他の典型テクニック)
9. [コンテスト運用Tips](#9-コンテスト運用tips)

---

## 1. 基礎・テンプレート

### 1.1 テンプレート全文

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
template<class T> using vc = vector<T>;
template<class T> using vvc = vector<vector<T>>;

#define rep(i, n) for (ll i = 0; i < (ll)(n); i++)
#define rep1(i, n) for (ll i = 1; i <= (ll)(n); i++)
#define repr(i, n) for (ll i = (ll)(n) - 1; i >= 0; i--)
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()

template<class T> bool chmin(T &a, const T &b) { if (a > b) { a = b; return true; } return false; }
template<class T> bool chmax(T &a, const T &b) { if (a < b) { a = b; return true; } return false; }

constexpr ll INF = 1LL << 60;
constexpr ll MOD = 1000000007; // or 998244353

int main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    // ここに処理を書く

    return 0;
}
```

### 1.2 入出力高速化

- `cin.tie(nullptr); ios::sync_with_stdio(false);` は必須（特に大量入力時）。
- `endl` は毎回フラッシュするので大量出力時は `'\n'` を使う。
- 出力をまとめて1回の `cout` にする（`ostringstream` でバッファしてから出力すると更に速い場合がある）。

```cpp
ostringstream oss;
rep(i, n) oss << a[i] << " \n"[i == n - 1];
cout << oss.str();
```

### 1.3 多次元vectorの初期化

```cpp
vvc<int> dp(h, vc<int>(w, 0));
vvc<vc<int>> dp3(a, vvc<int>(b, vc<int>(c, INF)));
```

### 1.4 デバッグ用マクロ

```cpp
#ifdef LOCAL
#define dbg(x) cerr << #x << " = " << (x) << endl
#else
#define dbg(x)
#endif
```
ローカルでのみ `-DLOCAL` を付けてコンパイルすればジャッジ環境の出力を汚さない。

### 1.5 オーバーフロー・型の注意

- `int` は約2.1×10^9まで。N×N程度の計算で簡単に溢れるので **迷ったら ll を使う**。
- 配列サイズや添字計算は `int` のままだと乗算でオーバーフローしやすい → `(ll)a * b` のようにキャスト。
- 浮動小数点の比較は `eps` を使う（`1e-9` 程度）。

### 1.6 計算量の目安（1秒で大体できる回数）

| 計算量 | N の目安 |
|---|---|
| O(N!) | N ≤ 10 |
| O(2^N) | N ≤ 20〜25 |
| O(N^3) | N ≤ 300〜500 |
| O(N^2) | N ≤ 5000〜10000 |
| O(N log N) | N ≤ 10^6〜10^7 |
| O(N) | N ≤ 10^8 |

ICPCはTLが厳しめのことが多いので余裕を持って見積もる。

---

## 2. データ構造

### 2.1 UnionFind（素集合データ構造）

```cpp
struct UnionFind {
    vector<int> par, sz;
    UnionFind(int n) : par(n, -1), sz(n, 1) {}
    int root(int x) {
        if (par[x] == -1) return x;
        return par[x] = root(par[x]);
    }
    bool same(int x, int y) { return root(x) == root(y); }
    bool unite(int x, int y) {
        x = root(x); y = root(y);
        if (x == y) return false;
        if (sz[x] < sz[y]) swap(x, y);
        par[y] = x;
        sz[x] += sz[y];
        return true;
    }
    int size(int x) { return sz[root(x)]; }
};
```
- 経路圧縮＋union by size でほぼ O(1) amortized。
- 用途: 連結判定、MST（Kruskal）、グループ分け系の典型。

### 2.2 セグメント木（区間min/max/sum, 一点更新）

```cpp
template <class T>
struct SegTree {
    int n;
    vector<T> dat;
    T def; // 単位元
    function<T(T, T)> op;

    // n_: 要素数, def_: 演算の単位元（minならINF, sumなら0）
// op_: 二項演算（例: [](T a, T b){ return min(a, b); }）
SegTree(int n_, T def_, function<T(T, T)> op_) : def(def_), op(op_) {
        n = 1;
        while (n < n_) n <<= 1;
        dat.assign(2 * n, def);
    }
    // i: 更新位置（0-indexed）, x: 更新後の値（上書き）
    void update(int i, T x) {
        i += n;
        dat[i] = x;
        while (i > 1) { i >>= 1; dat[i] = op(dat[2 * i], dat[2 * i + 1]); }
    }
    // [l, r) のクエリ
    T query(int l, int r) {
        T resL = def, resR = def;
        l += n; r += n;
        while (l < r) {
            if (l & 1) resL = op(resL, dat[l++]);
            if (r & 1) resR = op(dat[--r], resR);
            l >>= 1; r >>= 1;
        }
        return op(resL, resR);
    }
};
// 使用例: 区間最小値
// SegTree<ll> seg(n, INF, [](ll a, ll b){ return min(a, b); });
```

### 2.3 BIT（Binary Indexed Tree / Fenwick Tree）

```cpp
template <class T>
struct BIT {
    int n;
    vector<T> bit;
    BIT(int n_) : n(n_), bit(n_ + 1, 0) {}
    // i: 更新位置（0-indexed）, x: 加算する値
    void add(int i, T x) { // 0-indexed
        for (i++; i <= n; i += i & (-i)) bit[i] += x;
    }
    T sum(int i) { // [0, i) の和
        T s = 0;
        for (; i > 0; i -= i & (-i)) s += bit[i];
        return s;
    }
    T sum(int l, int r) { return sum(r) - sum(l); } // [l, r)
};
```
- 用途: 区間和、転倒数の計算（座標圧縮＋BIT）。
- セグ木より実装が軽いので、和だけならBIT優先で良い。

### 2.4 優先度付きキュー（priority_queue）

```cpp
// 最小値を先頭にしたい場合
priority_queue<int, vector<int>, greater<int>> pq;

// pair で (距離, 頂点) を入れてダイクストラなどに使う場合は自動的に first 優先で比較される
priority_queue<pll, vector<pll>, greater<pll>> pq2;
```

### 2.5 スタック・キューを使う典型

- 括弧列のマッチング、ヒストグラム中の最大長方形（**モノトニックスタック**）。
- スライディングウィンドウ最大値（**モノトニックキュー / deque**）。

```cpp
// スライディングウィンドウ最小値（deque使用、O(N)）
deque<int> dq; // 添字を保持、値は単調増加
vector<int> res;
for (int i = 0; i < n; i++) {
    while (!dq.empty() && a[dq.back()] >= a[i]) dq.pop_back();
    dq.push_back(i);
    if (dq.front() <= i - k) dq.pop_front();
    if (i >= k - 1) res.push_back(a[dq.front()]);
}
```

### 2.6 多重集合・順序付き集合

```cpp
multiset<int> ms; // 重複可
auto it = ms.find(x);
if (it != ms.end()) ms.erase(it); // erase(x) だと全部消えるので注意！

set<int> s;
auto it2 = s.lower_bound(x); // x以上の最小要素
```
**注意**: `ms.erase(x)` は値xを**全部**削除する。1つだけ消したいときは iterator を渡す。

---

## 3. グラフ

### 3.1 グラフの持ち方

```cpp
// 隣接リスト（重みなし）
vvc<int> g(n);
g[u].push_back(v);
g[v].push_back(u); // 無向グラフなら両方に追加

// 隣接リスト（重みあり）
vvc<pii> g(n); // (行き先, コスト)
g[u].push_back({v, w});
```

### 3.2 DFS / BFS

```cpp
// DFS（再帰）
vector<bool> visited(n, false);
void dfs(int v) {
    visited[v] = true;
    for (int u : g[v]) {
        if (!visited[u]) dfs(u);
    }
}

// BFS（最短距離、重みなしグラフ）
vector<int> dist(n, -1);
queue<int> q;
dist[s] = 0;
q.push(s);
while (!q.empty()) {
    int v = q.front(); q.pop();
    for (int u : g[v]) {
        if (dist[u] == -1) {
            dist[u] = dist[v] + 1;
            q.push(u);
        }
    }
}
```
**注意**: 再帰DFSは頂点数が大きい（10^5〜）とスタックオーバーフローの危険あり。明示的スタックでのDFSに切り替える、またはコンパイル時にスタックサイズを増やす対策を検討。

### 3.3 ダイクストラ法（単一始点最短路、非負重み）

```cpp
// s: 始点, n: 頂点数, g: 隣接リスト（g[v] = {(隣接頂点u, 辺重みw)}）
// 戻り値: dist[v] = s→v の最短距離（到達不能なら INF）
vector<ll> dijkstra(int s, int n, vvc<pll>& g) {
    vector<ll> dist(n, INF);
    priority_queue<pll, vector<pll>, greater<pll>> pq;
    dist[s] = 0;
    pq.push({0, s});
    while (!pq.empty()) {
        auto [d, v] = pq.top(); pq.pop();
        if (d > dist[v]) continue; // 古い情報はスキップ
        for (auto [u, w] : g[v]) {
            if (chmin(dist[u], d + w)) {
                pq.push({dist[u], u});
            }
        }
    }
    return dist;
}
```
- O((V+E) log V)。**負の辺があると使えない**（その場合はベルマンフォード）。

### 3.4 ベルマンフォード法（負の辺OK、負閉路検出）

```cpp
// 負閉路があれば true を返す
// s: 始点, n: 頂点数, edges: 辺リスト（{u, v, w} = u→v のコストw）
// dist: 結果格納用（参照渡し）。戻り値: 負閉路があれば true
bool bellmanFord(int s, int n, vector<array<ll,3>>& edges, vector<ll>& dist) {
    dist.assign(n, INF);
    dist[s] = 0;
    for (int i = 0; i < n; i++) {
        for (auto& [u, v, w] : edges) {
            if (dist[u] != INF && chmin(dist[v], dist[u] + w)) {
                if (i == n - 1) return true; // n回目でも更新→負閉路
            }
        }
    }
    return false;
}
```

### 3.5 ワーシャルフロイド法（全頂点対最短路）

```cpp
vvc<ll> dist(n, vc<ll>(n, INF));
rep(i, n) dist[i][i] = 0;
// 辺の初期化: dist[u][v] = w;
rep(k, n) rep(i, n) rep(j, n) {
    if (dist[i][k] < INF && dist[k][j] < INF)
        chmin(dist[i][j], dist[i][k] + dist[k][j]);
}
```
- O(N^3)。N ≤ 300〜500 程度が目安。

### 3.6 クラスカル法（最小全域木 / MST）

```cpp
struct Edge { ll w; int u, v; };
// n: 頂点数, edges: 辺リスト（各辺は Edge{w, u, v} = u-v のコストw）
// 戻り値: MSTの総コスト
ll kruskal(int n, vector<Edge>& edges) {
    sort(edges.begin(), edges.end(), [](auto& a, auto& b){ return a.w < b.w; });
    UnionFind uf(n);
    ll total = 0;
    for (auto& e : edges) {
        if (uf.unite(e.u, e.v)) total += e.w;
    }
    return total;
}
```

### 3.7 トポロジカルソート

```cpp
// Kahnのアルゴリズム（BFSベース）
// n: 頂点数, g: 有向グラフの隣接リスト
// 戻り値: トポロジカル順序。order.size() < n ならサイクルが存在する
vector<int> topoSort(int n, vvc<int>& g) {
    vector<int> indeg(n, 0);
    for (int v = 0; v < n; v++) for (int u : g[v]) indeg[u]++;
    queue<int> q;
    for (int v = 0; v < n; v++) if (indeg[v] == 0) q.push(v);
    vector<int> order;
    while (!q.empty()) {
        int v = q.front(); q.pop();
        order.push_back(v);
        for (int u : g[v]) if (--indeg[u] == 0) q.push(u);
    }
    // order.size() < n ならサイクルが存在
    return order;
}
```

### 3.8 強連結成分分解（SCC）

```cpp
// 簡易版: Tarjanアルゴリズム
struct SCC {
    int n, cnt = 0;
    vvc<int> g, rg;
    vector<int> comp, order, used;
    SCC(int n_) : n(n_), g(n_), rg(n_), comp(n_, -1), used(n_, 0) {}
    void addEdge(int u, int v) { g[u].push_back(v); rg[v].push_back(u); }
    void dfs(int v) {
        used[v] = 1;
        for (int u : g[v]) if (!used[u]) dfs(u);
        order.push_back(v);
    }
    void rdfs(int v, int k) {
        comp[v] = k;
        for (int u : rg[v]) if (comp[u] == -1) rdfs(u, k);
    }
    int build() {
        for (int v = 0; v < n; v++) if (!used[v]) dfs(v);
        reverse(order.begin(), order.end());
        for (int v : order) if (comp[v] == -1) rdfs(v, cnt++);
        return cnt; // SCCの個数
    }
};
```
- 用途: 2-SAT、グラフの縮約による DAG 化。

### 3.9 LCA（最小共通祖先、ダブリング）

```cpp
struct LCA {
    int n, log2n;
    vvc<int> parent; // parent[k][v] = vの2^k個上の祖先
    vector<int> depth;
    vvc<int> g;
    LCA(int n_) : n(n_), g(n_) {
        log2n = 1;
        while ((1 << log2n) < n) log2n++;
        parent.assign(log2n + 1, vc<int>(n, -1));
        depth.assign(n, 0);
    }
    void addEdge(int u, int v) { g[u].push_back(v); g[v].push_back(u); }
    void dfs(int v, int p, int d) {
        parent[0][v] = p;
        depth[v] = d;
        for (int u : g[v]) if (u != p) dfs(u, v, d + 1);
    }
    void build(int root = 0) {
        dfs(root, -1, 0);
        for (int k = 0; k < log2n; k++)
            for (int v = 0; v < n; v++)
                parent[k+1][v] = (parent[k][v] == -1) ? -1 : parent[k][parent[k][v]];
    }
    int lca(int u, int v) {
        if (depth[u] > depth[v]) swap(u, v);
        int diff = depth[v] - depth[u];
        for (int k = 0; k < log2n; k++)
            if ((diff >> k) & 1) v = parent[k][v];
        if (u == v) return u;
        for (int k = log2n - 1; k >= 0; k--) {
            if (parent[k][u] != -1 && parent[k][u] != parent[k][v]) {
                u = parent[k][u]; v = parent[k][v];
            }
        }
        return parent[0][u];
    }
};
```
- 木上の2頂点間距離 = `depth[u] + depth[v] - 2*depth[lca(u,v)]`。

### 3.10 二部グラフ判定

```cpp
bool isBipartite(int n, vvc<int>& g) {
    vector<int> color(n, -1);
    for (int s = 0; s < n; s++) {
        if (color[s] != -1) continue;
        queue<int> q; q.push(s); color[s] = 0;
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (int u : g[v]) {
                if (color[u] == -1) { color[u] = 1 - color[v]; q.push(u); }
                else if (color[u] == color[v]) return false;
            }
        }
    }
    return true;
}
```

---

## 4. 動的計画法（DP）

### 4.1 ナップザックDP（0-1ナップザック）

```cpp
// dp[w] = 重さwまでで得られる最大価値
vector<ll> dp(W + 1, 0);
rep(i, n) {
    for (int w = W; w >= weight[i]; w--) {
        chmax(dp[w], dp[w - weight[i]] + value[i]);
    }
}
// 答え: dp[W]
```
- 個数無制限（個数制限なしナップザック）なら `w` のループを **昇順** にするだけ。

### 4.2 LCS（最長共通部分列）

```cpp
int n = s.size(), m = t.size();
vvc<int> dp(n + 1, vc<int>(m + 1, 0));
rep(i, n) rep(j, m) {
    if (s[i] == t[j]) dp[i+1][j+1] = dp[i][j] + 1;
    else dp[i+1][j+1] = max(dp[i][j+1], dp[i+1][j]);
}
// 答え: dp[n][m]
```

### 4.3 LIS（最長増加部分列）— O(N log N)

```cpp
vector<int> a; // 入力列
vector<int> lis;
for (int x : a) {
    auto it = lower_bound(lis.begin(), lis.end(), x); // 非減少ならupper_bound
    if (it == lis.end()) lis.push_back(x);
    else *it = x;
}
// 答え: lis.size()
```

### 4.4 区間DP（典型: 区間分割系、行列連鎖乗算など）

```cpp
// dp[l][r] = 区間[l, r)を処理する最小コスト など
vvc<ll> dp(n + 1, vc<ll>(n + 1, INF));
rep(i, n) dp[i][i+1] = 0; // 長さ1は基底
for (int len = 2; len <= n; len++) {
    for (int l = 0; l + len <= n; l++) {
        int r = l + len;
        for (int m = l + 1; m < r; m++) {
            chmin(dp[l][r], dp[l][m] + dp[m][r] + cost(l, m, r));
        }
    }
}
```

### 4.5 桁DP

```cpp
// 例: N以下である条件を満たす数の個数
string s = to_string(N);
int len = s.size();
// dp[i][tight][...状態] のように持つ
vvc<ll> dp(len + 1, vc<ll>(2, 0));
dp[0][1] = 1; // 最初はNの上限と一致している状態
rep(i, len) {
    rep(tight, 2) {
        if (dp[i][tight] == 0) continue;
        int lim = tight ? (s[i] - '0') : 9;
        for (int d = 0; d <= lim; d++) {
            int ntight = (tight && d == lim) ? 1 : 0;
            // 状態遷移をここに追加
            dp[i+1][ntight] += dp[i][tight];
        }
    }
}
ll ans = dp[len][0] + dp[len][1];
```

### 4.6 集合DP（bitDP, 巡回セールスマン問題TSP）

```cpp
// dp[S][v] = 集合Sを訪問済みで現在頂点vにいる時の最小コスト
vvc<ll> dp(1 << n, vc<ll>(n, INF));
dp[1][0] = 0; // 頂点0から開始
rep(S, 1 << n) rep(v, n) {
    if (dp[S][v] == INF) continue;
    rep(u, n) {
        if (S & (1 << u)) continue;
        int nS = S | (1 << u);
        chmin(dp[nS][u], dp[S][v] + cost[v][u]);
    }
}
// 巡回して戻る場合の答え:
ll ans = INF;
rep(v, n) if (dp[(1<<n)-1][v] < INF) chmin(ans, dp[(1<<n)-1][v] + cost[v][0]);
```
- N ≤ 15〜20 程度が現実的な上限（O(2^N * N^2)）。

### 4.7 DPの典型パターンまとめ（チェックリスト）

- **遷移を「最後に何をしたか」で考える** のが基本姿勢。
- 配るDP / 貰うDP どちらが楽か考える。
- 添字がマイナスにならないよう offset を入れることが多い（差分系DPなど）。
- メモリが厳しい場合は **配列を使い回す**（区間DPの長さ方向、ナップザックの1次元化など）。
- 復元が必要な場合は別配列で「どこから来たか」を記録しておく。

---

## 5. 数学

### 5.1 modpow（繰り返し二乗法）

```cpp
// a: 底, n: 指数, mod: 法 → a^n mod mod を返す
ll modpow(ll a, ll n, ll mod) {
    ll res = 1;
    a %= mod;
    while (n > 0) {
        if (n & 1) res = res * a % mod;
        a = a * a % mod;
        n >>= 1;
    }
    return res;
}
```

### 5.2 GCD / LCM

```cpp
ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; } // C++17以降は std::gcd が使える
ll lcm(ll a, ll b) { return a / gcd(a, b) * b; }
```

### 5.3 拡張ユークリッドの互除法（ax + by = gcd(a,b) の解）

```cpp
// 戻り値: gcd(a, b)。x, yに解が格納される
// a, b: 係数（ax + by = gcd(a,b)）, x/y: 解の格納先（参照渡し）
ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    ll g = extgcd(b, a % b, y, x);
    y -= (a / b) * x;
    return g;
}
```
- 用途: モジュラ逆元の計算、中国剰余定理。

### 5.4 モジュラ逆元

```cpp
// mod が素数の場合（フェルマーの小定理）
// a: 逆元を求めたい数, mod: 素数の法 → a^{-1} mod mod を返す
ll modinv(ll a, ll mod) { return modpow(a, mod - 2, mod); }

// mod が素数でなくてもOK（拡張ユークリッド使用、gcd(a,mod)=1が必要）
// a: 逆元を求めたい数, mod: 法（素数でなくてもよい、ただしgcd(a,mod)=1が必要）
ll modinv_ext(ll a, ll mod) {
    ll x, y;
    extgcd(a, mod, x, y);
    return (x % mod + mod) % mod;
}
```

### 5.5 組合せ論（階乗テーブル＋逆元の前計算）

```cpp
const int MAXN = 2000005;
ll fact[MAXN], inv_fact[MAXN];
void precompute_comb(ll mod) {
    fact[0] = 1;
    for (int i = 1; i < MAXN; i++) fact[i] = fact[i-1] * i % mod;
    inv_fact[MAXN-1] = modpow(fact[MAXN-1], mod - 2, mod);
    for (int i = MAXN - 2; i >= 0; i--) inv_fact[i] = inv_fact[i+1] * (i+1) % mod;
}
// n: 総数, r: 選ぶ数, mod: 法（precompute_combを先に呼ぶ必要あり）
ll comb(int n, int r, ll mod) {
    if (r < 0 || r > n) return 0;
    return fact[n] * inv_fact[r] % mod * inv_fact[n-r] % mod;
}
```
- 前計算 O(N)、クエリ O(1)。MAXNは必要な範囲に応じて調整。

### 5.6 素数判定・エラトステネスの篩

```cpp
bool isPrime(ll n) {
    if (n < 2) return false;
    for (ll i = 2; i * i <= n; i++) if (n % i == 0) return false;
    return true;
}

// 篩（N以下の素数を全列挙）
vector<bool> sieve(int n) {
    vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; (ll)i * i <= n; i++) {
        if (!is_prime[i]) continue;
        for (int j = i * i; j <= n; j += i) is_prime[j] = false;
    }
    return is_prime;
}
```

### 5.7 素因数分解

```cpp
vector<pll> factorize(ll n) {
    vector<pll> res;
    for (ll p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            ll cnt = 0;
            while (n % p == 0) { n /= p; cnt++; }
            res.push_back({p, cnt});
        }
    }
    if (n > 1) res.push_back({n, 1});
    return res;
}
```
- O(√N)。N ≤ 10^12 程度ならこれで十分。事前に篩を作って高速化することも可能（最小素因数テーブル）。

### 5.8 約数列挙

```cpp
vector<ll> divisors(ll n) {
    vector<ll> res;
    for (ll i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            res.push_back(i);
            if (i != n / i) res.push_back(n / i);
        }
    }
    sort(res.begin(), res.end());
    return res;
}
```

### 5.9 中国剰余定理（CRT）

```cpp
// x ≡ r1 (mod m1), x ≡ r2 (mod m2) を満たす x (mod lcm(m1,m2)) を求める
// 戻り値: {x, lcm}。解なしなら lcm = -1
// r1, m1: 1つ目の合同式の余り・法, r2, m2: 2つ目の合同式の余り・法
// 戻り値: {x mod lcm, lcm}。解なし(m1,m2が互いに素でない場合)は lcm = -1
pll crt(ll r1, ll m1, ll r2, ll m2) {
    ll p, q;
    ll d = extgcd(m1, m2, p, q);
    if ((r2 - r1) % d != 0) return {0, -1};
    ll lcm = m1 / d * m2;
    ll tmp = (r2 - r1) / d % (m2 / d) * p % (m2 / d);
    ll r = ((r1 + m1 * tmp) % lcm + lcm) % lcm;
    return {r, lcm};
}
```

### 5.10 行列累乗（漸化式の高速計算）

```cpp
using Mat = vvc<ll>;
// a: 左の行列(n×k), b: 右の行列(k×m), mod: 法 → 積(n×m)を返す
Mat matmul(const Mat& a, const Mat& b, ll mod) {
    int n = a.size(), m = b[0].size(), k = b.size();
    Mat c(n, vc<ll>(m, 0));
    rep(i, n) rep(j, m) rep(l, k) c[i][j] = (c[i][j] + a[i][l] * b[l][j]) % mod;
    return c;
}
// a: 正方行列, n: 指数, mod: 法 → a^n を返す
Mat matpow(Mat a, ll n, ll mod) {
    int sz = a.size();
    Mat res(sz, vc<ll>(sz, 0));
    rep(i, sz) res[i][i] = 1; // 単位行列
    while (n > 0) {
        if (n & 1) res = matmul(res, a, mod);
        a = matmul(a, a, mod);
        n >>= 1;
    }
    return res;
}
// 用途: フィボナッチ数列の高速計算など、線形漸化式 O(K^3 log N)
```

### 5.11 浮動小数点・誤差処理の注意

- `1e-9` 程度の eps を使って比較する。
- 累積誤差が怖い場合は可能な限り整数演算に持ち込む（座標を整数化、分数のまま比較するなど）。

---

## 6. 文字列

### 6.1 ローリングハッシュ

```cpp
struct RollingHash {
    vector<ll> hash1, hash2, pow1, pow2;
    ll mod1 = 1000000007, mod2 = 998244353;
    ll base1 = 1009, base2 = 1007;

    RollingHash(const string& s) {
        int n = s.size();
        hash1.assign(n + 1, 0); hash2.assign(n + 1, 0);
        pow1.assign(n + 1, 1); pow2.assign(n + 1, 1);
        rep(i, n) {
            hash1[i+1] = (hash1[i] * base1 + s[i]) % mod1;
            hash2[i+1] = (hash2[i] * base2 + s[i]) % mod2;
            pow1[i+1] = pow1[i] * base1 % mod1;
            pow2[i+1] = pow2[i] * base2 % mod2;
        }
    }
    // s[l, r) のハッシュ値（pair）
    pll get(int l, int r) {
        ll h1 = (hash1[r] - hash1[l] * pow1[r-l] % mod1 + mod1 * mod1) % mod1;
        ll h2 = (hash2[r] - hash2[l] * pow2[r-l] % mod2 + mod2 * mod2) % mod2;
        return {h1, h2};
    }
};
```
- 用途: 部分文字列の一致判定をO(1)で、文字列検索、最長共通部分文字列の二分探索など。
- 衝突対策として2つのmodを併用（pair）するのが安全。

### 6.2 Zアルゴリズム

```cpp
// z[i] = sとs[i:]の最長共通接頭辞の長さ
vector<int> zAlgorithm(const string& s) {
    int n = s.size();
    vector<int> z(n, 0);
    z[0] = n;
    int i = 1, j = 0;
    while (i < n) {
        while (i + j < n && s[j] == s[i + j]) j++;
        z[i] = j;
        if (j == 0) { i++; continue; }
        int k = 1;
        while (i + k < n && k + z[k] < j) { z[i + k] = z[k]; k++; }
        i += k; j -= k;
    }
    return z;
}
```
- O(N)。パターンマッチング（`pattern + '#' + text` の形で連結して使う）にも使える。

### 6.3 KMP法（文字列検索）

```cpp
vector<int> buildKMPTable(const string& pat) {
    int m = pat.size();
    vector<int> table(m, 0);
    int j = 0;
    for (int i = 1; i < m; i++) {
        while (j > 0 && pat[i] != pat[j]) j = table[j - 1];
        if (pat[i] == pat[j]) j++;
        table[i] = j;
    }
    return table;
}
vector<int> kmpSearch(const string& text, const string& pat) {
    vector<int> table = buildKMPTable(pat);
    vector<int> res;
    int j = 0;
    for (int i = 0; i < (int)text.size(); i++) {
        while (j > 0 && text[i] != pat[j]) j = table[j - 1];
        if (text[i] == pat[j]) j++;
        if (j == (int)pat.size()) {
            res.push_back(i - j + 1);
            j = table[j - 1];
        }
    }
    return res;
}
```

### 6.4 文字列の典型注意点

- 回文判定: 両端からポインタを動かす、または Manacher法 (O(N)で全回文半径)。
- 辞書順最小・最大の構築は貪欲法 + スタックが定番（例: 重複文字除去系）。
- `string` の `substr` は O(N) コピーが発生するので、ループ内で多用すると TLE しやすい。

---

## 7. 二分探索・しゃくとり法

### 7.1 二分探索（典型形）

```cpp
// 「条件を満たす最小のx」を探す（条件は単調: f(x)がfalse,false,...,true,true,...の形）
// lo: 探索下限（f(lo)=false想定）, hi: 探索上限（f(hi)=true想定）
// f: 単調な条件関数。戻り値: f(x)=trueとなる最小のx
ll binarySearch(ll lo, ll hi, function<bool(ll)> f) {
    // f(lo)はfalse, f(hi)はtrueを想定（区間は適宜調整）
    while (hi - lo > 1) {
        ll mid = lo + (hi - lo) / 2;
        if (f(mid)) hi = mid;
        else lo = mid;
    }
    return hi;
}
```
- STLの `lower_bound` / `upper_bound` は配列が**ソート済み**であることが前提。
- 実数の二分探索は反復回数を固定する（`for (int i = 0; i < 100; i++)` など）と楽。

### 7.2 しゃくとり法（two pointers）

```cpp
// 例: 和がX以下になる最長区間の長さ
int n = a.size();
ll sum = 0;
int r = 0, ans = 0;
for (int l = 0; l < n; l++) {
    while (r < n && sum + a[r] <= X) { sum += a[r]; r++; }
    chmax(ans, r - l);
    if (l == r) r++; // lがrに追いついた場合の処理
    else sum -= a[l];
}
```
- 単調性（区間を広げると条件が緩む/狭めると条件が厳しくなる）がある場合に使える。O(N)。

---

## 8. その他の典型テクニック

### 8.1 座標圧縮

```cpp
vector<ll> a = /* 元の値の配列 */;
vector<ll> b = a;
sort(b.begin(), b.end());
b.erase(unique(b.begin(), b.end()), b.end());
for (auto& x : a) {
    x = lower_bound(b.begin(), b.end(), x) - b.begin(); // 0-indexed圧縮後の値
}
```

### 8.2 imos法（差分配列、累積和の逆操作）

```cpp
// 区間[l, r)に値xを加算する操作をQ回行った後の最終配列を求める
vector<ll> imos(n + 1, 0);
// 各クエリで: imos[l] += x; imos[r] -= x;
rep(i, n) imos[i+1] += imos[i]; // 累積和を取ると最終結果
```
- 2次元版（矩形領域への加算）も同様に2次元累積和で実現できる。

### 8.3 累積和

```cpp
vector<ll> s(n + 1, 0);
rep(i, n) s[i+1] = s[i] + a[i];
// 区間[l, r)の和 = s[r] - s[l]

// 2次元累積和
vvc<ll> s2(h + 1, vc<ll>(w + 1, 0));
rep(i, h) rep(j, w) s2[i+1][j+1] = s2[i][j+1] + s2[i+1][j] - s2[i][j] + a[i][j];
// 矩形[(y1,x1), (y2,x2)) の和 = s2[y2][x2] - s2[y1][x2] - s2[y2][x1] + s2[y1][x1]
```

### 8.4 区間スケジューリング（貪欲法）

```cpp
// 終了時刻でソートし、貪欲に選んでいく → 最大選択数を求める典型
vector<pll> intervals; // (start, end)
sort(intervals.begin(), intervals.end(), [](auto& a, auto& b){ return a.second < b.second; });
ll lastEnd = -INF;
int cnt = 0;
for (auto& [s, e] : intervals) {
    if (s >= lastEnd) { cnt++; lastEnd = e; }
}
```

### 8.5 半分全列举（meet in the middle）

```cpp
// N ≤ 40程度で全探索したい時、前半・後半に分けて O(2^(N/2)) で計算
// 例: 部分集合の和の組み合わせを高速に求める
// 1. 前半の全部分集合の和をソートして持つ
// 2. 後半の各部分集合について、二分探索で条件を満たす相方を探す
```

### 8.6 累積XOR・bit演算の典型

```cpp
// 部分配列のXOR = 累積XORの差分
vector<ll> xorPrefix(n + 1, 0);
rep(i, n) xorPrefix[i+1] = xorPrefix[i] ^ a[i];
// 区間[l, r)のXOR = xorPrefix[r] ^ xorPrefix[l]

// 1の立っている個数
int popcount = __builtin_popcount(x); // 32bit
int popcountll = __builtin_popcountll(x); // 64bit
```

### 8.7 グリッド系の典型（迷路探索など）

```cpp
int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};
// 範囲外チェックを忘れない
auto inGrid = [&](int x, int y) { return 0 <= x && x < h && 0 <= y && y < w; };
```

### 8.8 乱択・期待値・確率の注意

- 期待値の線形性を使うと、各事象を独立に計算して足すだけで済む場合が多い。
- 乱択アルゴリズムを使う場合、最悪ケースで詰むことがあるため `mt19937` を使い、シード固定でテストしておく。

```cpp
mt19937 rng(12345); // シード固定で再現性確保（本番はrandom_device推奨）
```

---

## 9. コンテスト運用Tips

### 9.1 早解き担当としての心構え

- **問題文を読む順番**: 制約 → 入出力例 → 本文。制約から想定解法の見当をつけてから読むと早い。
- 簡単そうな問題から通すのが基本だが、**チームメイトと「今どの問題に取り組んでいるか」を共有**して被りを避ける。
- サンプル一致だけで提出せず、**コーナーケース**（N=0, N=1, 全要素同じ、最大値最小値）を自分の頭で確認する。

### 9.2 デバッグの優先順位

1. **コンパイルエラー**: 型の不一致、テンプレートのインスタンス化エラーが多い。
2. **WA（不正解）**: まずサンプルが合っているか確認 → 想定解法自体が間違っていないか再考。
3. **RE（実行時エラー）**: 配列範囲外アクセス、ゼロ除算、再帰のスタックオーバーフローが多い。
4. **TLE**: 計算量の見積もりが正しいか確認。`O(N^2)` のつもりが `O(N^2 log N)` になっていないか等。

### 9.3 ICPC特有の制約

- **1台のPCをチームで共有**するため、誰がPCを使うかの取り決めを事前に決めておく。
- 提出はチームで1つのアカウントのみ。ペナルティ（誤答時の時間ペナルティ）に注意。
- プリンタが使える場合は紙に問題を印刷して、PCを使わずに考察する時間を作ると効率が良い。
- 自分のテンプレート・このチートシートを**印刷物として持ち込めるか事前にルールを確認**しておく（持ち込み可否はコンテストごとに異なる）。

### 9.4 提出前の最終チェックリスト

- [ ] 入出力の形式（スペース区切り、改行区切り）を問題文と一致させたか
- [ ] `long long` を使うべき箇所で `int` を使っていないか
- [ ] 配列サイズは十分か（添字が制約の最大値+1まで届くか）
- [ ] modが必要な問題で、引く操作の後に `+ mod` を忘れていないか
- [ ] マルチテストケース（複数クエリ）の場合、グローバル変数を**毎回リセット**しているか

---

*作成日: 2026年6月 / Maximumサークル ICPC対策用*

---

## 10. 追加典型

### 10.1 Suffix Array（SA-IS / O(N log N) 版）

```cpp
// O(N log N) の簡易実装（競プロではこれで十分なことが多い）
struct SuffixArray {
    int n;
    string s;
    vector<int> sa, rank_, tmp;

    SuffixArray(const string& s_) : s(s_), n(s_.size()), sa(s_.size()), rank_(s_.size()), tmp(s_.size()) {
        build();
    }

    void build() {
        iota(sa.begin(), sa.end(), 0);
        for (int i = 0; i < n; i++) rank_[i] = s[i];

        for (int k = 1; k < n; k <<= 1) {
            auto cmp = [&](int a, int b) {
                if (rank_[a] != rank_[b]) return rank_[a] < rank_[b];
                int ra = a + k < n ? rank_[a + k] : -1;
                int rb = b + k < n ? rank_[b + k] : -1;
                return ra < rb;
            };
            sort(sa.begin(), sa.end(), cmp);
            tmp[sa[0]] = 0;
            for (int i = 1; i < n; i++)
                tmp[sa[i]] = tmp[sa[i-1]] + (cmp(sa[i-1], sa[i]) ? 1 : 0);
            rank_ = tmp;
        }
    }

    // pat: 検索したいパターン文字列 → s中に存在すれば true
    // O(|pat| log N)
    bool contains(const string& pat) {
        int lo = 0, hi = n;
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (s.substr(sa[mid], pat.size()) < pat) lo = mid + 1;
            else hi = mid;
        }
        return lo < n && s.substr(sa[lo], pat.size()) == pat;
    }
};
```

**LCP配列（隣接するサフィックス間の最長共通接頭辞）と組み合わせるとさらに強力。**

```cpp
// Kasai法でLCP配列を O(N) で構築
// s: 元の文字列, sa: SuffixArrayのsa配列
// 戻り値: lcp[i] = sa[i-1]とsa[i]のサフィックスの最長共通接頭辞の長さ
vector<int> buildLCP(const string& s, const vector<int>& sa) {
    int n = s.size();
    vector<int> rank_(n), lcp(n, 0);
    for (int i = 0; i < n; i++) rank_[sa[i]] = i;
    int h = 0;
    for (int i = 0; i < n; i++) {
        if (rank_[i] > 0) {
            int j = sa[rank_[i] - 1];
            while (i + h < n && j + h < n && s[i + h] == s[j + h]) h++;
            lcp[rank_[i]] = h;
            if (h > 0) h--;
        }
    }
    return lcp;
}
```

**典型的な用途:**
- 文字列中の異なる部分文字列の数 = `N*(N+1)/2 - sum(lcp)`
- 最長共通部分文字列（複数文字列を番兵文字で連結してSA+LCPを構築）
- `lcp` + sparse table でLCP(i, j)をO(1)クエリ

---

### 10.2 ローリングハッシュ（先頭N文字を取る版）

既存の `RollingHash` を拡張して、先頭 `n` 文字のハッシュ値を O(1) で取れるようにしたバージョン。

```cpp
struct RollingHash {
    vector<ll> hash1, hash2, pow1, pow2;
    const ll mod1 = 1000000007, mod2 = 998244353;
    const ll base1 = 131, base2 = 137; // 文字コード範囲より大きい素数推奨

    RollingHash(const string& s) {
        int n = s.size();
        hash1.assign(n + 1, 0); hash2.assign(n + 1, 0);
        pow1.assign(n + 1, 1);  pow2.assign(n + 1, 1);
        for (int i = 0; i < n; i++) {
            hash1[i+1] = (hash1[i] * base1 + s[i]) % mod1;
            hash2[i+1] = (hash2[i] * base2 + s[i]) % mod2;
            pow1[i+1]  = pow1[i]  * base1 % mod1;
            pow2[i+1]  = pow2[i]  * base2 % mod2;
        }
    }

    // l, r: 半開区間 [l, r)。戻り値: (hash1, hash2) のペア
    // 区間 [l, r) のハッシュ（汎用）
    pll get(int l, int r) {
        ll h1 = (hash1[r] - hash1[l] * pow1[r-l] % mod1 + mod1 * mod1) % mod1;
        ll h2 = (hash2[r] - hash2[l] * pow2[r-l] % mod2 + mod2 * mod2) % mod2;
        return {h1, h2};
    }

    // n: 先頭から取る文字数。戻り値: s[0, n) のハッシュペア
    // 先頭 n 文字のハッシュ（get(0, n) と同じだが意図を明示する版）
    pll prefix(int n) {
        return {hash1[n], hash2[n]};
    }

    // hs: 前の文字列のハッシュ, ht: 後ろの文字列のハッシュ
    // len_t: 後ろの文字列の長さ（pow のシフト量として使用）
    // 2つのハッシュを連結: hash(s) + hash(t) → hash(s+t)
    pll concat(pll hs, pll ht, int len_t) {
        ll h1 = (hs.first  * pow1[len_t] + ht.first)  % mod1;
        ll h2 = (hs.second * pow2[len_t] + ht.second) % mod2;
        return {h1, h2};
    }
};
```

**使用例: 最長共通接頭辞を二分探索で求める**

```cpp
// s[la:] と s[lb:] の最長共通接頭辞の長さ
// rh: 対象文字列のローリングハッシュ
// la, lb: 比較する2つのサフィックスの開始位置
// maxLen: LCPの探索上限（= min(s.size()-la, s.size()-lb) など）
// 戻り値: s[la:] と s[lb:] の最長共通接頭辞の長さ
int lcp(RollingHash& rh, int la, int lb, int maxLen) {
    int lo = 0, hi = maxLen;
    while (lo < hi) {
        int mid = (lo + hi + 1) / 2;
        if (rh.get(la, la + mid) == rh.get(lb, lb + mid)) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}
```

---

### 10.3 floor_sum — ∑⌊(a·i + b) / m⌋ を O(log m) で計算

```cpp
// sum_{i=0}^{n-1} floor((a*i + b) / m) を返す
// 制約: n, a, b >= 0, m >= 1
// a, b は m 以上でも可（内部で mod を取る）
// n: 和の項数（i=0,...,n-1）, a: 1次係数, b: 定数項, m: 除数
// 戻り値: sum_{i=0}^{n-1} floor((a*i + b) / m)
ll floor_sum(ll n, ll a, ll b, ll m) {
    ll ans = 0;
    // a >= m の部分を先に処理
    ans += (a / m) * (n - 1) * n / 2;
    a %= m;
    // b >= m の部分を先に処理
    ans += (b / m) * n;
    b %= m;
    // 残りの処理（ユークリッド的な再帰）
    ll y_max = (a * (n - 1) + b) / m;
    if (y_max == 0) return ans;
    ll x_max = y_max * m - b;
    ans += (n - (x_max + a - 1) / a) * y_max;
    ans += floor_sum(y_max, m, (a - x_max % a) % a, a);
    return ans;
}
```

**何に使うのか:**

| 場面 | 変換 |
|---|---|
| 格子点の数え上げ | 直線 y = (a/m)x + b/m の下の格子点数 = floor_sum(n, a, b, m) |
| 等差数列の整数除算の総和 | ∑⌊(a·i + b)/m⌋ をそのまま |
| 数論的関数の計算 | ∑_{i=1}^{N} ⌊N/i⌋ と組み合わせることも |

**計算量:** O(log m)（拡張ユークリッドと同じ縮小構造）

**検証用サンプル:**
```
n=4, a=3, b=2, m=5
i=0: floor(2/5)=0
i=1: floor(5/5)=1
i=2: floor(8/5)=1
i=3: floor(11/5)=2
合計 = 4 → floor_sum(4, 3, 2, 5) = 4
```