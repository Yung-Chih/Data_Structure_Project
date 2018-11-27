#include <bits/stdc++.h>
#include <windows.h>
#define WALL 1e9

using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;
const int dir_x[4] = {0,1,0,-1},
          dir_y[4] = {1,0,-1,0};


class FloorCleaner{
public:
    FloorCleaner(const pii& pos, const int& life ): _pos(pos), _life(life){};
    FloorCleaner(const int& x, const int& y, const int& life ): _pos(x,y), _life(life){};
    void charge(const int& life){ _life = life; }
    void go_home();
    void auto_clean();
    void go_back();
    void walk(const pii& to);
    void nearest_dirty();
    void print_status(){
        cout << "life: " << _life << ", pos: (" << _pos.first << ", " << _pos.second << ")" << endl;
    }
    void print_route();
    void print_route(ofstream& fout);
    const pii& pos(){ return _pos; };
    const int& life(){ return _life; };
    const size_t total_step(){ return _route.size(); }
private:
    pii _pos;
    int _life;
    vector<pii> _route;
};

int m, n, LIFE;
int dirty;
pii R;
vector< vector<int> > Floor;

void init_input(ifstream& fin);
void process_floor();
void display(const pii& pos);

bool isWall(const int& x, const int& y);
bool isWall(const pii& pos);

bool isCleaned(const int& x, const int& y);
bool isCleaned(const pii& pos);

int Step_to_R(const int& x, const int& y);
int Step_to_R(const pii& pos);

int main(int argc, char const *argv[])
{
    string path( argv[1] );
    ifstream fin ( (path + "\\floor.data").c_str(), ios::in );
    ofstream fout( (path + "\\final.path").c_str(), ios::out );

    fin >> m >> n >> LIFE;
    LIFE -= (LIFE&1);

    init_input(fin);
    process_floor();

    FloorCleaner FC(R, LIFE);
    //cout << "Clean Start" << endl;
    while( dirty ){
        //cout << "------------------------------\n";
        //FC.print_route();
        if( FC.pos() == R ){
            //cout << "!!!!!!!!!!!start!!!!!!!!!" <<endl;
            FC.go_back();
            //FC.print_status();
            FC.nearest_dirty();
        }
        else if( Step_to_R(FC.pos()) >= FC.life() ){
            //cout << "go Home" << endl;
            FC.go_home();
        }
        else{
            //cout << "auto Clean" <<endl;
            FC.auto_clean();
        }
        /*
        cout << "step: " << FC.total_step() << endl;
        cout << "step_to_R: " << Step_to_R(FC.pos()) << endl;
        FC.print_status();
        display(FC.pos());
        cout << "------------------------------\n";
        system("PAUSE");
        */
    }
    //cout << "Clean end, Go home" << endl;
    while( FC.pos() != R ){
        FC.go_home();
        /*
        cout << "step: " << FC.total_step() << endl;
        cout << "step_to_R: " << Step_to_R(FC.pos()) << endl;
        FC.print_status();
        display(FC.pos());
        cout << "------------------------------\n";
        system("PAUSE");
        */
    }
    //FC.print_route();
    FC.print_route(fout);
    //cout << "total step: " << FC.total_step() << endl;
    //cout << "End Normally" << endl;
    return 0;
}

void init_input(ifstream& fin){
    dirty = 0;
    Floor.resize(m);
    for(int i=0; i<m; i++){
        Floor[i].resize(n);
        for(int j=0; j<n; j++){
            char c; fin >> c;
            Floor[i][j] = ( c == '1' ) ? WALL : 0;
            dirty += ( c == '0' );
            if( c == 'R' )
                R.first = i, R.second = j;
        }
    }
}

void process_floor(){
    queue< pii > q;
    q.emplace(R);
    while( !q.empty() ){
        pii now = q.front(); q.pop();
        for(int i=0; i<4; i++){
            pii nxt( now.first + dir_x[i], now.second + dir_y[i] );
            if( nxt != R && !isWall(nxt) && Step_to_R(nxt) == 0 ){
                q.emplace(nxt);
                Floor[nxt.first][nxt.second] = Step_to_R( now ) + 1;
            }
        }
    }
}

