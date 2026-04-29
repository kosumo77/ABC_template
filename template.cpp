#include <bits/stdc++.h>
#include <atcoder/all>
#define rep(i,m,n) for (int i = m; i < (n); i++)
#define REP(i,m,n) for (int i = m; i <= (n); i++)
#define all(a) a.begin(),a.end()
using namespace std;
using ll = long long;
using pii = pair<int,int>;
using pll = pair<ll,ll>;
using vi = vector<int>;
using vvi = vector<vector<int>>;
using vl = vector<ll>;
using vvl = vector<vector<ll>>;
using vc = vector<char>;
using vvc = vector<vector<char>>;
using vb = vector<bool>;
using vvb = vector<vb>;
constexpr array<int, 4> dx = {0, 0, 1, -1};
constexpr array<int, 4> dy = {1, -1, 0, 0};

// Input functions
template<typename T>
void in(T& x) {
    cin >> x;
}
template<typename T, typename... Args>
void in(T& x, Args&... args) {
    cin >> x;
    in(args...);
}
template<typename T>
void in(vector<T>& v) {
    for (auto& x : v) {
        in(x);
    }
}

// Output functions
void out() {
    // Do nothing for empty out() call without newline
}
template<typename T>
void out(T x) {
    cout << x;
}
template<typename T, typename... Args>
void out(T x, Args... args) {
    cout << x << " ";
    out(args...);
}
template<typename T>
void out(const vector<T>& v, int start_index = 0) {
    for (int i = start_index; i < v.size(); i++) {
        cout << v[i] << (i == v.size() - 1 ? "" : " ");
    }
}

void outln() {
    cout << endl;
}
template<typename T>
void outln(T x) {
    cout << x << endl;
}
template<typename T, typename... Args>
void outln(T x, Args... args) {
    cout << x << " ";
    outln(args...);
}
template<typename T>
void outln(const vector<T>& v, int start_index = 0) {
    for (int i = start_index; i < v.size(); i++) {
        cout << v[i] << (i == v.size() - 1 ? "" : " ");
    }
    cout << endl;
}
template <typename T>
inline vector<vector<T>> make_vec2(const size_t H, const size_t W, const T& init) {
    return vector<vector<T>>(H, vector<T>(W, init));
}

template <typename T>
inline vector<vector<T>> make_vec2(const size_t H, const size_t W) {
    return vector<vector<T>>(H, vector<T>(W));
}

void solve() {
    
}

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    solve();
    return 0;
}