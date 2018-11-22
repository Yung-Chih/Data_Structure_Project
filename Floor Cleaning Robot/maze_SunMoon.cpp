#include<bits/stdc++.h>
using namespace std;
struct P{
	P(int q,int w,int e,int r):a(q),b(w),x(e),y(r){}
	int a,b;
	int x,y;
};
char s[105][105];
int id[105][105],cnt;
int st[100005];
vector<P>p;

ofstream fout( "maze.data", ios::out);

inline int find(int x){
	return st[x]==x?x:st[x]=find(st[x]);
}
inline void print(int n,int m){
	for(int i=0;i<n;++i){
		for(int j=0;j<m;++j){
			fout << s[i][j];

		}
		fout << endl;
	}
}
int main(){
	int n=100,m=100;
	fout << n << " " << m << " " << 100 << endl;
	for(int i=0;i<n;++i){
		for(int j=0;j<m;++j){
			s[i][j]='1';
		}
	}
	for(int i=1;i<n;i+=2){
		for(int j=1;j<m;j+=2){
			id[i][j]=++cnt;
			s[i][j]='0';
		}
	}
	for(int i=1;i<n-1;++i){
		for(int j=1;j<m-1;++j){
			if(s[i][j]== '1'){
				if(s[i][j-1]!= '1'&&s[i][j+1]!= '1'){
					p.push_back(P(id[i][j-1],id[i][j+1],i,j));
				}else if(s[i-1][j]!= '1'&&s[i+1][j]!= '1'){
					p.push_back(P(id[i-1][j],id[i+1][j],i,j));
				}
			}
		}
	}
	for(int i=1;i<=cnt;++i)st[i]=i;
	srand(time(0));
	random_shuffle(p.begin(),p.end());
	for(int i=0;i<(int)p.size();++i){
		if(find(p[i].a)!=find(p[i].b)){
			st[find(p[i].a)]=find(p[i].b);
			s[p[i].x][p[i].y]= '0';
		}
	}
	print(n,m);
	return 0;
}
