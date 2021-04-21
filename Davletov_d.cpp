#include <bits/stdc++.h>
using namespace std;

#define int long long
#define mk make_pair
#define fr first
#define sc second
#define pb push_back
#define pi pair < int, int >
#define vi vector < int >

const int N = 3e5 + 7;
const int INF = 1e9 + 7;

int a[N];
int n, k;

int check (int d){
    int r = 1, res = 0;
    for (int i = 1; i <= n; i++){
        while (a[r + 1] - a[i] <= d){
            r++;
        }
        res += r - i;
    }
    return res;
}

main(){
    cin >> n;
    k = n * (n - 1) / 2;
    for (int i = 1; i <= n; i++)
        cin >> a[i];
    sort(a + 1, a + n + 1);
    a[n + 1] = 2 * INF + 7;
    int l = -1, r = INF;
    while (l + 1 < r){
        int mid = (l + r) >> 1;
        if (check(mid) < (k + 1) / 2)
            l = mid;
        else
            r = mid;
    }
    cout << l + 1;
}

