//problem D: https://codeforces.com/gym/101284/attachments
//submission: https://codeforces.com/gym/101284/submission/35415520

#include <iostream>
#include <vector>
#include <algorithm>
 
using namespace std;
 
typedef long long ll;
 
struct node {
    node *left, *right;
    int x, y;
    int cnt;
};
 
int get_cnt(node *v)
{
    if (!v)
        return 0;
    return v -> cnt;
}
 
void upd_cnt(node *v)
{
    v -> cnt = get_cnt(v -> left) + get_cnt(v -> right) + 1;
}
 
pair<node*, node*> split_dt(node *v, int k)
{
    if (!v)
        return {nullptr, nullptr};
    pair<node*, node*> res;
    if (get_cnt(v -> left) < k) {
        res = split_dt(v -> right, k - get_cnt(v -> left) - 1);
        v -> right = res.first;
        upd_cnt(v);
        res.first = v;
        return res;
    }
    res = split_dt(v -> left, k);
    v -> left = res.second;
    upd_cnt(v);
    res.second = v;
    return res;
}
 
pair<node*, node*> split_for_max(node *v, int x)
{
    if (!v)
        return {nullptr, nullptr};
    pair<node*, node*> res;
    if ((v -> x) <= x) {
        res = split_for_max(v -> right, x);
        v -> right = res.first;
        upd_cnt(v);
        res.first = v;
        return res;
    }
    res = split_for_max(v -> left, x);
    v -> left = res.second;
    upd_cnt(v);
    res.second = v;
    return res;
}
 
node* merge_dt(node *L, node *R)
{
    if (!L)
        return R;
    if (!R)
        return L;
    if (L -> y > R -> y) {
        L -> right = merge_dt(L -> right, R);
        upd_cnt(L);
        return L;
    }
    R -> left = merge_dt(L, R -> left);
    upd_cnt(R);
    return R;
}
 
node* add_node(node *v, int x)
{
    pair<node*, node*> t = split_for_max(v, x);
    node *now = new node({nullptr, nullptr, x, rand(), 1});
    t.first = merge_dt(t.first, now);
    return v = merge_dt(t.first, t.second);
}
 
int get_kth_max(node *v, int k)
{
    pair<node*, node*> t = split_dt(v, get_cnt(v) - k);
    node *now = t.second;
    while (now -> left)
        now = now -> left;
    int ans = now -> x;
    v = merge_dt(t.first, t.second);
    return ans;
}
 
bool cmp(pair<int, int> a, pair<int, int> b)
{
    return a.second < b.second;
}
 
int n, m;
vector<pair<int, int> > vect;
 
int main()
{
    freopen("pencils.in", "r", stdin);
    freopen("pencils.out", "w", stdout);
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        int k;
        cin >> k;
        int min_now, max_now;
        cin >> min_now;
        max_now = min_now;
        for (int i = 1; i < k; i++) {
            int a;
            cin >> a;
            min_now = min(min_now, a);
            max_now = max(max_now, a);
        }
        vect.push_back({min_now, max_now});
    }
    sort(vect.begin(), vect.end(), cmp);
    if (m == 1) {
        int res = 1e9;
        for (auto h : vect)
            res = min(res, h.second - h.first);
        cout << res;
        return 0;
    }
    node *root = nullptr;
    int res = 1e9;
    for (auto h : vect) {
        if (get_cnt(root) >= m - 1) {
            res = min(res, h.second - min(get_kth_max(root, m - 1), h.first));
        }
        root = add_node(root, h.first);
    }
    cout << res;
    return 0;
}