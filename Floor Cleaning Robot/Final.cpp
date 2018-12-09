#include<bits/stdc++.h>
#define WALL (int)1e9

using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int dir_x[4] = {0,1,0,-1},
          dir_y[4] = {1,0,-1,0};

int m, n, LIFE, dirty=0;
int entry[4] = {0, 0, 0, 0};
int dis[4][4];
int p[4][4];
pii R;
vector< vector<int> > Floor;
vector< vector<pii> > dp;
vector< pii > S_route[4][4];

inline void init_input( ifstream& fin );
inline int dp_update( const pii& pos, const int& deep);
inline vector<pii> shortest_route( const pii& from,const pii& to );

inline bool isWall(const int& x, const int& y ){
    if( x >= m ) return true;
    if( x <  0 ) return true;
    if( y >= n ) return true;
    if( y < 0  ) return true;
    return Floor[x][y] == WALL;
}
inline bool isWall( const pii& pos ){
    return isWall( pos.first, pos.second );
}

inline int Step_to_R(const int& x, const int& y){
    if( isWall(x,y) ) return 1e9;
    return abs(Floor[x][y]);
}
inline int Step_to_R(const pii& pos){
    return Step_to_R(pos.first, pos.second);
}

inline bool isClean(const int& x, const int& y){
    if( isWall(x,y) ) return true;
    return Floor[x][y] <= 0;
}
inline bool isClean(const pii& pos){
    return isClean(pos.first, pos.second);
}

inline bool inside_dp(const int& x, const int& y){
    if( isWall(x,y) ) return false;
    return dp[x][y].first != -1;
}
inline bool inside_dp(const pii& pos){
    return inside_dp(pos.first, pos.second);
}

inline int dir_a_to_b(const pii& a, const pii&b ){
    int dx = b.first - a.first;
    int dy = b.second - a.second;
    if( dx == 0 && dy == 1)  return 0;
    if( dx == 1 && dy == 0)  return 1;
    if( dx == 0 && dy == -1) return 2;
    if( dx == -1 && dy == 0)  return 3;
    return -1;
}

class FloorCleaner{
public:
    FloorCleaner(const pii& pos, const int& life ): _pos(pos), _life(life){};
    FloorCleaner(const int& x, const int& y, const int& life ): _pos(x,y), _life(life){};
    void charge(const int& life){ _life = life; }
    void go_home();
    void auto_clean();
    void change_entry();
    void walk(const pii& to){
        _pos = to;
        _life--;
        _route.emplace_back(to);
        if( !isClean(to) ){
            dirty--;
            Floor[to.first][to.second] *= -1;
        }
        if( _pos == R )
            charge(LIFE);
    }
    void walk(const int& x, const int& y){
        walk( pii(x,y) );
    }
    void restart();
    void print_route(ofstream& fout){
        fout << _route.size() << '\n';
        for( pii& p : _route )
            fout << p.first << ' ' << p.second << '\n';
    }
    const pii& pos(){ return _pos; };
    const int& life(){ return _life; };
private:
    pii _pos;
    int _life;
    vector<pii> _route;
};


int main(int argc, char const *argv[])
{

    string path(argv[1]);
    ifstream fin ( (path+"/floor.data").c_str(), ios::in );
    init_input(fin);
    fin.close();

    FloorCleaner FC(R, LIFE);

    while( dirty ){

        if( FC.pos() == R ){
            FC.restart();
        }
        else if( Step_to_R(FC.pos()) >= FC.life() ){
            FC.go_home();
        }
        else{
            FC.auto_clean();
        }
    }
    while( FC.pos() != R ){
        FC.go_home();
    }

    ofstream fout( (path+"/final.path").c_str(), ios::out );
    FC.print_route(fout);
    fout.close();

    return 0;
}

inline void init_input( ifstream& fin ){
    fin >> m >> n >> LIFE;
    LIFE -= (LIFE&1);
    dirty = 0;
    Floor.resize(m);
    dp.resize(m);
    for(int i=0; i<m; i++){
        Floor[i].resize(n);
        dp[i].resize(n);
        for(int j=0; j<n; j++){
            char c; fin >> c;

            Floor[i][j] = ( c == '1' ? 1e9 : -1 );
            dirty += ( c == '0' );
            if( c == 'R' )
                R.first = i, R.second = j;
        }
    }
    /// BFS O( n*m )
    queue<pii> q;
    q.emplace( R );
    Floor[R.first][R.second] = 0;

    while( !q.empty() ){
        pii now = q.front(); q.pop();

        for(int i=0; i<4; i++){
            int x = now.first + dir_x[i], y = now.second + dir_y[i];
            if( isWall(x,y) ) continue;
            if( Floor[x][y] == -1 ){
                Floor[x][y] = Step_to_R(now) + 1;
                q.emplace(x,y);
            }
        }

    }

    for(int i=0; i<4; i++){
        int x = R.first + dir_x[i], y = R.second + dir_y[i];
        if( isWall(x,y) ) continue;
        entry[i] = 1;
    }

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){

            pii f( R.first + dir_x[i], R.second + dir_y[i] );
            pii t( R.first + dir_x[j], R.second + dir_y[j] );
            S_route[i][j] = shortest_route( f, t);

            dis[i][j] = S_route[i][j].size();
            p[i][j] = -1;
            if( Step_to_R(t) + dis[i][j] + 1 > LIFE )
                dis[i][j] = 1e9;

        }
    }

    for( int k=0; k<4; k++)
        for(int i=0; i<4; i++ )
            for(int j=0; j<4; j++)
                if( dis[i][j] > dis[i][k] + dis[k][j] ){
                    dis[i][j] = dis[i][k] + dis[k][j];
                    p[i][j] = k;
                }


}

