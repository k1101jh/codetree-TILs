#include <iostream>
#include <vector>
#include <map>
#include <queue>
using namespace std;

// 좌표가 위아래로 뒤집힌 것으로 생각하기
// 세로 길이에 3을 추가한 forest 생성하기


int R, C, K;

enum Direction {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3,
};

// 북 동 남 서
int dy[4] = {1, 0, -1, 0};
int dx[4] = {0, 1, 0, -1};

// 이동 우선순위
// 남 서 동
Direction move_priority[3] = {NORTH, WEST, EAST};

// 골렘 생김새
// 북, 동, 남, 서, 가운데
pair<int, int> golem[5] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {0, 0}};

// 방향에 따라 확인이 필요한 블록들
map<Direction, vector<Direction>> blocks_to_check = {{SOUTH, {EAST, SOUTH, WEST}}, {WEST, {NORTH, WEST, SOUTH}}, {EAST, {NORTH, EAST, SOUTH}}};

vector<vector<int>> forest;
vector<vector<bool>> is_exit;

bool MoveInDirection(pair<int, int> base_pos, Direction direction) {
    // 골렘이 특정 방향으로 한 칸 이동이 가능한지 검사

    for(int i: blocks_to_check[direction]) {
        pair<int, int> next_pos = {base_pos.first + golem[i].first + dy[direction], base_pos.second + golem[i].second + dx[direction]};

        // 숲을 벗어났는지 검사
        if(next_pos.first < 0 || next_pos.second < 0 || next_pos.second >= C) return false;
        // 다른 골렘이 있는지 검사
        if(forest[next_pos.first][next_pos.second] != 0) return false;
    }
    return true;
}

int bfs(pair<int, int> start) {
    vector<vector<bool>> visited(R, vector<bool>(C, false));
    queue<pair<pair<int, int>, int>> q;
    q.push({start, forest[start.first][start.second]});

    int min_y = R;

    while(!q.empty()) {
        pair<int, int> cur = q.front().first;
        int cur_val = q.front().second;
        q.pop();

        min_y = min(min_y, cur.first);        

        for(int i=0; i < 4; i++) {
            pair<int, int> next = {cur.first + dy[i], cur.second + dx[i]};

            if(next.first < 0 || next.first >= R || next.second < 0 || next.second >= C) continue;
            if(visited[next.first][next.second]) continue;

            int next_val = forest[next.first][next.second];
            if(next_val != cur_val && !is_exit[cur.first][cur.second]) continue;

            visited[next.first][next.second] = true;
            q.push({next, next_val});
        }
    }

    return R - min_y;
}

void print_forest() {
    for(int i=R - 1; i >= 0; i--) {
        for(int j=0; j < C; j++) {
            cout << forest[i][j] << ' ';
        }
        cout << '\n';
    }
}

int MoveGolem(int id, int c, int d) {
    bool moved = true;

    pair<int, int> base_pos = {R + 1, c};

    // 이전에 이동을 했고, 바닥에 도달하지 않았으면 계속 이동 시도
    while(moved && base_pos.first - 1 > 0) {
        moved = false;

        // 남쪽 검사
        if(MoveInDirection(base_pos, SOUTH)) {
            moved = true;
            base_pos.first--;
        } else if(MoveInDirection(base_pos, WEST) && MoveInDirection({base_pos.first, base_pos.second - 1}, SOUTH)) {
            moved = true;
            base_pos.first--;
            base_pos.second--;
            d = (d + 3) % 4;
        } else if(MoveInDirection(base_pos, EAST) && MoveInDirection({base_pos.first, base_pos.second + 1}, SOUTH)) {
            moved = true;
            base_pos.first--;
            base_pos.second++;
            d = (d + 1) % 4;
        }
    }

    // 골렘 위치 검사
    // 맨 위 블럭만 검사하면 됨
    if(base_pos.first + 1 >= R) {
        // 숲 초기화
        forest = vector<vector<int>>(R + 3, vector<int>(C, 0));
        is_exit = vector<vector<bool>>(R + 3, vector<bool>(C, false));
        return 0;
    } else {
        // 숲에 골렘 기록
        for(int i=0; i < 5; i++) {
            pair<int, int> golem_block = {base_pos.first + golem[i].first, base_pos.second + golem[i].second};
            forest[golem_block.first][golem_block.second] = id;
        }

        // 출구 위치 기록
        pair<int, int> exit_pos = {base_pos.first + golem[d].first, base_pos.second + golem[d].second};
        is_exit[exit_pos.first][exit_pos.second] = true;

        // 정령 도달 위치 반환
        return bfs(base_pos);
    }
}


int main() {
    int c, d;
    cin >> R >> C >> K;

    forest = vector<vector<int>>(R + 3, vector<int>(C, 0));
    is_exit = vector<vector<bool>>(R + 3, vector<bool>(C, false));
    int ans = 0;

    for(int i=1; i <= K; i++) {
        cin >> c >> d;

        ans += MoveGolem(i, c - 1, d);
    }

    cout << ans << '\n';

    return 0;
}