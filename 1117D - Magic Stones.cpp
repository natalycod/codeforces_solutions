//problem: https://codeforces.com/contest/1117/problem/D
//submission: https://codeforces.com/contest/1117/submission/50126520

#include <iostream>
#include <vector>
 
using namespace std;
 
const long long MOD = 1e9 + 7;
 
struct matrix {
    long long n, k;
    vector<vector<long long> > matr;
 
    matrix(long long a, long long b)
    {
        n = a, k = b;
        matr.resize(a);
        for (long long i = 0; i < a; i++)
            matr[i].resize(b);
    }
 
    matrix operator *(matrix &other)
    {
        matrix res = matrix(this -> n, other.k);
        for (long long i = 0; i < res.n; i++) {
            for (long long j = 0; j < res.k; j++) {
                res.matr[i][j] = 0;
                for (long long c = 0; c < k; c++) {
                    res.matr[i][j] += matr[i][c] * other.matr[c][j];
                    res.matr[i][j] %= MOD;
                }
            }
        }
        return res;
    }
 
    void print()
    {
        for (long long i = 0; i < n; i++) {
            for (long long j = 0; j < k; j++)
                cout << matr[i][j] << ' ';
            cout << "\n";
        }
        cout << "\n";
    }
};
 
matrix get_step(matrix now, long long st)
{
    matrix one = matrix(now.n, now.k);
    for (long long i = 0; i < one.n; i++) {
        for (long long j = 0; j < one.k; j++)
            one.matr[i][j] = (i == j ? 1 : 0);
    }
    while (st > 1) {
        if (st & 1)
            one = one * now;
        now = now * now;
        st >>= 1;
    }
    return now * one;
}
 
long long n, m;
 
matrix read_matr()
{
    cin >> n >> m;
    matrix now(m, 1);
    for (long long i = 0; i < m; i++)
        now.matr[i][0] = 1;
    return now;
}
 
matrix get_mult_matrix()
{
    matrix now(m, m);
    for (long long i = 0; i < m - 1; i++) {
        for (long long j = 0; j < m; j++)
            now.matr[i][j] = (i == j - 1 ? 1 : 0);
    }
    now.matr[m - 1][0] = now.matr[m - 1][m - 1] = 1;
    for (long long i = 1; i < m - 1; i++)
        now.matr[m - 1][i] = 0;
    return now;
}
 
signed main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    matrix now = read_matr();
    matrix mult = get_mult_matrix();
    matrix res = get_step(mult, n);
    res = res * now;
    cout << res.matr[0][0];
    return 0;
}