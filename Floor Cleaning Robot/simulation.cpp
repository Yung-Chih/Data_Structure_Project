#include<iostream>
#include<vector>
#include<queue>
#include<fstream>
#include<algorithm>
#include<tuple>
#include<cassert>
#include<windows.h>

#define WALL 1e9

using namespace std;
using pii = pair<int, int>;

int m, n, life;
char map[1000][1000];
int MAP[1000][1000];
bool vis[1000][1000];
int dir_x[4] = {1,-1,0,0};
int dir_y[4] = {0,0,1,-1};
int Rx, Ry;
int dirty = 0;


bool valid(const int& x, const int& y);
bool valid(const pii& p);
int BFS();

void display(int x, int y, int life){
    cout << "-------------------\n";
    cout << "dirty: " << dirty << endl;
    cout << "life: " << dirty << endl;
    cout << "pos: (" << x << ", "<< y << ")" << endl;
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            map[i][j] = ( vis[i][j] && map[i][j] == '0' ) ? '.' : map[i][j];
            cout << ( i == x && j == y ? 'C' : map[i][j]) << " \n"[j==n-1];
        }
    }
    cout << "---------------------\n";
}

bool check_testcase();

int main(){
    ifstream fin( "center.data", ios::in);
    ifstream ain( "final.path", ios::in);

    fin >> m >> n >> life;
    cout << m << ' ' << n << ' ' << life << endl;
    assert( m < 1000 && n < 1000 );

    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            fin >> map[i][j];
            MAP[i][j] = (map[i][j] == '1' )? WALL : 0;
            dirty += (map[i][j] == '0');
            vis[i][j] = !(map[i][j] == '0');
            if( map[i][j] == 'R' )
                Rx = i, Ry = j;
        }
    }

    int M = BFS();
    assert( life >= M*2 );
    assert( check_testcase() );

    int cur_life = life;
    size_t k; 
    ain >> k;
    int px = Rx, py = Ry;
    int o_x = -1, o_y = -1;
    for(size_t i=0; i<k; i++){
        int x, y; ain >> x >> y;

        assert( x < m && x >= 0 );
        assert( y < n && y >= 0 );
        assert( map[x][y] == 'R' || map[x][y] == '0' || map[x][y] == '.');
        cout << cur_life << endl;
        cout << (cur_life>0) << endl;
        assert( cur_life > 0 );
        assert( (abs(px-x) + abs(py-y)) == 1 );
        
        dirty -= (vis[x][y] == false);

        vis[x][y] = true;
        cur_life--;
        if( x == Rx && y == Ry )
            cur_life = life;
        display(x,y, cur_life);

        /// check if go through the R
        if( o_x != -1 && o_y != -1){
            cout << "enter & out is different" << endl;
            assert( o_x == x && o_y == y );
        }

        if( x == Rx && y == Ry ){
            o_x = px, o_y = py;
        }
        else o_x = o_y = -1;
        px = x, py = y;

    }

    assert( px == Rx && py == Ry );
    assert( dirty == 0 );
    cout << "clear!" << endl;
}

bool valid(const int& x, const int& y){
    if( x >= m ) return false;
    if( x <  0 ) return false;
    if( y >= n ) return false;
    if( y <  0 ) return false;
    return map[x][y] != WALL;
}

bool valid(const pii& p){
    return valid(p.first, p.second);
}

int BFS(){
    int M = 0;
    queue<pii> q;
    q.emplace( Rx, Ry );
    while( !q.empty() ){
        pii p = q.front(); q.pop();
        for(int i=0; i<4; i++){
            int x = p.first + dir_x[i], y = p.second + dir_y[i];
            if( valid( x, y) && MAP[x][y] == 0 ){
                MAP[x][y] = MAP[p.first][p.second] + 1;
                M = max(M, MAP[x][y]);
                q.emplace(x,y);
            }
        }
    }
    MAP[Rx][Ry] = 0;
    return M;
}

bool check_testcase(){
    int cnt = 0;
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            cnt += ( MAP[i][j] != WALL && MAP[i][j] != 0 );
        }
    }
    return cnt == dirty;
}

