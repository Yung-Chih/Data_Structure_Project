#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

inline bool peak( short& x, short& y, vector< vector<long int> >& m);

int main(int argc, char const* argv[]){

    string root = argv[1];
    string out = argv[1];
    root += "\\matrix.data";
    out += "\\2D.peak";
    
    ifstream fin( root.c_str() , ios::in);
    ofstream fout( out.c_str(), ios::out);

    short m,n;
    vector< vector<long int> > map;
    vector< pair<long int,long int> > point;
    
    fin >> m >> n;
    map.resize(m);

    short i,j;
    for( i=0; i<m; i++){
        map[i].resize(n);
        for( j=0; j<n; j++){
            fin >> map[i][j];
        }
    }

    for( i=0; i<m; i++){
        for( j=0; j<n; j++){
            if( peak(i,j, map) ){
                point.push_back( pair<long int, long int>(i+1, j+1) );
            }
        }
    }

    fout << point.size() << '\n';
    for(int i=0; i<point.size(); i++){
        fout << point[i].first << ' ' << point[i].second << '\n';
    }
    return 0;
}

inline bool peak( short& x, short& y, vector< vector<long int> >& m){
    bool valid = true;
    if( x > 0 )             valid &= (m[x][y] >= m[x-1][y]);
    if( x < m.size()-1 )    valid &= (m[x][y] >= m[x+1][y]);
    if( y > 0 )             valid &= (m[x][y] >= m[x][y-1]);
    if( y < m[x].size()-1 ) valid &= (m[x][y] >= m[x][y+1]);
    return valid;
}

