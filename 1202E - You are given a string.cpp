//problem: https://codeforces.com/contest/1202/problem/E
//submission: https://codeforces.com/contest/1202/submission/58677177

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
 
using namespace std;
 
const long long SZ = 2e5 + 10;
const long long ALPHA_SZ = 30;
 
struct node {
    node* next_let[ALPHA_SZ];
    node *link;
    long long cnt;
 
    node()
    {
        for (long long i = 0; i < ALPHA_SZ; i++)
            next_let[i] = nullptr;
        link = nullptr;
        cnt = 0;
    }
};
 
node* build_fish(vector<string> &vect)
{
    node *fict = new node();
    node *root = new node();
    for (long long i = 0; i < ALPHA_SZ; i++)
        fict -> next_let[i] = root;
    root -> link = fict;
 
    for (string s : vect) {
        node *now = root;
        for (long long i = 0; i < s.size(); i++) {
            long long c = s[i] - 'a';
            if (!now -> next_let[c])
                now -> next_let[c] = new node();
            now = now -> next_let[c];
        }
        now -> cnt++;
    }
    queue<pair<node*, char> > q;
    for (long long c = 0; c < ALPHA_SZ; c++) {
        if (root -> next_let[c])
            q.push({root, c});
    }
    while (!q.empty()) {
        node *v = q.front().first;
        long long c = q.front().second;
        q.pop();
        node *now = v -> next_let[c];
        v = v -> link;
        while (!v -> next_let[c])
            v = v -> link;
        v = v -> next_let[c];
        now -> link = v;
        now -> cnt += now -> link -> cnt;
        for (long long i = 0; i < ALPHA_SZ; i++) {
            if (now -> next_let[i])
                q.push({now, i});
        }
    }
    return root;
}
 
long long n;
string t;
vector<string> vect;
 
pair<long long, long long> ans[SZ];
 
void read_all()
{
    cin >> t >> n;
    for (long long i = 0; i < n; i++) {
        string s;
        cin >> s;
        vect.push_back(s);
    }
}
 
signed main()
{
    //ios_base::sync_with_stdio(false);
    //cin.tie(nullptr);
    read_all();
    node *now = build_fish(vect);
    for (long long i = 0; i < t.size(); i++) {
        long long c = t[i] - 'a';
        while (!now -> next_let[c])
            now = now -> link;
        now = now -> next_let[c];
        ans[i].second = now -> cnt;
    }
 
    for (long long i = 0; i < n; i++) {
        string S;
        for (char c : vect[i])
            S = c + S;
        vect[i] = S;
    }
    now = build_fish(vect);
    for (long long i = t.size() - 1; i >= 0; i--) {
        long long c = t[i] - 'a';
        while (!now -> next_let[c])
            now = now -> link;
        now = now -> next_let[c];
        ans[i].first = now -> cnt;
    }
    long long res = 0;
    for (long long i = 0; i + 1 < t.size(); i++)
        res += ans[i].second * ans[i + 1].first;
    cout << res;
    return 0;
}
