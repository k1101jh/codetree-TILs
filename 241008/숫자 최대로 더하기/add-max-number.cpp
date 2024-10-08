#include <iostream>
#include <vector>
using namespace std;

int main() {
    // 하나에 몰빵하는게 좋음
    // max값 제외하고 다 더한 다음 1/2 곱해서 max값에 더하기
    int n;
    cin >> n;

    vector<int> vec(n, 0);

    int max_val = 0;
    int sum = 0;
    for(int i=0; i < n; i++) {
        cin >> vec[i];
        max_val = max(max_val, vec[i]);
        sum += vec[i];
    }

    double ans = (double)(sum - max_val) / 2.0 + max_val;

    cout << fixed;
    cout.precision(1);
    cout << ans << '\n';

    return 0;
}