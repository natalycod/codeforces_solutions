// problem: https://codeforces.com/contest/813/problem/E?locale=en
// submission: https://codeforces.com/contest/813/submission/38689195

// Problem overview:
//   There is an array A with N numbers and number K.
//   Need to answer several queries:
//   Find the size of the biggest subset of the fraction [L; R] of the array A, where there is no subsubset of K+1 equal numbers.

// Data structures and algorithms usage:
//   Persistent Segment Tree

// Time complexities:
//   Operations are performed offline with O(log^2 (N)) time complexity for every query
//   Pre-calculation is performed in O(N log N) time complexity

#include <iostream>
#include <map>
 
using namespace std;
 
typedef long long ll;
 
const int SZ = 1e5 + 10;
 
struct node {
    node *left, *right;
    int vl, vr;
    int val;
 
    ~node() {
        delete left;
        delete right;
    }
 
    void upd(int ind, int x)
    {
        if (ind < vl || vr <= ind)
            return;
        if (vr - vl == 1) {
            val = x;
            return;
        }
        left -> upd(ind, x);
        right -> upd(ind, x);
        val = left -> val + right -> val;
    }
 
    int get_sum(int L, int R)
    {
        if (R <= vl || vr <= L)
            return 0;
        if (L <= vl && vr <= R)
            return val;
        return left -> get_sum(L, R) + right -> get_sum(L, R);
    }
};
 
node* build_empty_tree(int L, int R)
{
    node *now = new node({nullptr, nullptr, L, R, 0});
    if (R - L == 1)
        return now;
    int mid = (L + R) / 2;
    now -> left = build_empty_tree(L, mid);
    now -> right = build_empty_tree(mid, R);
    return now;
}
 
node* build_new_tree(node *v, int ind)
{
    node *now = new node({v -> left, v -> right, v -> vl, v -> vr, v -> val});
    if (v -> vr - v -> vl == 1)
        return now;
    int mid = (v -> vl + v -> vr) / 2;
    if (ind < mid)
        now -> left = build_new_tree(v -> left, ind);
    else
        now -> right = build_new_tree(v -> right, ind);
    return now;
}
 
int n, k;
int arr[SZ];
int next_ind[SZ];
 
map<int, int> last_ind;
 
node* tree[SZ];
 
void build_next()
{
    map<int, int> now;
    for (int i = n - 1; i >= 0; i--) {
        if (!now.count(arr[i]))
            next_ind[i] = n;
        else
            next_ind[i] = now[arr[i]];
        now[arr[i]] = i;
    }
}
 
void build_tree_0()
{
    map<int, int> cnt;
    tree[0] = build_empty_tree(0, n);
    for (int i = 0; i < n; i++) {
        if (cnt[arr[i]] < k) {
            tree[0] -> upd(i, 1);
            cnt[arr[i]]++;
            last_ind[arr[i]] = i;
        }
    }
}
 
void build_tree_i(int ind)
{
    node *now = build_new_tree(tree[ind - 1], ind - 1);
    now -> upd(ind - 1, 0);
    int nxt = next_ind[last_ind[arr[ind - 1]]];
    if (nxt != n) {
        node *now_now = build_new_tree(now, nxt);
        swap(now, now_now);
        now -> upd(nxt, 1);
        last_ind[arr[nxt]] = nxt;
    }
    tree[ind] = now;
}
 
void print_do(node *v)
{
    if (v -> vr - v -> vl == 1) {
        cout << v -> val << ' ';
        return;
    }
    print_do(v -> left);
    print_do(v -> right);
}
 
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> k;
    for (int i = 0; i < n; i++)
        cin >> arr[i];
    build_next();
    build_tree_0();
    for (int i = 1; i < n; i++)
        build_tree_i(i);
 
    int q;
    cin >> q;
    int last = 0;
    for (int i = 0; i < q; i++) {
        int L, R;
        cin >> L >> R;
        L = (L + last) % n;
        R = (R + last) % n;
        if (L > R)
            swap(L, R);
        last = tree[L] -> get_sum(L, R + 1);
        cout << last << "\n";
    }
    return 0;
}