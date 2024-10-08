#include <iostream>
#include <vector>
using namespace std;

int getM(vector<int>& vec, int k) {
    int num = 0;
    for(auto& element : vec) {
        num += element / k;
    }
    return num;
}

int main() {
    // 여기에 코드를 작성해주세요.
    int n, m;
    cin >> n >> m;

    vector<int> vec(n, 0);

    for(int i=0; i < n; i++) {
        cin >> vec[i];
    }

    int left = 1;
    int right = 100000;

    int answer = 0;

    while(left <= right) {
        int mid = (left + right) / 2;
        int cur_m = getM(vec, mid);
        if(cur_m >= m) {
            answer = max(answer, mid);
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    cout << answer << '\n';

    return 0;
}