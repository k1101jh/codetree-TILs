#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int n, m;

bool valid(vector<int>& vec, int num) {
    int prev = vec[0];
    int num_objects = 1;
    for(int i=1; i < n; i++) {
        if(vec[i] - prev >= num) {
            prev = vec[i];
            num_objects++;
        }
    }
    return num_objects >= m;
}

int main() {
    // 여기에 코드를 작성해주세요.

    cin >> n >> m;
    vector<int> vec(n, 0);

    for(int i=0; i < n; i++) {
        cin >> vec[i];
    }

    sort(vec.begin(), vec.end());

    int left = 1;
    int right = 1000000000;
    int ans = 0;

    while(left <= right) {
        int mid = (left + right) / 2;
        
        if(valid(vec, mid)) {
            ans = max(ans, mid);
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    cout << ans << '\n';
    return 0;
}