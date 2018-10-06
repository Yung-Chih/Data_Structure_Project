#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

inline bool peak( short& x, short& y, vector< vector<long int> >& m);

int main(int argc, char const* argv[]){

    ofstream fout( "106062140\\matrix.data", ios::out);
    fout << 1000 << ' ' << 1000 << '\n';
    for(int i=0; i<1000;i++){
        for(int j=0;j<1000;j++){
            fout << 1 << " \n"[j==999];
        }
    }
    return 0;
}

