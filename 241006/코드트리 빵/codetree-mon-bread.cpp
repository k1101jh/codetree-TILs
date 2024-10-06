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
vector<pair<int, int>> stores_vec;
list<pair<int, int>> basecamp_list;

queue<pair<Consumer, int>> q;

struct Consumer {
public:
    Consumer(pair<int, int> cur_pos, pair<int, int> dest, int bit)
    : cur_pos(cur_pos), dest(dest), bit(bit)
    {
    }
    pair<int, int> cur_pos;
    pair<int, int> dest;
    int bit;
};

int calc_l1_dist(pair<int, int>& p1, pair<int, int>& p2) {
    return abs(p1.first - p2.first) + abs(p1.second - p2.second);
}

void set_start_basecamp() {
    for(int i=0; i < stores_vec.size(); i++) {
        int best_dist = INT32_MAX;
        pair<int, int>& store = stores_vec[i];
        vector<list<pair<int, int>>::iterator> candidate_camps;
        
        // 거리가 가장 짧은 basecamp들 찾기
        for(auto iter=basecamp_list.begin(); iter != basecamp_list.end(); iter++) {
            // 가게와 basecamp 거리 계산
            int dist = calc_l1_dist(store, *iter);

            // 새로 계산한 거리가 최소거리면 후보군 비운다음 현재 basecamp를 후보군에 추가
            if(best_dist > dist) {
                candidate_camps = vector<list<pair<int, int>>::iterator>();
                candidate_camps.push_back(iter);
                best_dist = dist;
            // 새로 계산한 거리와 최소거리가 같으면 현재 basecamp를 후보군에 추가
            } else if(best_dist == dist) {
                candidate_camps.push_back(iter);
            }
        }

        // 거리가 짧은 basecamp 중 행이 작거나, 행이 같으면 열이 작은 캠프 찾기

        // 거리가 짧은 basecamp가 하나만 있는 경우
        if(candidate_camps.size() == 1) {    
            q.push({Consumer(*candidate_camps[0], store, (1 << i)), i + 1});
            basecamp_list.erase(candidate_camps[0]);
        // 거리가 짧은 basecamp가 여럿 있는 경우
        } else {
            list<pair<int, int>>::iterator best_camp = candidate_camps[0];
            for(int j=1; j < candidate_camps.size(); j++) {
                if(candidate_camps[j]->first < best_camp->first) {
                    best_camp = candidate_camps[j];
                } else if(candidate_camps[j]->first == best_camp->first && candidate_camps[j]->second < best_camp->second) {
                    best_camp = candidate_camps[j];
                }
            }
            q.push({Consumer(*best_camp, store, (1 << i)), i + 1});
            basecamp_list.erase(best_camp);
        }
    }
}

int bfs() {
    // i에 해당하는 사람이 방문하면 (1 << i)를 & 하기
    vector<vector<int>> visited(n, vector<int>(n, false));

    int time = 0;
    int prev_bit = 0;
    int human_bit = 0;
    int num_complete = 0;
    int complete_bit = 0;

    while(!q.empty()) {
        Consumer consumer = q.front().first;
        time = q.front().second;
        q.pop();

        // 현재 consumer가 처음 등장한 경우면
        // basecamp에 위치해 있으므로 해당 basecamp를 봉쇄
        if((human_bit & consumer.bit) ==  0) {
            visited[consumer.cur_pos.first][consumer.cur_pos.second] = INT32_MAX;
            human_bit |= consumer.bit;
        }

        // 현재 consumer가 이미 편의점에 도착한 상태라면
        // 건너뛰기
        if((complete_bit & consumer.bit) > 0) {
            continue;
        }

        for(int i=0; i < 4; i++) {
            pair<int, int> next = {consumer.cur_pos.first + dy[i], consumer.cur_pos.second + dx[i]};
            if(next.first >= 0 && next.first < n && next.second >= 0 && next.second < n
            && (visited[next.first][next.second] & consumer.bit) == 0) {
                // 다음 위치가 목적지가 아니면
                if(next != consumer.dest) {
                    visited[next.first][next.second] |= consumer.bit;
                    q.push({Consumer(next, consumer.dest, consumer.bit), time + 1});
                // 다음 위치가 목적지면
                } else {
                    visited[next.first][next.second] = INT32_MAX;
                    num_complete++;
                    complete_bit |= consumer.bit;
                    // 모든 사람이 다 목적지에 도착했으면
                    if(num_complete == m) {
                        return time + 1;
                    }
                }
            }
        }
    }

    return time + 1;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> n >> m;

    board = vector<vector<int>>(n, vector<int>(n, 0));
    stores_vec = vector<pair<int, int>>(m, {0, 0});
    basecamp_list = list<pair<int, int>>();

    for(int i=0; i < n; i++) {
        for(int j=0; j < n; j++) {
            cin >> board[i][j];
            if(board[i][j] == 1) {
                basecamp_list.push_back({i, j});
            }
        }
    }

    for(int i=0; i < m; i++) {
        cin >> stores_vec[i].first >> stores_vec[i].second;
        stores_vec[i].first--;
        stores_vec[i].second--;
    }

    // 가장 가까운 베이스캠프 찾기
    set_start_basecamp();

    cout << bfs() << '\n';

    return 0;
}