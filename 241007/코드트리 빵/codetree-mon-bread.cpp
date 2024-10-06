#include <iostream>
#include <vector>
#include <queue>
#include <list>
using namespace std;

struct Consumer;

int n, m;

int dy[4] = {-1, 0, 0, 1};
int dx[4] = {0, -1, 1, 0};

// 사람들이 대기하는 큐
queue<pair<int, int>> waiting_q;
vector<vector<int>> board;
vector<vector<int>> visited;

struct Consumer {
public:
    Consumer() {}
    Consumer(pair<int, int> cur_pos, pair<int, int> dest, int bit)
    : cur_pos(cur_pos), dest(dest), bit(bit)
    {
    }
    pair<int, int> cur_pos;
    pair<int, int> dest;
    int bit;
};

// 현 시점에서 가장 가까운 basecamp 찾아내기
pair<int, int> get_nearest_basecamp(pair<int, int>& pos, int bit) {
    vector<vector<int>> visited_copy(visited);
    queue<pair<int, int>> q2;

    q2.push(pos);

    while(!q2.empty()) {
        pair<int, int> cur = q2.front();
        q2.pop();

        for(int i=0; i < 4; i++) {
            pair<int, int> next = {cur.first + dy[i], cur.second + dx[i]};
            if(next.first >= 0 && next.first < n && next.second >= 0 && next.second < n
            && (visited_copy[next.first][next.second] & bit) == 0) {
                // basecamp를 찾아냈으면 반환
                if(board[next.first][next.second] == 1) {
                    return next;
                }
                visited_copy[next.first][next.second] |= bit;
                q2.push(next);
            }
        }
    }

    return {-1, -1};
}

int bfs() {
    queue<pair<Consumer, int>> q;
    // i에 해당하는 사람이 방문하면 (1 << i)를 & 하기
    visited = vector<vector<int>>(n, vector<int>(n, false));

    int time = 1;
    int num_complete = 0;
    int complete_bit = 0;
    int consumer_cnt = 1;

    // 맨 처음 사람
    pair<int, int> dest = waiting_q.front();
    waiting_q.pop();
    int cur_bit = (1 << consumer_cnt);
    pair<int, int> start_camp = get_nearest_basecamp(dest, cur_bit);

    // 이 basecamp를 다른 사람이 사용하지 못하도록 방문처리
    visited[start_camp.first][start_camp.second] = INT32_MAX;
    q.push({Consumer(start_camp, dest, cur_bit), 1});
    consumer_cnt++;

    Consumer consumer;

    // 모든 사람이 목적지에 도착할 때까지 수행
    while(num_complete < m) {
        // 3단계 수행
        // 큐에서 가져오지 말고, 새로 들어온 사람에 대해 bfs 수행
        // time=n인 사람이 대기중일 때, n-1 번째 사람이 새로 들어와야 함
        if((q.empty() || time == consumer_cnt + 1) && !waiting_q.empty()) {
            dest = waiting_q.front();
            waiting_q.pop();

            cur_bit = (1 << consumer_cnt);
            start_camp = get_nearest_basecamp(dest, cur_bit);
            visited[start_camp.first][start_camp.second] = INT32_MAX;

            consumer = Consumer(start_camp, dest, cur_bit);
            time = consumer_cnt;
            consumer_cnt++;
        } else {
            consumer = q.front().first;
            time = q.front().second;
            q.pop();
        }

        // 현재 consumer가 이미 편의점에 도착한 상태라면
        // 건너뛰기
        if((complete_bit & consumer.bit) > 0) {
            continue;
        }

        // 목적지에 도착했는지 확인하기
        if(consumer.cur_pos == consumer.dest) {
            visited[consumer.cur_pos.first][consumer.cur_pos.second] = INT32_MAX;
            num_complete++;
            complete_bit |= consumer.bit;
            // 모든 사람이 다 목적지에 도착했으면
            if(num_complete == m) {
                return time;
            }
            continue;
        }

        for(int i=0; i < 4; i++) {
            pair<int, int> next = {consumer.cur_pos.first + dy[i], consumer.cur_pos.second + dx[i]};
            if(next.first >= 0 && next.first < n && next.second >= 0 && next.second < n
            && (visited[next.first][next.second] & consumer.bit) == 0) {
                visited[next.first][next.second] |= consumer.bit;
                q.push({Consumer(next, consumer.dest, consumer.bit), time + 1});
            }
        }
    }

    return time;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> n >> m;
    board = vector<vector<int>>(n, vector<int>(n, 0));

    for(int i=0; i < n; i++) {
        for(int j=0; j < n; j++) {
            cin >> board[i][j];
        }
    }

    int y, x;
    for(int i=0; i < m; i++) {
        cin >> y >> x;
        waiting_q.push({y - 1, x - 1});
    }

    cout << bfs() << '\n';

    return 0;
}