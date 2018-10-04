#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int main(int argc, char const *argv[])
{
    ofstream OF( "test.data", ios::out);

    int row = 1, col = 712;
    int i=0,j=0;
    srand(7122);
    OF << row << ' ' << col << endl;
    while( i<row && j<col){
        if( j < col/2 ){
            OF << rand()%1869-971 << ' ';
            j++;
        }
        else if( j < col/2+10 ){
            OF << "-7777777 -777777 -7777777 ";
            j+=3;
        }
        else if( j < col-1 ){
            OF << rand()%rand()-rand() << ' ';
            j++;
        }
        else {
            OF << "7122\n";
            j++;
        }
    }

    return 0;
}
