#include<bits/stdc++.h>
#include<ctime>
#include<cassert>
#define WALL 1e9

using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int dir_x[4] = {0,1,0,-1},
          dir_y[4] = {1,0,-1,0};

struct Node{
    int dist = -1;
    int cnt = 0;
};

int m, n, LIFE, dirty=0;
int entry[4] = {0, 0, 0, 0};
pii R;
vector< vector<Node> > Floor;
vector< vector<pii> > dp;
set< pii > leaf;

inline bool isWall(const int& x, const int& y ){
    if( x >= m ) return true;
    if( x <  0 ) return true;
    if( y >= n ) return true;
    if( y < 0  ) return true;
    return Floor[x][y].dist == WALL;
}
inline bool isWall( const pii& pos ){
    return isWall( pos.first, pos.second );
}

inline int Step_to_R(const int& x, const int& y){
    if( isWall(x,y) ) return 1e9;
    return abs(Floor[x][y].dist);
}
inline int Step_to_R(const pii& pos){
    return Step_to_R(pos.first, pos.second);
}

inline bool isClean(const int& x, const int& y){
    if( isWall(x,y) ) return true;
    return Floor[x][y].dist <= 0;
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
    bool go_to( const pii& to);
    void walk(const pii& to){
        _pos = to;
        _life--;
        _route.emplace_back(to);
        if( !isClean(to) ){
            dirty--;
            Floor[to.first][to.second].dist *= -1;
        }
        if( _pos == R )
            charge(LIFE);
    }
    void walk(const int& x, const int& y){
        walk( pii(x,y) );
    }
    void restart();
    void print_status(){
        cout << "life: " << _life << ", pos: (" << _pos.first << ", " << _pos.second << ")" << endl;
    }
    void print_route(){
        for( pii& p : _route )
            cout << p.first << ' ' << p.second << '\n';
    }
    void print_route(ofstream& fout){
        fout << _route.size() << '\n';
        for( pii& p : _route )
            fout << p.first << ' ' << p.second << '\n';
    }
    const pii& pos(){ return _pos; };
    const int& life(){ return _life; };
    size_t total_step(){ return _route.size(); }
private:
    pii _pos;
    int _life;
    vector<pii> _route;
};

inline void init_input( ifstream& fin );

inline int dp_update( const pii& pos, const int& deep);


void display(const pii& pos){
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            pii now(i,j);
            if( now == R )
                cout << 'R';
            else if( now == pos )
                cout << 'C';
            else if( isWall(now) )
                cout << '-';
            else if( isClean(now))
                cout << '.' ;
            else
                cout << '0';
            cout << " \n"[j == n-1];
        }
    }
}