void FloorCleaner::go_home(){
    pii nxt = _pos;
    for(int i=0; i<4; i++){
        pii adj( this->_pos.first + dir_x[i], this->_pos.second + dir_y[i] );
        if( isWall(adj) ) continue;
        if( !inside_dp(adj) ) continue;

        if( Step_to_R(adj) < Step_to_R(nxt) )
            nxt = adj;
        else if ( Step_to_R(adj) == Step_to_R(nxt) && !isClean(adj) )
            nxt = adj;
    }
    this->walk(nxt);
}

void FloorCleaner::auto_clean(){
    pii nxt = _pos;
    for(int i=0; i<4; i++){
        pii adj( this->_pos.first + dir_x[i], this->_pos.second + dir_y[i] );
        if( isWall(adj) || isClean(adj) ) continue;
        if( !inside_dp(adj) ) continue;

        if( Step_to_R(nxt) < Step_to_R(adj) && Step_to_R(adj) <= _life-1 )
            nxt = adj;
    }
    if( nxt == _pos )
        this->go_home();
    else
        this->walk(nxt);
}

void FloorCleaner::restart(){
    const size_t& cnt = _route.size();
    if( cnt >= 2 )
        walk( _route[cnt-2] );


    for(int i=0; i<m; i++)
        fill( dp[i].begin(), dp[i].end(), pii(-1, -1) );
    dp[R.first][R.second].first = dp_update(_pos, _life);


    if(  dp[_pos.first][_pos.second].first == 0 ){
        change_entry();
        return;
    }

    while( dp[_pos.first][_pos.second].second != -1 ){
        int& d = dp[_pos.first][_pos.second].second;
        walk( _pos.first + dir_x[d], _pos.second + dir_y[d] );
    }

}

inline int dp_update( const pii& pos, const int& deep){

    stack<int> st;
    st.emplace( pos.first*1000 + pos.second );

    while( !st.empty() ){
        int x = st.top()/1000;
        int y = st.top()%1000;
        int cnt = 0, M = 0;

        for(int i=0; i<4; i++){
            int nx = x + dir_x[i];
            int ny = y + dir_y[i];
            if( isWall(nx,ny) || Step_to_R(nx, ny) < Step_to_R(x, y) ) continue;

            if( !inside_dp(nx, ny) && (int)st.size() + Step_to_R(nx,ny) <= deep ){
                st.emplace( nx*1000 + ny );
                cnt++;
                break;
            }

            if( M < dp[nx][ny].first ){
                M = dp[nx][ny].first;
                dp[x][y].second = i;
            }
        }
        if( cnt == 0 )
            dp[x][y].first = M + !isClean(x,y);
        if( inside_dp(x,y) )
            st.pop();
    }
    return dp[pos.first][pos.second].first;
}

void FloorCleaner::change_entry(){
    int s = dir_a_to_b(R, _pos);
    vector<int> point;
    entry[s] = 2;

    for(int i=(s+1)%4; i!=s; i = (i+1)%4){
        if( entry[i] == 1 ){
            point.emplace_back(s);
            int a = s, b = i;
            while( p[a][b] != -1 ){
                a = p[a][b];
                point.emplace_back(a);
            }
            point.emplace_back(i);
            break;
        }
    }

    for(int i=1; i<(int)point.size(); i++){
        int& a = point[i-1];
        int& b = point[i];
        for( auto rit = S_route[a][b].rbegin(); rit != S_route[a][b].rend(); rit++)
            walk( *rit );
        if( i != (int)point.size() -1 ){
            walk(R);
            walk( _route[ _route.size()-2 ] );
        }
    }

}

inline vector<pii> shortest_route( const pii& from,const pii& to ){
    vector<pii> route;
    if( isWall(from) || isWall(to) || from == to ) return route;

    vector<vector<int>> dir;
    dir.resize(m);
    for(int i=0; i<m; i++) dir[i].resize(n, -1);

    queue<int> q;
    q.emplace( from.first*1000 + from.second );
    dir[from.first][from.second] = 4;
    dir[R.first][R.second] = 4;
    while( !q.empty() ){
        int x = q.front()/1000;
        int y = q.front()%1000;
        if( x == to.first && y == to.second ) break;
        q.pop();

        for(int i=0; i<4; i++){
            int nx = x + dir_x[i];
            int ny = y + dir_y[i];
            if( isWall(nx,ny) || dir[nx][ny] != -1 ) continue;

            dir[nx][ny] = (i+2)%4;
            q.emplace(nx*1000 + ny);
        }

    }
    int x = to.first;
    int y = to.second;
    while( x != from.first || y != from.second ){
        route.emplace_back(x,y);
        int& d = dir[x][y];
        x += dir_x[ d ];
        y += dir_y[ d ];
    }

    return route;

}

