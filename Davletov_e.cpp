#include <bits/stdc++.h>
using namespace std;

#define all(s) s.begin(), s.end()
#define ll long long
#define mk make_pair
#define fr first
#define sc second
#define pb push_back
#define pi pair < int, int >
#define vi vector < int >

const int N = 4e5 + 7;
const int SZ = 1e9;
const int INF = 1e9 + 7;

struct vil {
    int a, b, c;
} v[N];

bool comp (vil x, vil y){
    return x.a < y.a;
}

struct query {
    int pos, ind, x, y;
};

bool comp2 (query a, query b){
    return a.pos < b.pos;
}

int t[16 * N], lf[16 * N], rg[16 * N], cnt = 1;
int ans[N];
int n, m;

vector < query > q;

void upd (int v, int l, int r, int pos, int val){
    if (l == r)
        t[v] ^= val;
    else {
        int mid = (l + r) >> 1;
        if (!lf[v]){
            lf[v] = ++cnt;
            rg[v] = ++cnt;
        }
        if (pos <= mid)
            upd(lf[v], l, mid, pos, val);
        else
            upd(rg[v], mid + 1, r, pos, val);
        t[v] = t[lf[v]] ^ t[rg[v]];
    }
}

int get (int v, int l, int r, int ql, int qr){
    if (ql <= l && r <= qr)
        return t[v];
    if (r < ql || qr < l || !lf[v])
        return 0;
    int mid = (l + r) >> 1;
    return get(lf[v], l, mid, ql, qr) ^ get(rg[v], mid + 1, r, ql, qr);
}

main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> m;
    for (int i = 1; i <= n; i++){
        cin >> v[i].a >> v[i].b >> v[i].c;
    }
    v[n + 1].a = INF;
    sort(v + 1, v + n + 1, comp);
    for (int i = 1; i <= m; i++){
        int l, r, x, y;
        cin >> l >> r >> x >> y;
        if (l != 1)
            q.pb({l - 1, i, x, y});
        q.pb({r, i, x, y});
    }
    sort(all(q), comp2);
    int k = 1;
    for (query it : q){
        while (v[k].a <= it.pos){
            upd(1, 1, SZ, v[k].b, v[k].c);
            k++;
        }
        ans[it.ind] ^= get(1, 1, SZ, it.x, it.y);
    }
    for (int i = 1; i <= m; i++)
        cout << ans[i] << endl;
}

//4 3
//1 2 3
//2 2 5
//1 5 7
//4 2 4
//2 4 2 2
//1 2 1 2
//2 2 2 5
