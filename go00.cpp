#include <bits/stdc++.h>
using namespace std;
char board[11][11];										//for testing and manuiplaiton
char p[11][11]={0}; 									//the playing board
int dx[4]={0,0,1,-1},dy[4]={1,-1,0,0};//up,down,right,left
std::queue<pair <int,int > > dir;
void display()
{
	//prints the playing board.
	for(int i=0;i<=9;i++){for(int j=0;j<=9;j++)cout<<p[i][j]<<' ';cout<<endl;}
}
void loogingmoves(int x,int y,bool move)
{
	 //log every move and who played in moves a txt file.
	FILE *logmove;
	logmove=fopen("moves.txt","a");
	char c;
	move?c='Y':c='X';
	fprintf(logmove, "%c move : %d %d\n",c,x,y);
	fclose(logmove);
}
void save(int turn)
{
	//save the in current p array in save.txt and whose turn
	int x,y;
	char c;
	FILE *newf=fopen("save.txt","w");
	fprintf(newf, "%d\n", turn);
	for(int i=1;i<=9;i++)
	{
		for (int j=1;j<=9;j++)
		{
			c=p[i][j];
			fprintf(newf, "%c",c);
		}
		fprintf(newf, "\n" );
	}
	cout<<"SAVED\n";
}
bool load()
{
	// copy the board from the save.txt to p-array
	//return false if the file doesn't exist
	char raw[20];
	char c;
	int turn;
	FILE *save = fopen("save.txt","r");
	if(save==NULL)
			{
				cout<<"Can't find any saved data\n";
				return false;
			}
	fscanf(save,"%d",&turn);
	for(int i=1;i<=9;i++)
	{

		fscanf(save,"%s",raw);
	for(int j=1;j<=9;j++)
	{p[i][j]=raw[j-1];}
	}
	fclose(save);
	cout<<"Loaded\n";
	display();
	return turn;
}
void loogingboard()
{
	//save the whole board in file.
	//the file will contiain the last satate of the lastboard
	//shall be used to to check ko state
	FILE *board;
	board=fopen("lastboard.dat","w");
	for(int i=1;i<=9;i++){for(int j=1;j<=9;j++)fprintf(board, "%c",p[i][j]);fprintf(board, "\n");}
	fclose(board);
}
void update(){for(int i=1;i<=9;i++)for(int j=1;j<=9;j++)if(board[i][j]=='V')p[i][j]='.';else p[i][j]=board[i][j];}
void copy()
{
	for(int i=1;i<=9;i++)for(int j=1;j<=9;j++)board[i][j]=p[i][j];
}

