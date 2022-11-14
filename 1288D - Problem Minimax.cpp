//task: https://codeforces.com/contest/1288/problem/D
//submission: https://codeforces.com/contest/1288/submission/68793393

#include <iostream>
#include <vector>
 
using namespace std;
 
const int M_SZ = 1e3;
const long long MOD = 1e9 + 7;
 
int n, m;
vector<vector<int>> vect;
 
void read_all() {
    cin >> n >> m;
    vect.resize(n);
    for (int i = 0; i < n; i++) {
        vect[i].resize(m);
        for (int j = 0; j < m; j++)
            cin >> vect[i][j];
    }
}
 
int masks[M_SZ];
 
void build_masks(int k) {
    for (int mask = 0; mask < (1 << m); mask++)
        masks[mask] = -1;
    for (int i = 0; i < n; i++) {
        int now = 0;
        for (int j = 0; j < m; j++) {
            now <<= 1;
            if (vect[i][j] >= k)
                now++;
        }
        masks[now] = i;
    }
}
 
pair<int, int> check() {
    for (int mask1 = 0; mask1 < (1 << m); mask1++) {
        for (int mask2 = 0; mask2 < (1 << m); mask2++) {
            if (masks[mask1] == -1 || masks[mask2] == -1)
                continue;
            if ((mask1 | mask2) == (1 << m) - 1)
                return {masks[mask1], masks[mask2]};
        }
    }
    return {-1, -1};
}
 
void BinSearch() {
    int L = 0, R = 1e9;
    while (L < R) {
        int mid = (L + R + 1) / 2;
        build_masks(mid);
        pair<int, int> now = check();
        if (now.first == -1)
            R = mid - 1;
        else
            L = mid;
    }
    build_masks(L);
    pair<int, int> now = check();
    cout << now.first + 1 << ' ' << now.second + 1;
}
 
int main()
{
    read_all();
    BinSearch();
    return 0;
}