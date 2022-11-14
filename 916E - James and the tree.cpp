//problem: https://codeforces.com/contest/916/problem/E?locale=en
//submission: https://codeforces.com/contest/916/submission/36314825

// Problem overview:
//   There is a rooted tree (connective graph without cycles). Every node contains a number.
//   Need to perform 3 operations:
//   1. Change the root of the tree to the given node
//   2. Given 2 nodes V, U. Need to find the Lowest Common Ancestor of V, U and add given number X to every node in its subtree
//   3. Find sum of values in given subtree

// Data structures and algorithms usage:
//   Segment Tree
//   Depth First Search
//   Lowest Common Ancestor

// Time complexities:
//   First operation is performed in O(1) time complexity
//   Operations 2 and 3 are performed in O(log N) time complexity

#include <iostream>
#include <vector>
#include <algorithm>
 
using namespace std;
 
typedef long long ll;
 
const ll SZ = 1e5 + 10;
const ll INF = 1e18;
 
struct node {
    node *left, *right;
    ll val, delta;
    ll vl, vr;
 
    void push()
    {
        if (left) {
            left -> val += delta * ((left -> vr) - (left -> vl));
            left -> delta += delta;
        }
        if (right) {
            right -> val += delta * ((right -> vr) - (right -> vl));
            right -> delta += delta;
        }
        delta = 0;
    }
 
    void add(ll L, ll R, ll x)
    {
        if (R <= vl || vr <= L)
            return;
        push();
        if (L <= vl && vr <= R) {
            val += x * (vr - vl);
            delta += x;
            push();
            return;
        }
        left -> add(L, R, x);
        right -> add(L, R, x);
        val = 0;
        if (left)
            val += left -> val;
        if (right)
            val += right -> val;
    }
 
    ll get_sum(ll L, ll R)
    {
        if (R <= vl || vr <= L)
            return 0LL;
        push();
        if (L <= vl && vr <= R)
            return val;
        return (left -> get_sum(L, R)) + (right -> get_sum(L, R));
    }
};
 
ll n, q;
vector<ll> vect;
vector<ll> graph[SZ];
 
//-------------SUBTREE OPERATIONS----------------------------------------
 
node* build_tree(ll L, ll R, vector<ll> &vect)
{
    if (R - L == 1)
        return new node({nullptr, nullptr, vect[L], 0, L, R});
    node *res = new node({nullptr, nullptr, 0, 0, L, R});
    ll mid = (L + R) / 2;
    res -> left = build_tree(L, mid, vect);
    res -> right = build_tree(mid, R, vect);
    res -> val = (res -> left -> val) + (res -> right -> val);
    return res;
}
 
vector<ll> vect_for_subtrees;
node *root_for_subtrees;
 
pair<ll, ll> fract[SZ];
 
void dfs_for_subtrees(ll from, ll v)
{
    fract[v].first = vect_for_subtrees.size();
    vect_for_subtrees.push_back(vect[v]);
    for (ll to : graph[v]) {
        if (to == from)
            continue;
        dfs_for_subtrees(v, to);
    }
    fract[v].second = vect_for_subtrees.size();
}
 
void build_all_for_subtrees()
{
    dfs_for_subtrees(0, 1);
    root_for_subtrees = build_tree(0, vect_for_subtrees.size(), vect_for_subtrees);
}
 
void add_to_subtree(ll v, ll x)
{
    root_for_subtrees -> add(fract[v].first, fract[v].second, x);
}
 
void add_to_vertex(ll v, ll x)
{
    root_for_subtrees -> add(fract[v].first, fract[v].first + 1, x);
}
 
ll get_sum_of_subtree(ll v)
{
    return root_for_subtrees -> get_sum(fract[v].first, fract[v].second);
}
 
ll get_val_of_vertex(ll v)
{
    return root_for_subtrees -> get_sum(fract[v].first, fract[v].first + 1);
}
 
//-----------ALL FOR LCA---------------------------------------------------------
 
vector<ll> vect_for_LCA;
ll high[SZ];
ll fir[SZ];
ll last[SZ];
 
ll tin[SZ];
ll tout[SZ];
ll t = 1;
 
pair<ll, ll> tree_for_LCA[SZ * 8];
ll sz_for_LCA = 1;
 
void dfs_for_LCA(ll from, ll v, ll h_now)
{
    tin[v] = t++;
    high[v] = h_now;
    fir[v] = vect_for_LCA.size();
    for (ll to : graph[v]) {
        if (to == from)
            continue;
        dfs_for_LCA(v, to, h_now + 1);
        vect_for_LCA.push_back(v);
    }
    last[v] = vect_for_LCA.size();
    vect_for_LCA.push_back(v);
    tout[v] = t++;
}
 
