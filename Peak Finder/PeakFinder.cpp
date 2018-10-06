#include <iostream>
#include <fstream>
#include <string>
using namespace std;

short row,col;
long int test[3][1000];
pair<long int, long int> ans[1000*1000];
int cnt;

inline bool peak(short x, short y);

int main(int argc, char const *argv[])
{
    string root = argv[1];
    
    ifstream fin( (root + "\\matrix.data").c_str() , ios::in);
    ofstream fout( (root + "\\final.peak").c_str(), ios::out);


    fin >> row >> col;
    short i,j;
    for(j=0; j<col; j++)
        fin >> test[0][j];

    for(i=1; i<row;i++){
        for(j=0;j<col;j++){
            fin >> test[i%3][j];
            if( peak(i-1,j) ){
                ans[cnt].first = i;
                ans[cnt].second = j+1;
                cnt++;
            }
        }
    }

    for(j=0; j<col; j++){
        if( peak(row-1,j) ){
            ans[cnt].first = row;
            ans[cnt].second = j+1;
            cnt++;
        }
    }

    fout << cnt << '\n';
    for(int i=0; i<cnt; i++)
        fout << ans[i].first << ' ' << ans[i].second << '\n';

    return 0;
}

inline bool peak(short x, short y){
    bool valid = true;
    if( x > 0 )     valid &= test[x%3][y] >= test[(x+2)%3][y];
    if( x < row-1 )   valid &= test[x%3][y] >= test[(x+1)%3][y];
    if( y > 0 )     valid &= test[x%3][y] >= test[x%3][y-1];
    if( y < col-1 )   valid &= test[x%3][y] >= test[x%3][y+1];
    return valid;
}
