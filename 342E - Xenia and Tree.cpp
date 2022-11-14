// problem: https://codeforces.com/contest/342/problem/E?locale=en
// submission: https://codeforces.com/contest/342/submission/36279219

// Problem overview:
//   There is a tree (connective graph without cycles). First node is red, others are blue.
//   Need to make 2 operations:
//   1. Paint given blue node in red color
//   2. Calculate the closest red node to the given one (print the distance)

// Data structures and algorithms usage:
//   Centroid Decomposition
//   Lowest Common Ancestor
//   Depth First Search

// Time complexities:
//   Both operations are performed in O(log^2 (N)) time complexity
//   Pre-calculation is performed in O(N) time complexity

#include <iostream>
#include <vector>
#include <algorithm>
 
using namespace std;
 
const int SZ = 2e5 + 10;
const int INF = 1e9;
 
int n, m;
vector<int> graph[SZ];
 
int size_of[SZ];
int max_size[SZ];
 
bool used[SZ];
int pr[SZ];
 
//------------FOR CENTROID DECOMPOSITION------------------------------
 
void dfs_for_calc(int from, int v, int all_size)
{
    size_of[v] = 1;
    max_size[v] = 0;
    for (auto to : graph[v]) {
        if (used[to] || to == from)
            continue;
        dfs_for_calc(v, to, all_size);
        size_of[v] += size_of[to];
        max_size[v] = max(max_size[v], size_of[to]);
    }
    max_size[v] = max(max_size[v], all_size - size_of[v]);
}
 
int find_centroid(int from, int v, int all_size)
{
    if (max_size[v] <= all_size / 2)
        return v;
    for (int to : graph[v]) {
        if (used[to] || to == from)
            continue;
        int u = find_centroid(v, to, all_size);
        if (u != 0)
            return u;
    }
    return 0;
}
 
void build_centroid(int from, int v, int all_size)
{
    int now = find_centroid(from, v, all_size);
    pr[now] = from;
    used[now] = true;
    dfs_for_calc(0, now, 0);
    for (int to : graph[now]) {
        if (used[to])
            continue;
        build_centroid(now, to, size_of[to]);
    }
}
 
//---------LCA BUILDING--------------------------------------
 
vector<int> vect;
pair<int, int> tree[SZ * 8];
int fir[SZ];
int last[SZ];
int sz = 1;
 
int high[SZ];
 
void dfs_for_ejler(int from, int v, int h_now)
{
    high[v] = h_now;
    fir[v] = vect.size();
    vect.push_back(v);
    for (int to : graph[v]) {
        if (to == from)
            continue;
        dfs_for_ejler(v, to, h_now + 1);
        vect.push_back(v);
    }
    last[v] = vect.size();
    vect.push_back(v);
}
 
void build_tree()
{
    sz = 1;
    while (sz < vect.size())
        sz <<= 1;
    for (int i = 0; i < vect.size(); i++)
        tree[sz + i] = {high[vect[i]], vect[i]};
    for (int i = sz - 1; i > 0; i--)
        tree[i] = min(tree[i * 2], tree[i * 2 + 1]);
}
 
pair<int, int> get_min(int L, int R)
{
    L += sz;
    R += sz;
    pair<int, int> res = {INF, 0};
    while (L <= R) {
        if (L & 1)
            res = min(res, tree[L++]);
        if (~R & 1)
            res = min(res, tree[R--]);
        L >>= 1;
        R >>= 1;
    }
    return res;
}
 
void build_LCA()
{
    dfs_for_ejler(0, 1, 0);
    build_tree();
}
 
int get_LCA(int v, int u)
{
    if (fir[v] < fir[u] && last[v] > last[u])
        return v;
    if (fir[u] < fir[v] && last[u] > last[v])
        return u;
    if (last[v] > fir[u])
        swap(v, u);
    return get_min(last[v], fir[u]).second;
}
 
int get_dist(int v, int u)
{
    int mid = get_LCA(v, u);
    return high[v] + high[u] - 2 * high[mid];
}
 
//--------------THE MAIN PROBLEM-----------------------------------
 
int next_red[SZ];
 
void make_red(int v)
{
    next_red[v] = 0;
    int u = v;
    while (pr[u]) {
        u = pr[u];
        next_red[u] = min(next_red[u], get_dist(v, u));
    }
}
 
int find_red(int v)
{
    int u = v;
    int res = next_red[u];
    while (pr[u]) {
        u = pr[u];
        res = min(res, next_red[u] + get_dist(v, u));
    }
    return res;
}
 
int main()
{
    //freopen("tickets.in", "r", stdin);
    //freopen("tickets.out", "w", stdout);
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 1; i < n; i++) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }
    dfs_for_calc(0, 1, n);
    build_centroid(0, 1, n);
    build_LCA();
    for (int i = 1; i <= n; i++)
        next_red[i] = INF;
    make_red(1);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        if (a == 1)
            make_red(b);
        else
            cout << find_red(b) << "\n";
    }
    return 0;
}