void build_tree_for_LCA()
{
    while (sz_for_LCA < vect_for_LCA.size())
        sz_for_LCA <<= 1;
    for (ll i = 0; i < vect_for_LCA.size(); i++)
        tree_for_LCA[sz_for_LCA + i] = {high[vect_for_LCA[i]], vect_for_LCA[i]};
    for (ll i = vect_for_LCA.size(); i < sz_for_LCA; i++)
        tree_for_LCA[sz_for_LCA + i] = {INF, INF};
    for (ll i = sz_for_LCA - 1; i > 0; i--)
        tree_for_LCA[i] = min(tree_for_LCA[i * 2], tree_for_LCA[i * 2 + 1]);
}
 
ll get_min_on_LCA_tree(ll L, ll R)
{
    L += sz_for_LCA;
    R += sz_for_LCA;
    pair<ll, ll> res = {INF, INF};
    while (L <= R) {
        if (L & 1)
            res = min(res, tree_for_LCA[L++]);
        if (~R & 1)
            res = min(res, tree_for_LCA[R--]);
        L >>= 1;
        R >>= 1;
    }
    return res.second;
}
 
void build_all_for_LCA()
{
    dfs_for_LCA(0, 1, 0);
    build_tree_for_LCA();
}
 
bool is_anc(ll v, ll u)
{
    return tin[v] <= tin[u] && tout[v] >= tout[u];
}
 
ll get_LCA(ll v, ll u)
{
    if (is_anc(v, u))
        return v;
    if (is_anc(u, v))
        return u;
    if (last[v] <= fir[u])
        return get_min_on_LCA_tree(last[v], fir[u]);
    return get_min_on_LCA_tree(last[u], fir[v]);
}
 
//-------------ADDITIONAL THINGS FOR FINDING NEEDED ANCESTOR------------------------------------------
 
ll get_next_anc(ll v, ll u)
{
    ll L = 0;
    ll R = graph[v].size() - 1;
    ll mid;
    while (L < R) {
        mid = (L + R + 1) / 2;
        if (tin[graph[v][mid]] <= tin[u])
            L = mid;
        else
            R = mid - 1;
    }
    return graph[v][L];
}
 
//-------------THE MAIN------------------------------------------------------
 
ll root_now = 1;
ll next_to_root;
 
bool cmp(ll a, ll b)
{
    return tin[a] < tin[b];
}
 
void build_all()
{
    build_all_for_subtrees();
    build_all_for_LCA();
    for (ll i = 1; i <= n; i++)
        sort(graph[i].begin(), graph[i].end(), cmp);
}
 
void first_operation(ll v)
{
    root_now = v;
}
 
void second_operation(ll v, ll u, ll x)
{
    ll m = get_LCA(v, u);
    if (is_anc(m, root_now)) {
        ll a = get_LCA(v, root_now);
        ll b = get_LCA(u, root_now);
        if (high[a] > high[b])
            m = a;
        else
            m = b;
    }
    if (m == root_now) {
        add_to_subtree(1, x);
        return;
    }
    if (is_anc(m, root_now)) {
        add_to_subtree(1, x);
        add_to_subtree(get_next_anc(m, root_now), -x);
        return;
    }
    add_to_subtree(m, x);
}
 
ll third_operation(ll v)
{
    if (v == root_now)
        return get_sum_of_subtree(1);
    if (is_anc(v, root_now))
        return get_sum_of_subtree(1) - get_sum_of_subtree(get_next_anc(v, root_now));
    return get_sum_of_subtree(v);
}
 
int main()
{
    //freopen("tickets.in", "r", stdin);
    //freopen("tickets.out", "w", stdout);
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> q;
    vect.push_back(0);
    for (ll i = 1; i <= n; i++) {
        ll a;
        cin >> a;
        vect.push_back(a);
    }
    for (ll i = 1; i < n; i++) {
        ll a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }
    build_all();
    for (ll i = 0; i < q; i++) {
        ll h;
        cin >> h;
        if (h == 1) {
            ll v;
            cin >> v;
            first_operation(v);
        }
        if (h == 2) {
            ll v, u;
            ll x;
            cin >> v >> u >> x;
            second_operation(v, u, x);
        }
        if (h == 3) {
            ll v;
            cin >> v;
            cout << third_operation(v) << "\n";
        }
    }
    return 0;
}