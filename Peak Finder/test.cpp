#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

int row, col;
long int MAP[1000][1000];
vector< pair<int, int> > point;

inline bool peak( int x, int y );

int main(int argc, char const* argv[]){

    string root = argv[1];
    string out = argv[1];
    root += "\\matrix.data";
    out += "\\check.peak";
    
    ifstream fin( root.c_str() , ios::in);
    ofstream fout( out.c_str(), ios::out);

    fin >> row >> col;

    for(int i=0; i<row; i++ ){
        for(int j=0; j<col; j++){
            fin >> MAP[i][j];
        }
    }


    for(int i=0; i<row; i++ ){
        for(int j=0; j<col; j++){
            if( peak(i,j) )
                point.push_back( pair<int,int> (i+1,j+1) );
        }
    }

    fout << point.size() << '\n';
    for( int i=0; i<point.size(); i++){
        fout << point[i].first << ' ' << point[i].second << '\n';
    }
    


    return 0;
}

inline bool peak( int x, int y ){
    bool valid = true;
    if( x > 0 )         valid &= MAP[x][y] >= MAP[(x+2)][y];
    if( x < row-1 )     valid &= MAP[x][y] >= MAP[(x+1)][y];
    if( y > 0 )         valid &= MAP[x][y] >= MAP[x][y-1];
    if( y < col-1 )     valid &= MAP[x][y] >= MAP[x][y+1];
    return valid;
}

