//problem C: https://codeforces.com/gym/100140/attachments
//submission: https://codeforces.com/gym/100140/submission/35121041

#include <iostream>
#include <vector>
#include <queue>
 
using namespace std;
 
typedef long long ll;
 
const ll INF = 1e9;
const ll SZ = 110;
 
struct edge {
    ll from, to, cap, flow, id;
};
 
ll S, T;
ll n, m;
vector<edge> e;
vector<ll> graph[SZ];
 
ll d[SZ];
ll now_ind[SZ];
 
void add_edge(ll from, ll to, ll cap)
{
    edge now = {from, to, cap, 0, e.size()};
    graph[from].push_back(e.size());
    e.push_back(now);
    now = {to, from, cap, 0, (ll)e.size()};
    graph[to].push_back(e.size());
    e.push_back(now);
}
 
bool bfs()
{
    queue<ll> q;
    fill(d + 1, d + n + 1, -1);
    d[T] = 0;
    q.push(T);
    while (!q.empty()) {
        ll v = q.front();
        q.pop();
        for (ll u : graph[v]) {
            if (d[e[u].to] == -1 && e[u ^ 1].flow < e[u ^ 1].cap) {
                d[e[u].to] = d[v] + 1;
                q.push(e[u].to);
            }
        }
    }
    return d[S] != -1;
}
 
ll dfs(ll v, ll flow)
{
    if (v == T || !flow)
        return flow;
    while (now_ind[v] < graph[v].size()) {
        edge now = e[graph[v][now_ind[v]]];
        if (d[now.to] == d[v] - 1 && now.cap > now.flow) {
            ll pushed = dfs(now.to, min(flow, now.cap - now.flow));
            if (pushed != 0) {
                e[now.id].flow += pushed;
                e[(now.id ^ 1)].flow -= pushed;
                return pushed;
            }
        }
        now_ind[v]++;
    }
    return 0;
}
 
ll action()
{
    ll res = 0;
    while (bfs()) {
        fill(now_ind + 1, now_ind + n + 1, 0);
        while (true) {
            ll ans = dfs(1, INF);
            if (ans == 0)
                break;
            res++;
        }
    }
    return res;
}
 
void upd_new()
{
    for (int i = 0; i < e.size(); i++) {
        e[i].cap = 1;
        e[i].flow = 0;
    }
}
 
int main()
{
    //ios_base::sync_with_stdio(false);
    //cin.tie(nullptr);
    cin >> n >> m;
    for (ll i = 0; i < m; i++) {
        ll a, b;
        cin >> a >> b;
        add_edge(a, b, 1);
    }
    ll my_res = INF;
    S = 1;
    for (T = 2; T <= n; T++) {
        upd_new();
        my_res = min(my_res, action());
    }
    cout << my_res;
    return 0;
}