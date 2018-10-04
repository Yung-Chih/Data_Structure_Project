#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

ifstream IF( "matrix.data", ios::in);
ofstream OF( "final.peak", ios::out);

int m,n;
vector<int> test[3];
vector< pair<int,int> > point;

inline bool peak(int x, int y);

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    
    IF >> m >> n;
    test[0].resize(n, -1e9);
    test[1].resize(n, -1e9);
    test[2].resize(n, -1e9);


    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            IF >> test[i%3][j];
            if( i > 0 && peak(i-1,j) ){
                point.emplace_back(i,j+1);
            }
        }
    }

    for(int j=0; j<n; j++){
        if( peak(m-1,j) ){
            point.emplace_back(m,j+1);
        }
    }

    OF << point.size() << '\n';
    cout << point.size() << '\n';

    for( auto& i: point){
        OF << i.first << ' ' << i.second << '\n';
        cout << i.first << ' ' << i.second << '\n';
    }

    return 0;
}

inline bool peak(int x, int y){
    bool valid = true;
    if( x > 0 )     valid &= test[x%3][y] >= test[(x+2)%3][y];
    if( x < m-1 )   valid &= test[x%3][y] >= test[(x+1)%3][y];
    if( y > 0 )     valid &= test[x%3][y] >= test[x%3][y-1];
    if( y < n-1 )   valid &= test[x%3][y] >= test[x%3][y+1];
    return valid;
}
