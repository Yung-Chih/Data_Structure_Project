#include<iostream>
#include<queue>
#include<vector>
#include<cstring>
#include<cassert>
#include<cstdlib>
#include<fstream>

using namespace std;

ofstream fout( "result.out", ios::out );

const int dir_x[4] = { 1, -1, 0, 0},
          dir_y[4] = { 0, 0, -1, 1};
enum Color{Black, Red, Blue, White };
bool Playing = true;

bool valid( int x, int y){
    if( x < 0 || x >= 5 || y < 0 || y >= 6 ) return false;
    return true;    
}

void init(int Record[5][6], int Max[5][6], Color color[5][6] );
bool make_step(int x, int y, Color inputcolor, int Record[5][6], int Max[5][6], Color color[5][6] );
void boom(int x, int y, int Record[5][6], int Max[5][6], Color color[5][6] );
void print_board( int Record[5][6], Color color[5][6] );

class Student_ver1{
    public:
        void makeMove(int Record[5][6], int Max[5][6], Color color[5][6], Color inputColor){
            std::vector<int> pos = this->possible_pos(Record, Max, color, inputColor);
            if( pos.size() == 0 ) return;
            this->x = pos[0]/10, this->y = pos[0]%10;
            int D = Max[x][y] - Record[x][y];
            for(int i=1; i<pos.size(); i++){
                int tx = pos[i]/10, ty = pos[i]%10;
                int td = Max[tx][ty] - Record[tx][ty];
                if( D > td )
                    this->x = tx, this->y = ty;
            }
        }
        int getX(){
            return x;
        }
        int getY(){
            return y;
        }

        vector<int> possible_pos( int Record[5][6], int Max[5][6], Color color[5][6], Color inputColor ){
            vector<int> pos;
            for(int i=0; i<5; i++)
                for(int j=0; j<6; j++)
                    if( color[i][j] == inputColor || color[i][j] == White )
                        pos.push_back(i*10 + j);
            return pos;
        }
    private:
        int x;
        int y;
};
class Student{
    public:
        
        void makeMove(int Record[5][6], int Max[5][6], Color color[5][6], Color inputColor){
            
            vector<int> pos = this->possible_pos(Record, Max, color, inputColor);
            
            if( pos.size() == 0 ) return;

            int nxt_Record[5][6];
            Color nxt_color[5][6];
            Color ecolor = ( inputColor == Red ) ? Blue : Red;

            int sel_value = -1e9;
            Color sel_c = inputColor;

            for(int i = 0; i < pos.size(); i++ ){
                int now_x = pos[i]/10, now_y = pos[i]%10;
                //Color now_c = color[now_x][now_y];
                //next_board(now_x, now_y, nxt_Record, nxt_color, Record, Max, color, inputColor);
                //int cnt = count_color( nxt_color, inputColor ) + count_color( color, White);
                int v = point_value( now_x, now_y, Record, Max, color, inputColor );
                
                if( sel_value < v || ( sel_value == v && sel_c == inputColor && color[now_x][now_y] == White ) ){
                    this->x = now_x;
                    this->y = now_y;
                    sel_value = v;
                    sel_c = color[now_x][now_y];
                }
                cout << "(" << now_x << ", " << now_y << ") " << v << ": " << color[now_x][now_y] << endl;

            }
            cout << "---------------------" << endl;
            cout << "(" << x << ", " << y << ") " << sel_value << ": " << sel_c << endl << endl;

        }
        int getX(){
            return x;
        }
        int getY(){
            return y;
        }
        vector<int> possible_pos( int Record[5][6], int Max[5][6], Color color[5][6], Color inputColor ){
            vector<int> pos;
            for(int i=0; i<5; i++)
                for(int j=0; j<6; j++)
                    if( color[i][j] == inputColor || color[i][j] == White )
                        pos.push_back(i*10 + j);
            return pos;
        }

        bool valid( int x, int y ){
            return( x >= 0 && x < 5 && y >= 0 && y < 6 );
        }

        int board_value(int Record[5][6], int Max[5][6], Color color[5][6], Color mycolor){
            
        }

        int point_value(int x, int y, int Record[5][6], int Max[5][6], Color color[5][6], Color inputColor ){
            int dist = Max[x][y] - Record[x][y];

            int value = 0, cnt = 0;
            for(int i = 0; i<4; i++){
                int nx = x + dir_x[i], ny = y + dir_y[i];
                int nc = color[nx][ny];
                if( !valid( nx, ny ) ) continue;
                int nd = Max[nx][ny] - Record[nx][ny];
                
                cnt += ( nc != Black );
                if( nc == White ){
                    value += 0;
                }
                else if( nc == Black ){
                    value += 0;
                }
                else if( nc == inputColor){
                    value -= 100;
                }
                else{
                    if( dist <= nd )
                        value += 1000;
                    else
                        value -= 1000;
                }
            }
            //cout << x << ", " << y << ": " << value << endl;

            if( value == 0 ){
                return ( dist == 1 ) ? -50*cnt : -dist;
            }
            else{
                return ( color[x][y] == White ) ? value-dist : value-10*dist;
            }
        }

