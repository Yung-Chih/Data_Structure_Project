#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

inline bool peak( int& x, int& y, vector< vector<int> > m);

int main(int argc, char const* argv[]){

    string root = argv[1];
    root += "\\matrix.data";
    
    ifstream fin( root.c_str() , ios::in);
    ofstream fout( "final.peak", ios::out);

    int m,n;
    fin >> m >> n;
    vector< vector<int> > map;
    map.resize(m);
    for(int i=0; i<m; i++){
        map[i].resize(n);
        for(int j=0; j<n; j++){
            fin >> map[i][j];
        }
    }

    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            fout << map[i][j] << " \n"[j==n-1];
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

    fout << point.size() << '\n';
    for( auto& i: point){
        fout << i.first << ' ' << i.second << '\n';
    }
    return 0;
}

inline bool peak( int& x, int& y, vector< vector<int> > m){
    bool valid = true;
    if( x > 0 )             valid &= (m[x][y] > m[x-1][y]);
    if( x < m.size()-1 )    valid &= (m[x][y] > m[x+1][y]);
    if( y > 0 )             valid &= (m[x][y] > m[x][y-1]);
    if( y < m[x].size()-1 ) valid &= (m[x][y] > m[x][y+1]);
    return valid;
}