void FloorCleaner::walk(const pii& to){
    _pos = to;
    _life--;
    _route.emplace_back(to);
    dirty -= ( Floor[to.first][to.second] > 0 );
    Floor[to.first][to.second] = -abs(Floor[to.first][to.second]);
    if( _pos == R )
        charge(LIFE);
}

void FloorCleaner::go_back(){
    if( _route.size() >= 2 ){
        walk(_route[ _route.size()-2 ]);
    }
}

void FloorCleaner::auto_clean(){
    pii nxt = _pos;
    for(int i=0; i<4; i++){
        pii adj( this->_pos.first + dir_x[i], this->_pos.second + dir_y[i] );
        if( isWall(adj) || isCleaned(adj) ) continue;
        if( Step_to_R(adj) > Step_to_R(nxt) && Step_to_R(adj) <= _life-1 )
            nxt = adj;
    }
    if( nxt == _pos )
        this->go_home();
    else
        this->walk(nxt);
}

void FloorCleaner::go_home(){
    pii nxt = _pos;
    for(int i=0; i<4; i++){
        pii adj( this->_pos.first + dir_x[i], this->_pos.second + dir_y[i] );
        if( isWall(adj) ) continue;
        if( Step_to_R(adj) < Step_to_R(nxt) )
            nxt = adj;
        else if ( Step_to_R(adj) == Step_to_R(nxt) && !isCleaned(adj))
            nxt = adj;
    }
    this->walk(nxt);
}

void FloorCleaner::print_route(){
    cout << _route.size() << '\n';
    for(size_t i=0; i<_route.size(); i++)
        cout << _route[i].first << ' ' << _route[i].second << '\n';
}
void FloorCleaner::print_route(ofstream& fout){
    fout << _route.size() << '\n';
    for(size_t i=0; i<_route.size(); i++)
        fout << _route[i].first << ' ' << _route[i].second << '\n';
}

void FloorCleaner::nearest_dirty(){
    vector< vector<int> > dir;
    dir.resize(m);
    for(int i=0; i<m; i++) dir[i].resize(n,-1);

    vector<pii> route;
    queue<pii> q;

    q.emplace(_pos);
    dir[_pos.first][_pos.second] = 0;
    dir[R.first][R.second] = 0;
    while( !q.empty() && route.empty() ){
        pii now = q.front();
        q.pop();
        for(int i=0; i<4; i++){
            pii nxt( now.first + dir_x[i], now.second + dir_y[i] );
            if( isWall(nxt) || dir[nxt.first][nxt.second] != -1  ) continue;
            dir[nxt.first][nxt.second] = (i+2)%4;
            if( !isCleaned(nxt) ){
                route.emplace_back(nxt);
                break;
            }
            q.emplace(nxt);
        }
    }
    while( true ){
        pii& now(route[route.size()-1]);
        int& d = dir[now.first][now.second];
        if( _pos.first == now.first + dir_x[d] && _pos.second == now.second + dir_y[d] )
            break;
        route.emplace_back( now.first + dir_x[d], now.second + dir_y[d]);
    }

    for( size_t i = route.size()-1; i>=0; i--){
        walk( route[i] );
        if( i == 0 ) break;
    }
}

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
            else if( isCleaned(now))
                cout << '.' ;
            else
                cout << '0';
            cout << " \n"[j == n-1];
        }
    }
}

bool isWall(const int& x, const int& y ){
    if( x >= m ) return true;
    if( x <  0 ) return true;
    if( y >= n ) return true;
    if( y < 0  ) return true;
    return Floor[x][y] == WALL;
}
bool isWall( const pii& pos ){
    return isWall( pos.first, pos.second );
}

bool isCleaned(const int& x, const int& y){
    if( isWall(x,y) ) return true;
    return Floor[x][y] <= 0;
}
bool isCleaned(const pii& pos){
    return isCleaned(pos.first, pos.second);
}

int Step_to_R(const int& x, const int& y){
    if( isWall(x,y) ) return 1e9;
    return abs(Floor[x][y]);
}
int Step_to_R(const pii& pos){
    return Step_to_R(pos.first, pos.second);
}
