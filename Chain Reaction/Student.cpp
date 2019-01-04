const int dir_x[4] = {1,-1,0,0};
const int dir_y[4] = {0,0,1,-1};

class Student{
    public:
        
        void makeMove(int Record[5][6], int Max[5][6], Color color[5][6], Color inputColor){
            
            vector<int> pos = this->possible_pos(Record, Max, color, inputColor);
            if( pos.size() == 0 ) return;

            int nxt_Record[5][6];
            Color nxt_color[5][6];

            this->x = pos[0]/10, this->y = pos[0]%10;
            Color now_c = color[x][y];
            int now_v = next_board( this->x, this->y, nxt_Record, nxt_color, Record, Max, color, inputColor );
            for(int i = 1; i<pos.size(); i++ ){
                int now_x = pos[i]/10, now_y = pos[i]%10;
                int value = next_board( now_x, now_y, nxt_Record, nxt_color, Record, Max, color, inputColor );
                //print_board(nxt_Record, nxt_color);
                //cout << value << endl;
                //cout << board_value(nxt_Record, Max, nxt_color ) << endl;
                //system("PAUSE");
                if( now_v > value && value%2 == 0 ){
                    if( now_c == inputColor && color[now_x][now_y] == White ){
                        this->x = now_x, this->y = now_y;
                        now_c = color[now_x][now_y];
                        now_v = value;
                    }
                }
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

        bool valid( int x, int y ){
            return( x >= 0 && x < 5 && y >= 0 && y < 6 );
        }

        int board_value(int Record[5][6], int Max[5][6], Color color[5][6]){
            int value = 0;
            for(int i=0; i<5; i++){
                for(int j=0; j<6; j++){
                    if( color[i][j] == Black ) continue;
                    //cout << Max[i][j] - Record[i][j] << " \n"[j==5];
                    value += Max[i][j] - Record[i][j];
                }
            }
            return value;
        }

        int next_board(int x, int y, int nxt_Record[5][6], Color nxt_color[5][6], int Record[5][6], int Max[5][6], Color color[5][6], Color inputColor){
            for(int i=0; i<5; i++)
                for(int j=0; j<6; j++)
                    nxt_Record[i][j] = Record[i][j], nxt_color[i][j] = color[i][j];
            
            nxt_Record[x][y]++;
            nxt_color[x][y] = inputColor;
            
            boom(x,y, nxt_Record, nxt_color, Max);

            return board_value(nxt_Record, Max, nxt_color);
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