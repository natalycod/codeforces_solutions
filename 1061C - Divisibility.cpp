//problem: https://codeforces.com/contest/1061/problem/C
//submission: https://codeforces.com/contest/1061/submission/46083391

#include <iostream>
#include <vector>
 
using namespace std;
 
typedef long long ll;
 
const ll SZ = 2e6 + 10;
const ll MOD = 1e9 + 7;
 
ll n;
ll arr[SZ];
 
ll ans[SZ];
ll sieve[SZ];
 
void build_sieve()
{
    ll mx = 2e6;
    for (ll i = 2; i <= mx; i++) {
        if (sieve[i])
            continue;
        for (ll j = i; j <= mx; j += i)
            sieve[j] = i;
    }
}
 
vector<pair<ll, ll> > vect;
vector<ll> dels;
 
void build_del(ll id = 0, ll numb = 1)
{
    if (id == vect.size()) {
        dels.push_back(numb);
        return;
    }
    build_del(id + 1, numb);
    for (ll i = 0; i < vect[id].second; i++) {
        numb *= vect[id].first;
        build_del(id + 1, numb);
    }
}
 
void find_dels(ll x)
{
    vect.clear();
    dels.clear();
    while (x != 1) {
        if (vect.empty() || vect.back().first != sieve[x])
            vect.push_back({sieve[x], 0});
        vect.back().second++;
        x /= sieve[x];
    }
    build_del();
}
 
signed main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    build_sieve();
    cin >> n;
    for (ll i = 1; i <= n; i++) {
        cin >> arr[i];
        find_dels(arr[i]);
        vector<pair<ll, ll> > murr;
        for (auto x : dels) {
            if (x == 1) {
                murr.push_back({1, 1});
                continue;
            }
            murr.push_back({x, ans[x - 1]});
        }
        for (auto h : murr) {
            ans[h.first] += h.second;
            ans[h.first] %= MOD;
        }
    }
    ll res = 0;
    for (ll i = 1; i <= n; i++) {
        res += ans[i];
        res %= MOD;
    }
    cout << res;
    return 0;
}