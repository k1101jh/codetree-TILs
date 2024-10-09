#include <iostream>
#include <vector>
using namespace std;

int main() {
    // 여기에 코드를 작성해주세요.

    int N;
    cin >> N;

    vector<int> vec(N, 0);

    for(int i=0; i < N; i++) {
        cin >> vec[i];
    }

    int ans = 0;

    for(int i=0; i < N; i++) {
        for(int j=i + 1; j < N; j++) {
            if(vec[i] > vec[j]) {
                swap(vec[i], vec[j]);
                ans++;
            }
        }
    }

    cout << ans << '\n';

    return 0;
}