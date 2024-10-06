#include <iostream>
#include <vector>
#include <queue>
#include <list>
using namespace std;

struct Consumer;

int n, m;

int dy[4] = {-1, 0, 0, 1};
int dx[4] = {0, -1, 1, 0};

vector<vector<int>> board;
vector<pair<int, int>> stores;
vector<vector<int>> dists;
list<pair<int, int>> positions;
list<pair<int, int>> basecamps;

void bfs(pair<int, int> pos) {
    // 편의점 좌표를 받아와서 각 타일까지의 거리 계산
    
    queue<pair<pair<int, int>, int>> q;
    q.push({pos, 0});

    dists = vector<vector<int>>(n, vector<int>(n, INT32_MAX));
    dists[pos.first][pos.second] = 0;

    while(!q.empty()) {
        pair<int, int> cur = q.front().first;
        int dist = q.front().second;
        q.pop();

        for(int i=0; i < 4; i++) {
            pair<int, int> next = {cur.first + dy[i], cur.second + dx[i]};

            if(next.first < 0 || next.first >= n || next.second < 0 || next.second >= n) continue;
            if(board[next.first][next.second] == INT32_MAX) continue;
            if(dists[next.first][next.second] != INT32_MAX) continue;

            dists[next.first][next.second] = dist + 1;
            q.push({next, dist + 1});
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> n >> m;
    board = vector<vector<int>>(n, vector<int>(n, 0));
    dists = vector<vector<int>>(n, vector<int>(n, 0));

    for(int i=0; i < n; i++) {
        for(int j=0; j < n; j++) {
            cin >> board[i][j];
            if(board[i][j] == 1) {
                basecamps.push_back({i, j});
            }
        }
    }

    int y, x;
    for(int i=0; i < m; i++) {
        cin >> y >> x;
        stores.push_back({y - 1, x - 1});
    }

    int time = 0;
    int completed_cnt = 0;

    list<pair<pair<int, int>, pair<int, int>>> consumers_list;

    while(completed_cnt < m) {
        // 1단계: 편의점 방향으로 움직이기
        // 편의점을 중심으로 bfs를 해서 각 좌표에 편의점까지의 거리를 기록
        // 거리가 짧아지는 방향으로 이동
        for(auto& consumer_info : consumers_list) {
            // cur_pos를 참조로 바꾸면 오류 발생
            // consumer_info.first = next 부분에서 cur_pos = next가 되는 문제 발생
            pair<int, int> cur_pos = consumer_info.first;
            pair<int, int>& dest = consumer_info.second;
            bfs(dest);

            int min_dist = INT32_MAX;
            for(int i=0; i < 4; i++) {
                pair<int, int> next = {cur_pos.first + dy[i], cur_pos.second + dx[i]};

                if(next.first < 0 || next.first >= n || next.second < 0 || next.second >= n) continue;
                if(board[next.first][next.second] == INT32_MAX) continue;
            
                if(dists[next.first][next.second] < min_dist) {
                    min_dist = dists[next.first][next.second];
                    consumer_info.first = next;
                }
            }
        }

        // 2단계: 편의점에 도착했는지 확인하기
        for(auto iter=consumers_list.begin(); iter != consumers_list.end();) {
            if(iter->first == iter->second) {
                completed_cnt++;
                board[iter->first.first][iter->first.second] = INT32_MAX;
                iter = consumers_list.erase(iter);
            } else {
                iter++;
            }
        }

        // 3단계: time에 맞춰 새로운 사람 데려오기
        if(time < m) {
            pair<int, int>& dest = stores[time];
            bfs(dest);

            // 가장 가까운 basecamp 찾기
            int min_dist = INT32_MAX;
            pair<int, int> closest_basecamp;

            for(int i=n - 1; i >= 0; i--) {
                for(int j=n - 1; j >= 0; j--) {
                    if(board[i][j] == 1 && dists[i][j] <= min_dist) {
                        min_dist = dists[i][j];
                        closest_basecamp.first = i;
                        closest_basecamp.second = j;
                    }
                }
            }

            board[closest_basecamp.first][closest_basecamp.second] = INT32_MAX;
            consumers_list.push_back({closest_basecamp, dest});
        }

        // 시간 증가
        time++;
    }

    cout << time << '\n';

    return 0;
}