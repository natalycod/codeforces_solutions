//problem: https://codeforces.com/contest/1019/problem/D?locale=en
//submission: https://codeforces.com/contest/1019/submission/41631502

// Problem overview:
//   There are N points.
//   Need to find triangle with the area equal to given number S (or say that there is no triangle with such area)

// Data structures and algorithms usage:
//   Geometry basics
//   Binary search

// Time complexities:
//   Triangle is found in O(N^2) time complexity

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
 
using namespace std;
 
typedef long double ld;
typedef long long ll;
 
const int INF = 1e9 + 10;
const int SZ = 2e3 + 10;
const ld eps = 1e-8;
 
struct point {
    ld x, y;
    int id;
 
    ld len()
    {
        return sqrt(x * x + y * y);
    }
    ld len(const point &other)
    {
        return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
    }
 
    ld angle()
    {
        return atan2(y, x);
    }
 
    point operator -(const point &other) const
    {
        return {x - other.x, y - other.y, -1};
    }
 
};
 
struct line {
    ld a, b, c;
    ld angle;
    int id1, id2;
 
    bool operator <(const line &other) const
    {
        return angle < other.angle;
    }
};
 
bool is_equal(ld a, ld b)
{
    return abs(a - b) < eps;
}
 
bool is_less(ld a, ld b)
{
    return a < b && !is_equal(a, b);
}
 
bool is_great(ld a, ld b)
{
    return is_less(b, a);
}
 
line get_line(point A, point B)
{
    if (B.y < A.y)
        swap(A, B);
    else if (B.y == A.y && B.x < A.x)
        swap(A, B);
    line res;
    res.a = B.y - A.y;
    res.b = A.x - B.x;
    res.c = B.x * A.y - A.x * B.y;
    res.id1 = A.id;
    res.id2 = B.id;
    res.angle = (B - A).angle();
    return res;
}
 
ld get_dist(point A, line L)
{
    return (L.a * A.x + L.b * A.y + L.c) / sqrt(L.a * L.a + L.b * L.b);
}
 
ld lie_dist(point A, line L)
{
    return L.a * A.x + L.b * A.y;
}
 
line zero;
 
bool cmp(point A, point B)
{
    return lie_dist(A, zero) < lie_dist(B, zero);
}
 
int n;
ld S;
vector<line> quest;
vector<point> vect;
int pos[SZ];
 
point BinSearch(int ind, ld d)
{
    int L = 0, R = vect.size() - 1;
    while (L < R) {
        int mid = (L + R) / 2;
        if (is_less(get_dist(vect[mid], quest[ind]), d))
            L = mid + 1;
        else
            R = mid;
    }
    return vect[L];
}
 
void swap_plz(int ind, int id1, int id2)
{
    int pos1 = pos[id1];
    int pos2 = pos[id2];
    if (pos1 > pos2)
        swap(pos1, pos2);
    if (is_great(lie_dist(vect[pos1], quest[ind]), lie_dist(vect[pos2], quest[ind]))) {
        swap(vect[pos1], vect[pos2]);
        swap(pos[id1], pos[id2]);
    }
}
 
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> S;
    for (int i = 0; i < n; i++) {
        point now;
        cin >> now.x >> now.y;
        now.id = i;
        vect.push_back(now);
    }
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++)
            quest.push_back(get_line(vect[i], vect[j]));
    }
    sort(quest.begin(), quest.end());
    zero = quest[0];
    sort(vect.begin(), vect.end(), cmp);
    for (int i = 0; i < vect.size(); i++)
        pos[vect[i].id] = i;
    int L = 0;
    for (int i = 0; i < quest.size(); i++) {
        while (!is_equal(quest[L].angle, quest[i].angle)) {
            swap_plz(i, quest[L].id1, quest[L].id2);
            L++;
        }
        point A = vect[pos[quest[i].id1]];
        point B = vect[pos[quest[i].id2]];
        ld h = S * 2 / A.len(B);
        point now = BinSearch(i, h);
        if (is_equal(get_dist(now, quest[i]), h)) {
            cout << "Yes\n";
            cout << (ll)A.x << ' ' << (ll)A.y << "\n";
            cout << (ll)B.x << ' ' << (ll)B.y << "\n";
            cout << (ll)now.x << ' ' << (ll)now.y << "\n";
            return 0;
        }
        now = BinSearch(i, -h);
        if (is_equal(get_dist(now, quest[i]), h)) {
            cout << "Yes\n";
            cout << (ll)A.x << ' ' << (ll)A.y << "\n";
            cout << (ll)B.x << ' ' << (ll)B.y << "\n";
            cout << (ll)now.x << ' ' << (ll)now.y << "\n";
            return 0;
        }
    }
    cout << "No";
    return 0;
}