int valid(int a ,int b){return a>=1&&a<=9&&b>=1&&b<=9;} //check if a (a,b)belongs to the board
bool liberity(int a ,int b)
{
  //return true iff (a,b) has at least 1 liberity
	for(int i=0;i<4;i++)
	{
		if(!valid(a+dx[i],b+dy[i]))continue;
		 if(board[a+dx[i]][b+dy[i]]=='.')return true;
	}
	return false;
}
bool strliberity(int a,int b)
{
	//return true iff at least the string has on stone in liberity state.
	char c= board[a][b];
	std::queue<pair<int ,int > > q;
	q.push({a,b});
	int x,y;
	while(!q.empty())
	{
		int aa=q.front().first,bb=q.front().second;
		q.pop();
		if(liberity(aa,bb))return true;
		board[aa][bb]='V';
		for(int i=0;i<4;i++)
		{
			int x=aa+dx[i],y=bb+dy[i];
			if(!valid(x,y)||board[x][y]!=c)continue;
			q.push({x,y});

		}
	}
	return false;
}
void adj(int a,int b,char trplay)
{
	//determine which stones belongs to strin that can be captured
	int x,y;
	char c=board[a][b],inversc;
	(trplay=='X')?inversc='O':inversc='X';
	copy();
	for(int i=0;i<4;i++)
	{
		board[a][b]=trplay;
		x=a+dx[i];y=b+dy[i];
		if(valid(x,y)&&board[x][y]==inversc)
		{
			cout<<i<<endl;
			if(!strliberity(x,y))dir.push({x,y});
		}

	}
}
 void capture(int a,int b)
 {
	 //remove the string a,b belongs to.
	 char c=board[a][b];
	 queue<pair<int,int> > q;
	 q.push({a,b});
	 while(!q.empty())
	 {
		 a=q.front().first;b=q.front().second;
		 q.pop();
		 board[a][b]='V';
		 for(int i=0;i<4;i++)
		 {
			 int x=a+dx[i],y=b+dy[i];
			 char cur=board[x][y];
			 if(!valid(x,y)||cur!=c)continue;
			 q.push({x,y});
		 }
	 }
 }
 void floadfill(int x,int y,char c )
 {
	  //mark territories
   queue<pair <int,int> >q;
   q.push({x,y});
   while(!q.empty())
   {
     int xx = q.front().first,yy=q.front().second;
     q.pop();
     for(int i=0;i<4;i++)
     {
       int a = xx+dx[i],b=yy+dy[i];
       if(!valid(a,b))continue;
       if(board[a][b]=='X'||board[a][b]=='O'||board[a][b]=='D'||board[a][b]==tolower(c)||board[a][b]=='t')continue;
       if(board[a][b]=='.')board[a][b]=tolower(c);
       else if(board[a][b]=='x'&&c=='O')board[a][b]='t';
       else if(board[a][b]=='o'&&c=='X'){board[a][b]='t';}
       else {board[a][b]='D';}
       q.push({a,b});
     }
   }

 }
 void clcpoints()
 {
	 //count the number of stones and territories for each player.
	 //and show the winner.
	 copy();
	 int cnx=0,cny=0;
	 for(int i=1;i<=9;i++)for(int j=1;j<=9;j++)
	 {
		 if(board[i][j]=='X'||board[i][j]=='O'){floadfill(i,j,board[i][j]);}
	 }
	 for(int i=1;i<=9;i++)
	 {
		 for(int j=1;j<=9;j++)
		 {
			 if(tolower(board[i][j])=='x')++cnx;
			 else if(tolower(board[i][j])=='o')++cny;
		 }
	 }
	 if(cnx==cny)printf("draw\n");
	 else {cnx>cny?printf("X wins\n"):printf("O wins\n");}

	 printf("X %d O %d\n",cnx,cny);
	 cout<<"CREDITS\n";
	 cout<<"Developed by : Ahmed Amr\n";
 }
int main()
{
	system("cls");
	system("title go");
	system("color 02");
	fclose(fopen("moves.txt","w"));//discard all content of moves.txt
	string corr;
	int x,spare,y,nx,ny; //true-->x turns
	bool turn =true,checkinput=true,pass=false,validpass;
	char inp;
	string s="123456789";
	for(int i=0;i<=9;i++){p[0][i+1]=s[i];p[i+1][0]=s[i];} //numbering the raws and columns
	for(int i=1;i<=9;i++)for(int j=1;j<=9;j++)p[i][j]='.';
	while(true)
	{
		display();
		checkinput=false;
		while(!checkinput)
		{
			if(turn)cout<<"X turn :\n";
			else cout<<"O turn :\n";
			getline(cin,corr);
			if(corr=="load"){turn=load();continue;}
			if(corr=="save"){save(turn);return 0;}
			if(corr!="pass")
			 {
					if(corr.size()!=3){cout<<"invalid move...\n";continue;}
					if(corr[1]!=' '){cout<<"invalid move...\n";continue;}
					x=corr[0]-'0';
					y=corr[2]-'0';
				}
			else x=-1;
    	if(x==-1&&pass){clcpoints();return 0;}
		else if(x==-1&&!pass){pass=1;turn=!turn;checkinput=false;continue;}
		pass=0;

		if(p[x][y]!='.'||!valid(x,y)){cout<<"can\'t play there.\n";continue;}
		else checkinput=true;
		}

		if(turn){inp='X';turn=false;}
		else {inp='O';turn=true;}
		while(!dir.empty())dir.pop();
    adj(x,y,inp);
		if(dir.empty())
				{
					copy();
					board[x][y]=inp;
					if(strliberity(x,y))
					{
						validpass=true;
					}
					else {system("cls");cout<<"Can\'t play for selfcapture...\n";turn=!turn;validpass=false;}
				}
				else
				{
					while(!dir.empty())
					{
						copy();
						nx = dir.front().first;ny=dir.front().second;dir.pop();
						capture(nx,ny);
						update();
						validpass=true;

					}
				}
				if(validpass)
				{
					system("cls");
					p[x][y]=inp;
				loogingmoves(x,y,turn);
				loogingboard();
			  }
		}
}
