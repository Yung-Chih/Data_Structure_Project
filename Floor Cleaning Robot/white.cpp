#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;
using pii = pair<int, int>;

int main(){
    string s1, s2;
    ofstream fout("1000_1000_white.data", ios::out);
    pii R( (rand()+7122)%1000, rand()*rand()%1000);
    vector<string> MMM;

    for(int i=0; i<1000; i++){
        s1 += '1';
        s2 += '0';
    }
    s2[0] = '1', s2[999] = '1';
    for(int i=0; i<1000; i++){
        MMM.emplace_back( ( i == 0 || i == 999 ) ? s1 : s2 );
    }
    MMM[R.first][R.second] = 'R';

    fout << 1000 << " " << 1000 << " " << 2000 << endl;
    for(int i=0; i<1000; i++){
        fout << MMM[i] << endl;
    }
    return 0;
}
