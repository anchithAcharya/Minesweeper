#include <iostream>
#include <conio.h>
#include <vector>
#include <time.h>
#include <Windows.h>

using namespace std;

#define RED "\33[31m"
#define ORANGE "\33[38;5;208m"
#define GREEN "\33[32m"
#define YELLOW "\33[38;5;220m"
#define BLUE "\33[34m"
#define PINK "\33[38;5;197m"
#define MAGENTA "\33[35m"
#define CYAN "\33[36m"
#define INDIGO "\33[38;5;135m"

#define FLAG_RED "\33[38;5;196m"
#define GRAY "\33[38;5;240m"
#define RESET "\33[m"

#define CURSOR_RESET printf("\033[%d;%dH",1,1);

#define SAFE_INTINPUT(condition,statements)\
{\
	if(!(condition))\
	{\
		cin.clear();\
		while(cin.get()!='\n');\
		statements\
	}\
}

#define PRINT_INCOLOUR(arg,colour)\
{\
	switch(arg)\
				{\
					case -5:	cout<<INDIGO<<'+'<<RESET;\
								break;\
\
					case -1:	cout<<str<<(char) ch<<RESET;\
								break;\
\
					case  0:	cout<<colour<<(char) 250<<RESET;\
								break;\
\
					case  1:	cout<<BLUE<<colour<<y.count<<RESET;\
								break;\
\
					case  2:	cout<<GREEN<<colour<<y.count<<RESET;\
								break;\
\
					case  3:	cout<<RED<<colour<<y.count<<RESET;\
								break;\
\
					case  4:	cout<<MAGENTA<<colour<<y.count<<RESET;\
								break;\
\
					case  5:	cout<<YELLOW<<colour<<y.count<<RESET;\
								break;\
\
					case  6:	cout<<CYAN<<colour<<y.count<<RESET;\
								break;\
\
					case  7:	cout<<PINK<<colour<<y.count<<RESET;\
								break;\
\
					case  8:	cout<<ORANGE<<colour<<y.count<<RESET;\
								break;\
\
					default:	cout<<RED<<"ER"<<RESET;\
				}\
}

