#include<bits/stdc++.h>

using namespace std;

char map[1000][1000];
int dist[1000][1000];
int m, n, life;

const int dir_x[4] = {-1,0,1,0};
const int dir_y[4] = {0,1,0,-1};

int BFS();

int main(int argc, char const *argv[])
{
    string path(argv[1]);
    ifstream fin( (path+"floor.data").c_str(), ios::in);

    fin >> m >> n >> life;
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            cin >> map[i][j];
        }
    }
    return 0;
}

int BFS(){
    q
}
