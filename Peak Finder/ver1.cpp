#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

ifstream IF( "matrix.data", ios::in);
ofstream OF( "final.peak", ios::out);

void ver1( int& m, int& n);
inline bool peak( int& x, int& y, vector< vector<int> > m);

int main(){

    int m,n;
    IF >> m >> n;
    ver1(m,n);
    return 0;
}

void ver1( int& m, int& n){
    vector< vector<int> > map;
    map.resize(m);
    for(int i=0; i<m; i++){
        map[i].resize(n);
        for(int j=0; j<n; j++){
            IF >> map[i][j];
        }
    }

    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            cout << map[i][j] << " \n"[j==n-1];
        }
    }

    vector< pair<int,int> > point;


    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            if( peak(i,j, map) ){
                point.emplace_back( i+1, j+1);
            }
        }
    }

    OF << point.size() << '\n';
    for( auto& i: point){
        OF << i.first << ' ' << i.second << '\n';
    }
}

inline bool peak( int& x, int& y, vector< vector<int> > m){
    bool valid = true;
    if( x > 0 )             valid &= (m[x][y] > m[x-1][y]);
    if( x < m.size()-1 )    valid &= (m[x][y] > m[x+1][y]);
    if( y > 0 )             valid &= (m[x][y] > m[x][y-1]);
    if( y < m[x].size()-1 ) valid &= (m[x][y] > m[x][y+1]);
    return valid;
}