#define INC_IFVALID(x,y)\
{\
	if(x && x##y)\
	{\
		if(x##y->count>=0) x##y->count++;\
	}\
}

#define REVEAL_IFVALID(x,y)\
{\
	if(x && x##y)\
	{\
		if(x##y->hidden) reveal(x##y);\
	}\
}

#define FLAG_COUNT(x,y)\
{\
	if(x && x##y)\
	{\
		if(x##y->flag) flags++;\
	}\
}

#define FOR_ALL_NEIGHBOURS(ptr,macro_name)\
{\
	if(ptr->spl!='1' && ptr->spl!='2') macro_name(ptr->top,)\
	if(ptr->spl!='3' && ptr->spl!='4') macro_name(ptr->bottom,)\
	if(ptr->spl!='1' && ptr->spl!='3') macro_name(ptr->left,)\
	if(ptr->spl!='2' && ptr->spl!='4') macro_name(ptr->right,)\
	\
	switch(ptr->spl)\
	{\
		case '1':	macro_name(ptr->bottom,->right)\
					break;\
	\
		case '2':	macro_name(ptr->bottom,->left)\
					break;\
	\
		case '3':	macro_name(ptr->top,->right)\
					break;\
	\
		case '4':	macro_name(ptr->top,->left)\
					break;\
	\
		default:	if((ptr->top&&ptr->top->spl=='1')||(ptr->bottom&&ptr->bottom->spl=='3'))\
					{\
						macro_name(ptr->top,->right)\
						macro_name(ptr->bottom,->right)\
					}\
	\
					else if((ptr->top&&ptr->top->spl=='2')||(ptr->bottom&&ptr->bottom->spl=='4'))\
					{\
						macro_name(ptr->top,->left)\
						macro_name(ptr->bottom,->left)\
					}\
	\
					else\
					{\
						macro_name(ptr->top,->right)\
						macro_name(ptr->top,->left)\
						macro_name(ptr->bottom,->left)\
						macro_name(ptr->bottom,->right)\
					}\
	}\
}

class CELL
{
	public:
			char spl;
			int count;
			bool hidden,flag=0;
			CELL *top,*bottom,*left,*right;

			CELL(char z='\0',int x=0,bool y=true)
			{
				count=x;
				hidden=y;
				spl=z;
				
				top=bottom=left=right=NULL;
			}
};

class GRID
{
	public:vector<vector<CELL>> grid;

	public: 

			GRID(int row=10,int col=10,int def=0)
			{
				CELL temp('\0',def);
				vector<vector<CELL>> myvector(row,vector<CELL>(col,temp));
				
				grid=myvector;
				grid.shrink_to_fit();

				setup();
			}

			void show(int,const char *,int) const;
			CELL * input();
			CELL * initialize(CELL *);
			void setup();
			bool checkwin(CELL *);
			void reveal(CELL *);
			void explode(CELL *);
};

int ROW=10;
int COL=10;
int MAX_MINES=15;

bool _X_RAY_VISION_=false;
bool SHOW_EXPLOSION=true;
bool PTR_CORNER_JMP=true;
bool INSTANT_REVEAL=false;

CELL *cellptr=NULL;

bool play();
void settings();

//TODO:multithreaded reveal

int main()
{
	char choice;

	while(1)
	{
		cout<<"\n1:Play    2:Settings\n[0:Exit]\n";
		cout<<"Enter your choice:";
		cin>>choice;

		switch(choice)
		{
			case '1':	while(play());
						break;

			case '2':	settings();
						break;

			case 'x':	_X_RAY_VISION_^=1;
						break;

			case '0':	exit(0);

			default:	cout<<"\nInvalid input. Please try again.\n";
		}
	}

	return 0;
}

void settings()
{
	int m=0,n=0;

	system("CLS");

	while(1)
	{
		cout<<"\n1:Change difficulty    2:Change grid size    3.Change number of mines\n4.Toggle explosion animation    5.Toggle pointer corner warping    6.Toggle instant reveal\n";
		cout<<"[0:Go back to main menu]\n";
		cout<<"Enter your choice:";
		SAFE_INTINPUT(cin>>n,cout<<RED<<"\nInvalid input"<<RESET<<endl;continue;)

		switch(n)
		{
			case 1 :	cout<<"\nSelect difficulty: 1:Easy    2:Normal(Default)    3:Medium    4:Hard";
						
						while(1)
						{
							cout<<"\nEnter your choice [0:Cancel]: ";
							SAFE_INTINPUT(cin>>m,m=-1;)

							switch(m)
							{
								case 1 :	ROW=COL=5;
											MAX_MINES=10;

											cout<<GREEN<<"\nDifficulty successfully set to Easy."<<RESET<<endl;
											break;

								case 2 :	ROW=COL=10;
											MAX_MINES=15;

											cout<<GREEN<<"\nDifficulty successfully set to Normal."<<RESET<<endl;
											break;

								case 3 :	ROW=COL=20;
											MAX_MINES=75;

											cout<<GREEN<<"\nDifficulty successfully set to Medium."<<RESET<<endl;
											break;

								case 4 :	ROW=COL=45;
											MAX_MINES=1000;

											cout<<GREEN<<"\nDifficulty successfully set to Hard."<<RESET<<endl;
											goto ir_ask;

								case 0 :	break;

								default:	cout<<RED<<"\nInvalid choice. Please try again."<<RESET<<endl;
							}

							if(m==0) break;
						}

						break;
			
			case 2 :	cout<<"\nCurrent grid size: "<<ROW<<" x "<<COL<<endl;
						cout<<"New grid size must not be lesser than 5 x 5 and greater than 50 x 50.";
						
						while(1)
						{
							
							cout<<"\nEnter the new grid size [Enter a negative value to cancel]: ";
							
							SAFE_INTINPUT(cin>>m>>n,cout<<RED<<"\nInvalid input"<<RESET<<endl;continue;)
																					
							if(m<0 || n<0)
							{
								cout<<RED<<"\nOperation cancelled."<<RESET<<endl;
								break;
							}

							else if((m>=5 && n>=5) && (m<=50 && n<=50))
							{
								ROW=m;
								COL=n;

								cout<<GREEN<<"\nGrid size successfully changed to: "<<ROW<<" x "<<COL<<RESET<<endl;
								break;
							}

							else cout<<RED<<"Error: "<<RESET<<"New grid size must not be lesser than 5 x 5 and greater than 50 x 50."<<endl;
						}
						
						if((m>25 || n>25) && !INSTANT_REVEAL)
						{
ir_ask:						cout<<"\nYour grid size is quite big. Would you like to turn on "<<BLUE<<"Instant Reveal "<<RESET<<"feature for better performance?";
							cout<<"\nPress '1' to turn it on [anything else to skip]:";
							SAFE_INTINPUT(cin>>n,n=2;)

							if(n!=1) 
							{
								cout<<BLUE<<"\nInstant Reveal "<<RESET<<"not turned on."<<endl;
								break;
							}

							goto ir_toggle;
						}

						else break;
			
			case 3 :	cout<<"\nCurrent grid size: "<<ROW<<" x "<<COL<<endl;
						cout<<"Current number of mines: "<<MAX_MINES<<endl;
						cout<<"\nNew value for the number of mines must not be greater than "<<ROW*COL;
				
						while(1)
						{
							cout<<"\nEnter the new value [Enter a negative value to cancel]: ";
							SAFE_INTINPUT(cin>>n,cout<<RED<<"\nInvalid input"<<RESET<<endl;continue;)
							
							if(n<0)
							{
								cout<<RED<<"\nOperation cancelled."<<RESET<<endl;
								break;
							}

							else if(n<=(ROW*COL))
							{
								MAX_MINES=n;
								cout<<GREEN<<"\nNumber of mines successfully changed to "<<MAX_MINES<<RESET<<endl;
								break;
							}

							else
							{
								cout<<RED<<"\nNumber of mines exceed grid capacity."<<RESET<<endl;
								cout<<"New value for the number of mines must not be greater than "<<ROW*COL<<endl;
							}
						}

						break;

			case 4 :	cout<<"\nMine explosion animation toggled ";
						
						if(SHOW_EXPLOSION=!SHOW_EXPLOSION)
							cout<<GREEN<<"ON"<<RESET<<endl;

						else cout<<RED<<"OFF"<<RESET<<endl;

						break;

			case 5 :	cout<<"\nCorner jumping of pointer toggled ";

						if(PTR_CORNER_JMP=!PTR_CORNER_JMP)
							cout<<GREEN<<"ON"<<RESET<<endl;

						else cout<<RED<<"OFF"<<RESET<<endl;

						break;

ir_toggle:	case 6 :	cout<<"\nInstant revealing toggled ";

						if(INSTANT_REVEAL=!INSTANT_REVEAL)
							cout<<GREEN<<"ON"<<RESET<<endl;

						else cout<<RED<<"OFF"<<RESET<<endl;

						break;

			case 0 :	return;

			default:	cout<<RED<<"\nInvalid input. Please try again.\n"<<RESET;
		}
	}

	system("CLS");
}

void GRID::show(int SLP_dur=0,const char *str=GRAY,int ch=77) const
{
	CURSOR_RESET

	for(auto x:grid)
	{
		for(auto y:x)
		{
			if(y.flag && y.count!=-5)
			{
				if(y.hidden) cout<<RED<<'F'<<RESET;
				
				else cout<<FLAG_RED<<'M'<<RESET;
			}

			else if(y.hidden && y.count!=-5)
			{
				if(_X_RAY_VISION_) PRINT_INCOLOUR(y.count,GRAY)
				else cout<<(char) 254;
			}

			else
			{
				PRINT_INCOLOUR(y.count,"")
			}

			cout<<" ";
		}

		cout<<endl;
	}

	Sleep(SLP_dur);
}

CELL * GRID::input()
{
	int temp=0;
	char dir='\0';
		
	while(1)
	{
		temp=cellptr->count;
		cellptr->count=-5;

		system("CLS");
		show();
		cellptr->count=temp;

		cout<<"WASD to move, ENTER to reveal, 'f' to flag/unflag, 'e' to reveal area\n[Esc:return to main menu]";
		dir=_getch();

		switch(dir)
		{
			case 'w' :	if(cellptr->top)
							cellptr=cellptr->top;
				
						break;

			case 's' :	if(cellptr->bottom)
							cellptr=cellptr->bottom;

						break;

			case 'a' :	if(cellptr->left)
							cellptr=cellptr->left;

						break;

			case 'd' :	if(cellptr->right)
							cellptr=cellptr->right;

						break;

			case 'f' :	if(cellptr->hidden) 
							cellptr->flag^=1;

						show(200);
						break;

			case '\n':
			case '\r':	if(!cellptr->flag)
						{
							if(cellptr->hidden) return cellptr;
						}

						else
						{
							cout<<RED<<"\nCannot reveal flagged cell."<<RESET;
							Sleep(300);
						}

					 	break;


			case 'e' :	if(!cellptr->flag)
						{
							if(!cellptr->hidden && cellptr->count>0)
							{
								int flags=0;
								
								FOR_ALL_NEIGHBOURS(cellptr,FLAG_COUNT)

								if(cellptr->count==flags)
								{
									FOR_ALL_NEIGHBOURS(cellptr,REVEAL_IFVALID)
									return NULL;
								}

								else
								{
									cout<<RED<<"\nYou must mark the number of surrounding mines first."<<RESET;
								}
							}

						}

						else
						{
							cout<<RED<<"\nCannot reveal flagged cell."<<RESET;
							Sleep(300);
						}

					 	break;

			case  27 :	cout<<"\nPress 'Esc' again to confirm, 'r' to restart,anything else to cancel: ";
						dir=_getch();

						if(dir==27 || dir =='r')
						{
							cellptr->spl=dir;
							return cellptr;
						}

						else break;

			case 'x':	_X_RAY_VISION_^=1;
						break;

			default:	cout<<RED<<"\nInvalid choice. Please try again.\n"<<RESET;
						Sleep(300);
				
		}
	}
}

void GRID::setup()
{
	cellptr=&grid[0][0];
	
	for(int x=1;x<ROW-1;x++)
	{
		for(int y=1;y<COL-1;y++)
		{
			grid[x][y].top=&grid[x-1][y];
			grid[x][y].bottom=&grid[x+1][y];
			grid[x][y].left=&grid[x][y-1];
			grid[x][y].right=&grid[x][y+1];
		}
	}

	for(int x=0;x<ROW-1;x++)
	{
		grid[x][0].right=&grid[x][1];
		grid[x][0].bottom=&grid[x+1][0];

		grid[x][COL-1].left=&grid[x][COL-2];
		grid[x][COL-1].bottom=&grid[x+1][COL-1];

		grid[x+1][0].top=&grid[x][0];
		grid[x+1][COL-1].top=&grid[x][COL-1];
	}

	for(int x=0;x<COL-1;x++)
	{
		grid[0][x].right=&grid[0][x+1];
		grid[0][x].bottom=&grid[1][x];

		grid[ROW-1][x].right=&grid[ROW-1][x+1];
		grid[ROW-1][x].top=&grid[ROW-2][x];

		grid[0][x+1].left=&grid[0][x];
		grid[ROW-1][x+1].left=&grid[ROW-1][x];
	}

	grid[0][COL-1].bottom=&grid[1][COL-1];
	grid[ROW-1][COL-1].top=&grid[ROW-2][COL-1];

	grid[ROW-1][0].right=&grid[ROW-1][1];
	grid[ROW-1][COL-1].left=&grid[ROW-1][COL-2];

	//special manouvers:

	if(PTR_CORNER_JMP)
	{
		grid[0][0].spl='1';
		grid[0][COL-1].spl='2';
		grid[ROW-1][0].spl='3';
		grid[ROW-1][COL-1].spl='4';

		grid[0][0].top=&grid[ROW-1][0];
		grid[0][0].left=&grid[0][COL-1];

		grid[0][COL-1].top=&grid[ROW-1][COL-1];
		grid[0][COL-1].right=&grid[0][0];

		grid[ROW-1][0].bottom=&grid[0][0];
		grid[ROW-1][0].left=&grid[ROW-1][COL-1];

		grid[ROW-1][COL-1].bottom=&grid[0][COL-1];
		grid[ROW-1][COL-1].right=&grid[ROW-1][0];
	}
}

CELL * GRID::initialize(CELL *ptr)
{
	if(MAX_MINES==0) return ptr;

	int set=-1,limit=MAX_MINES;

	if(MAX_MINES>((ROW*COL)/2))
	{
		int x,y;
		
		for(x=0;x<ROW;x++)
		{
			for(y=0;y<COL;y++)
			{
				if(ptr==&grid[x][y]) goto out;
			}
		}
		
out:	this->GRID::GRID(ROW,COL,-1);

		if(MAX_MINES==(ROW*COL)) return &grid[x][y];

		limit=(ROW*COL)-MAX_MINES-1;
		cellptr=ptr=&grid[x][y];
		ptr->count=0;
		set=0;
	}
	
	srand(time(0));

	for(int i=0;i<limit;i++)
	{
		int x=rand()%ROW;
		int y=rand()%COL;

		if((ptr==&grid[x][y]) || (grid[x][y].count==set))
		{
			i--;
			continue;
		}

		else
		{
			grid[x][y].count=set;
		}
	}

	show();

	for(int x=0;x<ROW;x++)
	{
		for(int y=0;y<COL;y++)
		{
			if(grid[x][y].count==-1)
			{
				CELL *ptr=&grid[x][y];
				FOR_ALL_NEIGHBOURS(ptr,INC_IFVALID)
			}
		}
	}

	return ptr;
}

void GRID::reveal(CELL *ptr)
{	
	if(!ptr || ptr->flag) return;
	
	if(ptr->count==-1)
	{
		ptr->hidden=0;
		show();

		return;
	}

	else if(ptr->count==0)
	{
		ptr->hidden=0;
		FOR_ALL_NEIGHBOURS(ptr,REVEAL_IFVALID)
		return;
	}

	else if(ptr->count<-1)
		return;

	else ptr->hidden=0;

	if(!INSTANT_REVEAL) show();
}

bool GRID::checkwin(CELL *ptr)
{
	int count=0;

	if(ptr->count==-1)
	{
		ptr->hidden=0;
		show(600);

		if(SHOW_EXPLOSION) explode(ptr);

		cout<<"You lose!";
		Sleep(500);

		for(auto &x:grid)
		{
			for(auto &y:x)
			{
				if(y.count==-1) y.hidden=0;
			}
		}

		show();
		cout<<"You lose!"<<endl;
		
		return 1;
	}

	else
	{
		for(auto x:grid)
		{
			for(auto y:x)
			{
				if(y.hidden && y.count!=-1)
					count++;
			}
		}

		if(count==0 || MAX_MINES==0)
		{
			show();
			cout<<"You win!";

			return 1;
		}

		else return 0;
	}
}

void GRID::explode(CELL *ptr)
{
	for(int i=0;i<3;i++)
	{
		show(50,RED);
		show(50);
	}

	show(100,ORANGE);
	show(150,"\33[38;5;220;48;5;196m",32);
	show(300,"\33[48;5;196m",32);
	show(200,RED,178);
	show(200,RED,177);
	show(200,RED,176);
	show();
}

bool play()
{
	CELL *ptr=NULL;
	GRID mygrid(ROW,COL);

	system("CLS");
	
	if((ptr=mygrid.input())->spl==27) return 0;
	else if(ptr->spl=='r') return 1;

	ptr=mygrid.initialize(ptr);
	mygrid.reveal(ptr);

	while(!ptr || !mygrid.checkwin(ptr))
	{
		if(ptr=mygrid.input())
		{
			switch(ptr->spl)
			{
				case 27:	return 0;

				case 'r':	return 1;

				default:	mygrid.reveal(ptr);
			}
		}

		system("CLS");
		if(INSTANT_REVEAL) mygrid.show();
	}

	return 0;
}