int main(int argc, char const *argv[])
{
    clock_t t;

    t = clock();
    string path(argv[1]);
    ifstream fin ( (path+"\\floor.data").c_str(), ios::in );
    
    if( !fin ){
        cout << "no file" << endl;
        return -1;
    }

    init_input(fin);
    fin.close();
    FloorCleaner FC(R, LIFE);
    cout << "Clean Start" << '\n';
    while( dirty ){
        /*
        if( dirty <= 10000 ){
            cout << "dirty: " << dirty << '\n';
            FC.print_status();
            display(FC.pos() );
        }
        system("PAUSE");
        */
        if( FC.pos() == R ){
            //cout << "restart\n";
            //clock_t test = clock();
            FC.restart();
            
            //cout << "dp:" << ((float)(clock()-test)/CLOCKS_PER_SEC) << '\n';
        }
        else if( Step_to_R(FC.pos()) >= FC.life() ){
            //cout << "go home\n";
            FC.go_home();
        }
        else{
            //cout << "auto clean\n";
            FC.auto_clean();
        }

        //system("PAUSE");
        //cout << ((float)(clock()-t)/CLOCKS_PER_SEC) << '\n';
    }
    cout << "Clean end\n";
    while( FC.pos() != R ){
        FC.go_home();
        //cout << ((float)(clock()-t)/CLOCKS_PER_SEC) << '\n';
    }

    ofstream fout( (path+"\\final.path").c_str(), ios::out );
    FC.print_route(fout);
    fout.close();
    cout << "end Normally \n"; 
    cout << ((float)(clock()-t)/CLOCKS_PER_SEC) << '\n';
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

            Floor[i][j].dist = ( c == '1' ? 1e9 : -1 ); 
            dirty += ( c == '0' );
            if( c == 'R' )
                R.first = i, R.second = j;
        }
    }
    cout << "input end" << '\n';
    /// BFS O( n*m )
    queue<pii> q;
    q.emplace( R );
    Floor[R.first][R.second].dist = 0;
    
    while( !q.empty() ){
        pii now = q.front(); q.pop();
        //cout << now.first << ' ' << now.second << ":" << Step_to_R(now) << '\n';

        int child = 0;

        for(int i=0; i<4; i++){
            int x = now.first + dir_x[i], y = now.second + dir_y[i];
            if( isWall(x,y) ) continue;
            if( Floor[x][y].dist == -1 ){
                Floor[x][y].dist = Step_to_R(now) + 1;
                q.emplace(x,y);
            }

            child += ( Step_to_R(now) < Step_to_R(x,y) );
        }

        if( !child ){
            leaf.insert( now );
        }

    }
    for(int i=0; i<4; i++){
        int x = R.first + dir_x[i], y = R.second + dir_y[i];
        if( isWall(x,y) ) continue;
        entry[i] = 1;
    }
    cout << "Entry: ";
    for(int i=0; i<4; i++){
        cout << entry[i] << ' ';
    }
    cout << endl;
    cout << "init end" << '\n';
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
    dp[R.first][R.second].first = dp_update(_pos, _life/2);
    
    //assert( dp[_pos.first][_pos.second].first > 0 );

    if(  dp[_pos.first][_pos.second].first == 0 ){
        entry[ dir_a_to_b(R, _pos) ] = 2;
        for(int i=0; i<4; i++){
            pii to( R.first + dir_x[i], R.second + dir_y[i]);
            if( entry[i] == 1 && go_to(to) )
                return;
        }
    }
    
    while( dp[_pos.first][_pos.second].second != -1 ){
        int& d = dp[_pos.first][_pos.second].second;
        walk( _pos.first + dir_x[d], _pos.second + dir_y[d] );
    }
    /*
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            if(dp[i][j].first == -1)
                cout << " " << " \n"[j==n-1];
            else 
                cout << dp[i][j].first << " \n"[j==n-1];
        }
    }
    cout << "--------------------------\n";
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            if(dp[i][j].second == -1)
                cout << " " << " \n"[j==n-1];
            else 
                cout << dp[i][j].second << " \n"[j==n-1];
        }
    }
    cout << "end" << '\n';
    
    system("PAUSE");
    */
}



inline int dp_update( const pii& pos, const int& deep){
    if( deep == 0 || isWall(pos))
        return dp[pos.first][pos.second].first = !isClean(pos);
    
    if( dp[pos.first][pos.second].first != -1 )
        return dp[pos.first][pos.second].first;
    //cout << Floor[pos.first][pos.second].dist << ", deep: " << deep << '\n';
    int M = 0;
    for(int i=0; i<4; i++){
        pii adj( pos.first + dir_x[i], pos.second + dir_y[i] );
        if( isWall(adj) || Step_to_R(adj) < Step_to_R(pos) ) continue;
        if( M < dp_update(adj, deep-1) ){
            M = dp[adj.first][adj.second].first;
            dp[pos.first][pos.second].second = i;
        }
    }

    return dp[pos.first][pos.second].first = M + !isClean(pos);
}



bool FloorCleaner::go_to( const pii& to ){
    vector<vector<int>> dir;
    dir.resize(m);
    for(int i=0; i<m; i++) dir[i].resize(n, -1);

    queue<int> q;
    q.emplace( _pos.first*1000 + _pos.second );
    dir[_pos.first][_pos.second] = 10;
    dir[R.first][R.second] = 10;
    while( !q.empty() ){
        int x = q.front()/1000;
        int y = q.front()%1000;
        //cout << x << ' ' << y << '\n';
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
    /*
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            cout << dir[i][j] << " \n"[j==n-1];
        }
    }
    */
    int x = to.first;
    int y = to.second;
    vector<pii> route;
    while( x != _pos.first || y != _pos.second ){
        route.emplace_back(x,y);
        int& d = dir[x][y];
        x += dir_x[ d ];
        y += dir_y[ d ];
    }
    if( (int)route.size() + Step_to_R(to) >= _life )
        return false;
    for( auto rit = route.rbegin(); rit != route.rend(); rit++)
        walk( *rit );
    
    return true;
}