#include <iostream>
using namespace std;

long long GetNumOfNumbers(long long mid) {
    long long num_moos = mid / 3 + mid / 5 - mid / 15;
    return mid - num_moos;
}

int main() {
    // 여기에 코드를 작성해주세요.

    long long N;
    cin >> N;

    long long left = 1;
    long long right = INT32_MAX;
    long long min_num = INT32_MAX;
    
    while(left <= right) {
        long long mid = (left + right) / 2;
        if(GetNumOfNumbers(mid) >= N) {
            right = mid - 1;
            min_num = min(min_num, mid);
        } else {
            left = mid + 1;
        }
    }
    
    cout << min_num << '\n';

    return 0;
}