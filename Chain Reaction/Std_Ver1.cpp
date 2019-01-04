/*
randomMove  O
onLook      X
heightof    O
rlawrenc    X
*/
class Student{
    public:
        void makeMove(int Record[5][6], int Max[5][6], Color color[5][6], Color inputColor){
            std::vector<int> pos = this->possible_pos(Record, Max, color, inputColor);

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
                    if( color[i][j] == inputColor || color[i][j] == White ){
                        pos.push_back(i*10 + j);
                    }
            return pos;
        }
    private:
        int x;
        int y;
};