        int count_color( Color color[5][6], Color target ){
            int cnt = 0;
            for(int i=0; i<5; i++)
                for(int j=0; j<6; j++)
                    if( color[i][j] == target )
                        cnt++;
            return cnt;
        }
        void next_board(int x, int y, int nxt_Record[5][6], Color nxt_color[5][6], int Record[5][6], int Max[5][6], Color color[5][6], Color inputColor){
            for(int i=0; i<5; i++)
                for(int j=0; j<6; j++)
                    nxt_Record[i][j] = Record[i][j], nxt_color[i][j] = color[i][j];
            
            nxt_Record[x][y]++;
            nxt_color[x][y] = inputColor;
            
            boom(x,y, nxt_Record, nxt_color, Max);
            return;
        }

        bool greater_than_near( int x, int y, int Record[5][6], int Max[5][6], Color color[5][6] ){
            if( !valid(x,y) ) return true;
            if( color[x][y] == Black ) return true;
            int now_d =  Max[x][y] - Record[x][y];

            for(int j = 0; j<4; j++){
                int near_x = x + dir_x[j], near_y = y + dir_y[j];
                if( !valid(near_x, near_y) ) continue;
                if( color[near_x][near_y] == Black ) continue;

                int D = Max[near_x][near_y] - Record[near_x][near_y];
                if( now_d <= D )
                    return false;
                
            }
            return true;

        }

        void boom(int x, int y, int nxt_Record[5][6], Color nxt_color[5][6], int Max[5][6] ){
            if( !valid(x,y) || nxt_Record[x][y] != Max[x][y] || nxt_color[x][y] == Black ) return;

            Color bcolor = nxt_color[x][y];
            nxt_color[x][y] = Black;

            for(int i=0; i<4; i++){
                int tx = x + dir_x[i], ty = y + dir_y[i];
                if( valid(tx,ty) && nxt_color[tx][ty] != Black){
                    nxt_color[tx][ty] = bcolor;
                    nxt_Record[tx][ty]++;
                    boom(tx, ty, nxt_Record, nxt_color, Max);
                }
            }
        }
    private:
        int x;
        int y;
};


int main(){
    int Record[5][6], Max[5][6];
    Color color[5][6];
    init(Record, Max, color);

    Student_ver1 p1;
    Student p2;
    Color now = Blue;
    fout << "Start" << endl;
    while( Playing ){
        int x, y;
        if( now == Blue ){
            p1.makeMove(Record, Max, color, now);
            x = p1.getX(), y = p1.getY();
        }
        else{
            p2.makeMove(Record, Max, color, now);
            x = p2.getX(), y = p2.getY();
        }
        Playing = make_step(x, y, now, Record, Max, color);
        now = ( now == Blue ) ? Red : Blue;

        //print_board(Record, color);
        //cout << ( now == Blue ? "Blue" : "Red" ) << endl;
        //system("Pause");
    }
    if( now == Red )
        fout << "Player Two Win !" << endl;
    else
        fout << "Player One Win !" << endl;
    fout.close();
    return 0;
}

void init(int Record[5][6], int Max[5][6], Color color[5][6] ){
    for(int i=0; i<5; i++){
        for(int j=0; j<6; j++){
            Record[i][j] = 0;
            Max[i][j] = 4;
            color[i][j] = White;

            if( i == 0 || i == 4 )
                Max[i][j]--;
            if( j == 0 || j == 5 )
                Max[i][j]--;
        }
    }
}

bool make_step(int x, int y, Color inputcolor, int Record[5][6], int Max[5][6], Color color[5][6] ){
    if ( !valid(x,y) ) return false;
    if ( !(color[x][y] == inputcolor || color[x][y] == White) ) return false;

    if( inputcolor == Blue )
        fout << "Player One : (" << x+1 << "," << y+1 << ")" << endl;
    else
        fout << "Player Two : (" << x+1 << "," << y+1 << ")" << endl;

    Record[x][y]++;
    color[x][y] = inputcolor;
    boom(x, y, Record, Max, color);
    return true;
}

void boom(int x, int y, int Record[5][6], int Max[5][6], Color color[5][6] ){
    if( !valid(x,y) || Record[x][y] != Max[x][y] || color[x][y] == Black) return;
    
    Color bcolor = color[x][y];
    color[x][y] = Black;

    for(int i=0; i<4; i++){
        int tx = x + dir_x[i], ty = y + dir_y[i];
        if( valid(tx,ty) && color[tx][ty] != Black){
            color[tx][ty] = bcolor;
            Record[tx][ty]++;
            boom(tx, ty, Record, Max, color);
        }
    }
}

void print_board( int Record[5][6], Color color[5][6] ){
    for(int i=0; i<5; i++){
        for(int j=0; j<6; j++){
            char c = ( color[i][j] == White ) ? ' ':
                     ( color[i][j] == Black ) ? 'X':
                     ( color[i][j] == Blue  ) ? 'B':
                     ( color[i][j] == Red   ) ? 'R': '?';
             
            if( c == 'R' || c == 'B' )
                cout << Record[i][j] << c << " \n"[j==5];
            else
                cout << c << c << " \n"[j==5];
        }
    }
}