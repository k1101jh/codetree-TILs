#include <iostream>
#include <vector>
using namespace std;

int main() {
    // 여기에 코드를 작성해주세요.
    string A, B;
    cin >> A >> B;

    vector<int> alp_idx_vec(26, 0);

    for(int i=0; i < A.length(); i++) {
        alp_idx_vec[A[i] - 'a'] = i;
    }

    int ans = 1;
    int j = 0;
    int prev = B[j];
    for(j = 1; j < B.length(); j++) {
        if(alp_idx_vec[B[j]] <= prev) {
            ans++;
        }
        prev = B[j];
    }

    cout << ans << '\n';

    return 0;
}