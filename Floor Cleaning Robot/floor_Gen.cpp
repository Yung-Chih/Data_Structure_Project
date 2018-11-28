#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;
using pii = pair<int, int>;
#define WALL 1e9
int dir_x[4] = {1,-1,0,0};
int dir_y[4] = {0,0,1,-1};
int Rx, Ry;
int m, n;

bool valid(const int& x, const int& y, vector< vector<int> >& map){
    if( x >= m ) return false;
    if( x <  0 ) return false;
    if( y >= n ) return false;
    if( y <  0 ) return false;
    return map[x][y] != WALL;
}

int BFS(vector<vector<int>>& map);



int main(int argc, char const *argv[])
{

    string path(argv[1]);
    ifstream fin ( (path+"\\floor.data").c_str(), ios::in );
    int a,b,c;
    fin >> a >> b >> c;
    if( !fin ){
        cout << "no file" << endl;
        return -1;
    }
    vector<string> map;

    string s;
    while( fin >> s ){
        map.emplace_back(s);
    }

    m = map.size();
    n = map[0].size();

    vector< vector<int> > GGG;
    GGG.resize(m);

    for(int i=0; i<map.size(); i++){
        GGG[i].resize(n);
        for(int j=0; j<map[i].size(); j++){
            if( map[i][j] == 'R' ){
                Rx = i, Ry = j;
            }
            GGG[i][j] = ( map[i][j] == '1' ) ? WALL : 0;
        }
    }

    int B = BFS(GGG);
    cout << B << '\n';
    /*
    cout << BFS(GGG) << endl;

    for(int i=0; i<map.size(); i++){
        for(int j=0; j<map[i].size(); j++){
            if( GGG[i][j] == WALL )
                cout << '-';
            else
                cout << GGG[i][j];
            cout << '\t' << " \n"[j==map[i].size()-1];
        }
    }
    */



    cout << map.size() << " " << map[0].size() << " " << B*2+1 << endl;
    /*
    for(int i=0; i<map.size(); i++){
        for(int j=0; j<map[i].size(); j++){
            cout << map[i][j];
        }
        cout << endl;
    }
    */
}


int BFS(vector<vector<int>>& map){
    int M = 0;
    queue<pii> q;
    q.emplace( Rx, Ry );
    while( !q.empty() ){
        pii p = q.front(); q.pop();
        for(int i=0; i<4; i++){
            int x = p.first + dir_x[i], y = p.second + dir_y[i];
            if( valid( x, y, map) && map[x][y] == 0 ){
                map[x][y] = map[p.first][p.second] + 1;
                M = max(M, map[x][y]);
                q.emplace(x,y);
            }
        }
    }
    map[Rx][Ry] = 0;
    return M;
}