#include<iostream>
#include<vector>
#include<queue>
#include<fstream>
#include<algorithm>
#include<tuple>
#include<windows.h>

#define WALL 1e9

using namespace std;
using pii = pair<int,int>;
using tiii = tuple<int,int,int>;
int dir_x[4] = {1,-1,0,0};
int dir_y[4] = {0,0,1,-1};
int Rx, Ry;
int m, n, life;
int dirty = 0;
vector< vector<int> > map;
vector< vector<pii> > to_orgin;
priority_queue< tiii, vector<tiii>, greater<tiii> > pq;
vector<pii> ans;

void input(ifstream& fin);
void display();
void display(const int& Cx, const int& Cy);
int BFS();
pii BFS_nearest(const int& Rx, const int& Ry);
bool valid(const int& x, const int& y);
bool valid(const pii& p);
int step(const int& x, const int& y){
    return abs(map[x][y]);
}
int step(const pii& p){
    return step(p.first, p.second);
}
bool cleaned(const int& x, const int& y){
    return map[x][y] <= 0;
}
bool cleaned(const pii& p){
    return cleaned(p.first, p.second);
}

class Cleaner{
public:
    Cleaner(int& x, int& y, int& life): cur(x,y), Life(life){};
    ~Cleaner(){};
    void to_nearst();
    void auto_clean();
    void go_home();
    void charge(int life){
        Life = life;
    }
    pii pos(){ return cur; }
    int life(){ return Life; };
    bool at(int x, int y){
        return cur.first == x && cur.second == y;
    }
    void walk(pii next){
        Life -= (cur != next);
        cur = next;
        if( !cleaned(cur) ) dirty--;
        map[cur.first][cur.second] = -step(cur);
        ans.emplace_back(cur);
    }
private:
    pii cur;
    int Life;
};

int main(int argc, char const *argv[])
{
    // BUG : go through the R //
    ifstream fin("center.data", ios::in);
    ofstream fout("final.path", ios::out);
    
    fin >> m >> n >> life;
    input(fin);
    int s = 0;
    int d = BFS();
    cout << "max distance: " << d << endl;
    if( d*2 > life ){
        cout << "life is invalid" << endl;
        return -1;
    }

    Cleaner CC( Rx, Ry, life );
    display(CC.pos().first, CC.pos().second);
    pii prev(Rx, Ry);
    while(dirty){
        cout << "-----------------------------------\n";

        cout << "dirty: " << dirty << endl;
        cout << "action:";

        if( CC.at(Rx, Ry) ){
            cout << "to nearest" << endl;
            CC.to_nearst();
        }
        else if( CC.life() > step( CC.pos() ) ){
            cout << "Clean" << endl;
            CC.auto_clean();
        }
        else{
            cout << "go Home" << endl;
            CC.go_home();
        }
        cout << "Life:" << CC.life()  << endl;
        if( CC.at(Rx, Ry) ){
            cout << "Charge" << endl;
            CC.charge(life);
        }
        display(CC.pos().first, CC.pos().second);
        cout << "total step: " << ans.size() << " " << ++s << endl;
        cout << "-----------------------------------\n";
        if( CC.life() == 0 ){
            cout << "\aError: run out of Life\a" << endl;
            return -1;
        }
        if( prev == CC.pos() ){
            cout << "\aError: No Move\a" << endl;
            return -1;
        }
        prev = CC.pos();
        //Sleep(250);
        system("PAUSE");
        system("CLS");
    }

    while( !CC.at(Rx,Ry) ){
        cout << "-----------------------------------\n";
        cout << "action: go Home" << endl;
        CC.go_home();
        cout << "Life:" << CC.life()  << endl;
        if( CC.at(Rx, Ry) ){
            cout << "Charge" << endl;
            CC.charge(life);
        }
        display(CC.pos().first, CC.pos().second);
        cout << "total step: " << ans.size() << " " << ++s << endl;
        cout << "-----------------------------------\n";
        if( CC.life() == 0 ){
            cout << "\aError: run out of Life\a" << endl;
            return -1;
        }
        if( prev == CC.pos() ){
            cout << "\aError: No Move\a" << endl;
            return -1;
        }
        prev = CC.pos();
        //Sleep(250);
        system("PAUSE");
        system("CLS");
    }

    fout << ans.size() << endl;
    for(size_t i=0; i<ans.size(); i++){
        fout << ans[i].first << " " << ans[i].second << endl;
    }
    return 0;
}

