#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

short m,n;
vector< long int > test[3];
vector< pair<long int,long int> > point;

inline bool peak(short x, short y);

int main(int argc, char const *argv[])
{
    string root = argv[1];
    string out = argv[1];
    root += "\\matrix.data";
    out += "\\Vec.peak";
    
    ifstream fin( root.c_str() , ios::in);
    ofstream fout( out.c_str(), ios::out);

    fin >> m >> n;
    test[0].resize(n, -1e9);
    test[1].resize(n, -1e9);
    test[2].resize(n, -1e9);

    short i,j;
    for( j=0; j<n; j++){
        fin >> test[0][j];
    }

    for( i=1; i<m; i++){
        for( j=0; j<n; j++){
            fin >> test[i%3][j];
            if( peak(i-1,j) ){
                point.push_back( pair<long int, long int>(i,j+1) );
            }
        }
    }

    for( j=0; j<n; j++){
        if( peak(m-1,j) ){
            point.push_back( pair<long int, long int>(m,j+1) );
        }
    }

    fout << point.size() << '\n';

    for( int i=0; i<point.size(); i++){
        fout << point[i].first << ' ' << point[i].second << '\n';
    }

    return 0;
}

inline bool peak(short x, short y){
    bool valid = true;
    if( x > 0 )     valid &= test[x%3][y] >= test[(x+2)%3][y];
    if( x < m-1 )   valid &= test[x%3][y] >= test[(x+1)%3][y];
    if( y > 0 )     valid &= test[x%3][y] >= test[x%3][y-1];
    if( y < n-1 )   valid &= test[x%3][y] >= test[x%3][y+1];
    return valid;
}
