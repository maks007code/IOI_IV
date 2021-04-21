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

const int N = 1e5 + 7;
const int INF = 1e9 + 7;

pi q[4 * N];
int p[N][18], k[4 * N];
int tin[N], tout[N], rev[N], tim;
int a[N], d[N];
int n, m, cnt;

vi g[N];

void dfs (int v, int pr = 0){
    p[v][0] = pr;
    for (int j = 1; j < 18; j++)
        p[v][j] = p[p[v][j - 1]][j - 1];
//    cout << v << endl;
    tin[v] = ++tim;
    //cout << v << ' ' << tin[v] << endl;
    rev[tin[v]] = v;
    d[v] = d[pr] + 1;
    for (int to : g[v])
        if (to != pr)
            dfs(to, v);
    tout[v] = tim;
}

bool check (int v, int u){
    return tin[v] <= tin[u] && tout[v] >= tout[u];
}

int lca (int v, int u){
    if (check(v, u))
        return v;
    if (check(u, v))
        return u;
    for (int j = 17; j >= 0; j--)
        if (p[v][j] && !check(p[v][j], u))
            v = p[v][j];
    return p[v][0];
}

int lca_ch (int v, int u){
    for (int j = 17; j >= 0; j--)
        if (p[v][j] && !check(p[v][j], u))
            v = p[v][j];
    return v;
}

void build (int v, int l, int r){
    if (l == r)
        q[v] = {INF, rev[l]};
    else {
        int mid = (l + r) >> 1;
        build(v + v, l, mid);
        build(v + v + 1, mid + 1, r);
        if (q[v + v].fr < q[v + v + 1].fr)
            q[v] = q[v + v];
        else
            q[v] = q[v + v + 1];
    }
}

void upd_ver (int v, int l, int r, int pos, int val, int inc){
    if (l == r){
        //cout << q[v].fr << ' ' << q[v].sc << endl;
        q[v].fr = val;
        k[v] += inc;
//        cout << v << ' ' << l << ' ' << r << ' ' << pos << ' ' << val << ' ' << inc << endl;
    }
    else {
        int mid = (l + r) >> 1;
        if (pos <= mid)
            upd_ver(v + v, l, mid, pos, val, inc);
        else
            upd_ver(v + v + 1, mid + 1, r, pos, val, inc);
        if (q[v + v].fr < q[v + v + 1].fr)
            q[v] = q[v + v];
        else
            q[v] = q[v + v + 1];
        k[v] = k[v + v] + k[v + v + 1];
    }
}

pair < pi, int > get_dw (int v, int l, int r, int ql, int qr){
    if (ql <= l && r <= qr)
        return {q[v], k[v]};
    if (r < ql || qr < l)
        return {{INF, 0}, 0};
    int mid = (l + r) >> 1;
    auto t1 = get_dw(v + v, l, mid, ql, qr);
    auto t2 = get_dw(v + v + 1, mid + 1, r, ql, qr);
    if (t1.fr.fr < t2.fr.fr)
        return {t1.fr, t1.sc + t2.sc};
    else
        return {t2.fr, t1.sc + t2.sc};
}

int last;

bool state (int u){
    if (a[u])
        return true;
    auto t_u = get_dw(1, 1, n, tin[u], tout[u]);
    int mn_u = t_u.fr.sc;
    int dw_u = t_u.sc;
    last = dw_u;
    if (!dw_u)
        return false;
    if (cnt - dw_u)
        return true;
    int ch_u = lca_ch(mn_u, u);
    //out << t_u.fr.fr << endl;
    if (dw_u - get_dw(1, 1, n, tin[ch_u], tout[ch_u]).sc)
        return true;
    return false;
}

int find_black (int v, int u){
    if (state(v))
        return v;
    if (state(u))
        return u;
    int l = v, r = u;
    for (int j = 17; j >= 0; j--){
        if (!p[l][j] || check(p[l][j], r))
            continue;
        if (state(p[l][j]))
            return p[l][j];
        else if (last)
            r = p[l][j];
        else
            l = p[l][j];
    }
    return -1;
}

int solve (int v, int u){
    int x = 0, y = 0;
    int st = find_black(v, u);
    if (u == v){
        if (state(v))
            return 1;
        return 0;
    }
    //cout << v << ' ' << u << ' ' << st << endl;
    if (st == -1)
        return 0;
    if (st != v){
        if (state(v))
            x = d[v];
        else {
            int l = v, r = st;
            for (int j = 17; j >= 0; j--){
                if (!p[l][j] || check(p[l][j], r))
                    continue;
                if (state(p[l][j]))
                    r = p[l][j];
                else
                    l = p[l][j];
            }
            x = d[r];
        }
    }
    else
        x = d[v];
    if (st != u){
        if (state(u))
            y = d[u];
        else {
            int l = st, r = u;
            for (int j = 17; j >= 0; j--){
                if (!p[l][j] || check(p[l][j], r))
                    continue;
                if (state(p[l][j]))
                    l = p[l][j];
                else
                    r = p[l][j];
            }
            y = d[l];
        }
    }
    else
        y = d[u];
    return abs(x - y) + 1;
}

main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cin >> n;
    for (int i = 1; i < n; i++){
        int a, b;
        cin >> a >> b;
        g[a].pb(b);
        g[b].pb(a);
    }
    dfs(1);
    build(1, 1, n);
    cin >> m;
    for (int i = 1; i <= m; i++){
        int t, x, y;
        cin >> t;
        if (t == 1){
            cin >> x;
            if (!a[x]){
                upd_ver(1, 1, n, tin[x], d[x], 1);
                cnt++;
            }
            else {
                upd_ver(1, 1, n, tin[x], INF, -1);
                cnt--;
            }
            a[x] ^= 1;
        }
        else {
            cin >> x >> y;
            int l = lca(x, y);
            cout << solve(x, l) + solve(y, l) - state(l) << endl;
        }
    }
    //cout << get_dw(1, 1, n, tin[3], tout[3]).fr.fr << ' ' << get_dw(1, 1, n, tin[3], tout[3]).fr.sc << ' ' << get_dw(1, 1, n, tin[3], tout[3]).sc << endl;
}

//6
//3 5
//5 6
//5 1
//3 4
//4 2
//2
//1 5
//2 1 3

