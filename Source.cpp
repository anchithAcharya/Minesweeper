#include <iostream>
#include<conio.h>
#include <Windows.h>
#include<vector>

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

#define GRAY "\33[38;5;240m"
#define RESET "\33[m"


void show(vector<vector<char>> grid,int slp=0,const char *str="")
{
	system("CLS");

	for(auto x:grid)
	{
		for(auto y:x)
		{
			switch(y)
			{
				case 'R':cout<<"\33[38;5;196m"<<(char) 178<<RESET;
					break;

				case 'q':cout<<"\33[38;5;196m"<<(char) 176<<RESET;
					break;

				case 'r':cout<<"\33[38;5;196m"<<(char) 177<<RESET;
						 break;

				case 's':cout<<RED<<'M'<<RESET;
						 break;

				case 'o':cout<<"\33[38;5;228;48;5;202m"<<'M'<<RESET;
						 break;

				case 'm':cout<<GRAY<<'M'<<RESET;
					break;

				case 'O':cout<<"\33[38;5;166m"<<'M'<<RESET;
					break;

				case 'p':cout<<"\33[38;5;220;48;5;196m"<<' '<<RESET;
					break;

				case 'B':cout<<"\33[48;5;196m"<<' '<<RESET;
					break;

				default: cout<<y;
			}

			cout<<' ';
		}

		cout<<endl;
	}

	Sleep(slp);
}

int main()
{
	
	cout<<RED<<GRAY<<"asfdf"<<RESET;
	cout<<"dfdf";

	return 0;
}