int BFS(){
    int M = 0;
    queue<pii> q;
    q.emplace( Rx, Ry );
    while( !q.empty() ){
        pii p = q.front(); q.pop();
        for(int i=0; i<4; i++){
            int x = p.first + dir_x[i], y = p.second + dir_y[i];
            if( valid( x, y) && map[x][y] == 0 ){
                map[x][y] = map[p.first][p.second] + 1;
                M = max(M, map[x][y]);
                q.emplace(x,y);
                to_orgin[x][y] = p;
                pq.emplace( map[x][y], x, y );
            }
        }
    }
    map[Rx][Ry] = 0;
    to_orgin[Rx][Ry] = pii(-1,-1);
    return M;
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

void input(ifstream& fin){
    map.resize(m);
    to_orgin.resize(m);
    for(int i=0; i<m; i++){
        map[i].resize(n);
        to_orgin[i].resize(n);
        for(int j=0; j<n; j++){
            char c;
            fin >> c;
            map[i][j] = ( c == '1' ) ? WALL : 0;
            dirty += ( c == '0' );
            if( c == 'R' )
                Rx = i, Ry = j;
        }
    }
}


void display(){
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            if( i == Rx && j == Ry)
                cout << "R" << " \n"[j==n-1];
            else
                cout << (map[i][j] == WALL ) << " \n"[j==n-1];
        }
    }
}

void display(const int& Cx, const int& Cy){
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            if( i == Cx && j == Cy)
                cout << "C" << " \n"[j==n-1];
            else if( i == Rx && j == Ry)
                cout << "R" << " \n"[j==n-1];
            else if ( map[i][j] == WALL)
                cout << "1" << " \n"[j==n-1];
            else if ( map[i][j] > 0)
                cout << "0" << " \n"[j==n-1];
            else
                cout << "." << " \n"[j==n-1];
        }
    }
}


void Cleaner::go_home(){
    pii next = cur;
    for(int i=0; i<4; i++){
        int x = cur.first + dir_x[i], y = cur.second + dir_y[i];
        if( !valid(x,y) ) continue;
        if( step(x,y) < step(next) || ( step(x,y) == step(next) && !cleaned(x,y) ) ){
            next.first = x;
            next.second = y;
        }
    }
    walk(next);
}


void Cleaner::auto_clean(){
    pii next = cur;
    for(int i=0; i<4; i++){
        int x = cur.first + dir_x[i], y = cur.second + dir_y[i];
        if( !valid(x,y) ) continue;
        if( step(x,y) > step(next) && !cleaned(x,y) ){
            next.first = x;
            next.second = y;
        }
    }
    if( next != cur )
        walk(next);
    else
        go_home();
}


pii nearest(){
    
    int x = Rx, y= Ry;
    while( !pq.empty() ){
        x = get<1>(pq.top());
        y = get<2>(pq.top());
        if( !cleaned(x,y) )
            return pii(x,y);
        pq.pop();
    }
    return pii(x,y);
    
}

void Cleaner::to_nearst(){
    pii next = nearest();
    vector<pii> route;
    while( next != cur ){
        route.emplace_back( next );
        next = to_orgin[next.first][next.second];
    }
    for(int i = (int)route.size()-1; i>=0; i--){
        cout << route[i].first << " " << route[i].second << endl;
        walk(route[i]);
        if( !cleaned(route[i]) ) dirty--;
        map[route[i].first][route[i].second] = -step(route[i]);
    }
}