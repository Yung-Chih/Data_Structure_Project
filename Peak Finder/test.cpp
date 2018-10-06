#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

inline bool peak( short& x, short& y, vector< vector<long int> >& m);

int main(int argc, char const* argv[]){

    string root = argv[1];
    root += "\\matrix.data";
    
    ifstream fin( root.c_str() , ios::in);
    ofstream fout( "2D.peak", ios::out);

    short m,n;
    vector< vector<long int> > map;
    
    fin >> m >> n;
    map.resize(m);

    short i,j;
    for( i=0; i<m; i++){
        map[i].resize(n);
        for( j=0; j<n; j++){
            fin >> map[i][j];
        }
    }

    for( j=200; j<205; j++){
        cout << map[0][j] << ' ';
    }
    return 0;
}

