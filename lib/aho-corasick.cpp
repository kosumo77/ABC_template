// ============================================================
// Aho-Corasick法（複数パターン文字列の同時マッチング）
// ============================================================
// 概要:
//   パターン集合 {T_1, T_2, ..., T_k} を trie 木にまとめ、
//   failure link（KMPのprefix functionの多パターン版）を張ることで、
//   テキストSを1回スキャンするだけで全パターンの出現位置を
//   O(|S| + sum|T_i| + マッチ数) で列挙できる。
//
// 使い方:
//   AhoCorasick ac;
//   ac.add_pattern("aba");      // パターンを追加（複数回呼べる）
//   ac.add_pattern("bab");
//   ac.build();                 // 追加が終わったら必ず呼ぶ
//   auto res = ac.search(S);    // res[i] = Sのi文字目で終わるパターンのid一覧
//
// 計算量:
//   構築: O(sum|T_i| * Σ)  (Σ=文字種数。26文字なら実質O(sum|T_i|))
//   検索: O(|S| + Z)  (Zはマッチの総数)
//
// 引数の意味:
//   add_pattern(string t)
//     t     : 登録したいパターン文字列
//     返り値: 登録したパターンのid（0-indexed, 追加順）
//
//   build()
//     引数なし。add_patternを全て呼んだ後に1回だけ呼ぶこと。
//     内部でBFSしてfailure linkを構築する。
//
//   search(string s)
//     s     : 検索対象のテキスト
//     返り値: vector<vector<int>> res
//             res[i] には「sの0-indexedでi文字目を末尾として
//             一致が終了したパターンのid」が全て入る
//             （s[i - len(T_id) + 1 ..= i] == T_id）
//
//   count_matches(string s)
//     s     : 検索対象のテキスト
//     返り値: 各パターンidごとの出現回数 vector<long long>
//             （同じパターンが複数回出現してもOK、重複含む出現数を数える）
// ============================================================

#include <bits/stdc++.h>
using namespace std;

struct AhoCorasick {
    static const int ALPHABET = 26;      // 文字種数（英小文字想定。必要に応じて変更）
    static const char BASE_CHAR = 'a';   // 文字種の基準文字

    struct Node {
        array<int, ALPHABET> next;  // trie上の子（goto関数。構築後は自動遷移込み）
        int fail = 0;               // failure link先のノードid
        int depth = 0;              // ルートからの深さ（＝対応する文字列の長さ）
        vector<int> pat_ids;        // このノードで終端となるパターンのid一覧
        Node() { next.fill(-1); }
    };

    vector<Node> nodes;
    int pattern_count = 0;

    AhoCorasick() {
        nodes.emplace_back(); // root = node 0
    }

    // パターンを1つ登録する。返り値はパターンid（0-indexed）
    int add_pattern(const string& t) {
        int cur = 0;
        for (char c : t) {
            int ci = c - BASE_CHAR;
            if (nodes[cur].next[ci] == -1) {
                nodes[cur].next[ci] = (int)nodes.size();
                nodes.emplace_back();
                nodes.back().depth = nodes[cur].depth + 1;
            }
            cur = nodes[cur].next[ci];
        }
        nodes[cur].pat_ids.push_back(pattern_count);
        return pattern_count++;
    }

    // failure linkの構築（BFS: 浅いノードから確定させる必要があるため）
    void build() {
        queue<int> q;
        // 深さ1のノード（rootの直接の子）はfailがrootで確定
        for (int c = 0; c < ALPHABET; c++) {
            if (nodes[0].next[c] == -1) {
                nodes[0].next[c] = 0; // rootで見つからない文字はrootに留まる（番兵）
            } else {
                nodes[nodes[0].next[c]].fail = 0;
                q.push(nodes[0].next[c]);
            }
        }
        while (!q.empty()) {
            int v = q.front(); q.pop();
            // 親のfailで登録されているpat_idsを継承（"aba"にマッチしたら"a"にもマッチした扱い、
            // のように失敗リンクを辿った先のマッチも全部拾いたい場合に使う）
            for (int id : nodes[nodes[v].fail].pat_ids) nodes[v].pat_ids.push_back(id);

            for (int c = 0; c < ALPHABET; c++) {
                int u = nodes[v].next[c];
                if (u == -1) {
                    // 子がない場合はgoto関数をfailの遷移先で埋める（トリック：これで検索時の
                    // failループが不要になり、常にO(1)で次状態が求まる）
                    nodes[v].next[c] = nodes[nodes[v].fail].next[c];
                } else {
                    nodes[u].fail = nodes[nodes[v].fail].next[c];
                    q.push(u);
                }
            }
        }
    }

    // テキストsを検索し、各位置で終端となるパターンidの一覧を返す
    // res[i] : sの i 文字目(0-indexed)で一致が終わったパターンのid一覧
    vector<vector<int>> search(const string& s) {
        vector<vector<int>> res(s.size());
        int cur = 0;
        for (int i = 0; i < (int)s.size(); i++) {
            int ci = s[i] - BASE_CHAR;
            cur = nodes[cur].next[ci]; // O(1)遷移（buildでgoto化済み）
            res[i] = nodes[cur].pat_ids;
        }
        return res;
    }

    // 各パターンの出現回数（重複・オーバーラップ含む）をまとめて数える
    vector<long long> count_matches(const string& s) {
        vector<long long> cnt(pattern_count, 0);
        int cur = 0;
        for (char c : s) {
            int ci = c - BASE_CHAR;
            cur = nodes[cur].next[ci];
            for (int id : nodes[cur].pat_ids) cnt[id]++;
        }
        return cnt;
    }
};

// ============================================================
// 使用例
// ============================================================
/*
int main() {
    AhoCorasick ac;
    ac.add_pattern("aba");   // id=0
    ac.add_pattern("bab");   // id=1
    ac.add_pattern("ab");    // id=2
    ac.build();

    string s = "ababab";
    auto res = ac.search(s);
    for (int i = 0; i < (int)s.size(); i++) {
        for (int id : res[i]) {
            cout << "pos=" << i << " pattern_id=" << id << "\n";
        }
    }

    auto cnt = ac.count_matches(s);
    for (int id = 0; id < (int)cnt.size(); id++) {
        cout << "pattern " << id << ": " << cnt[id] << " matches\n";
    }
}
*/