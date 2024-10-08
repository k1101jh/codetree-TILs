#include <iostream>
using namespace std;


long long n, k;

long long GetNumSmaller(long long a) {
    long long num = 0;

    for(int i=1; i <= n; i++) {
        num += min(n, a / i);
    }

    return num;
}

int main() {

    cin >> n >> k;

    long long left = 1;
    long long right = n * n;
    long long ans = n * n;

    while(left <= right) {
        long long mid = (left + right) / 2;
        if(GetNumSmaller(mid) >= k) {
            right = mid - 1;
            ans = min(ans, mid);
        } else {
            left = mid + 1;
        }
    }

    cout << ans << '\n';

    return 0;
}