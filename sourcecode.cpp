#include<iostream>
#include<windows.h>
#include<conio.h>
#include<time.h>

#define POINT 10
using namespace std;
void swap(char &x, char &y)
{
    char temp;
    temp = x;
    x = y;
    y = temp;
}

void GenerateCharacters(char *arr)
{
    for(int i = 0; i < 16; i++)
        arr[i] = i/2 + 'A';
    
    for(int i = 0; i < 16; i++)
    {
        int n = rand() % 16;
        if(n != i)
            swap(arr[n], arr[i]);
    }
}

void gotoxy(short x, short y)
{
 HANDLE hConsoleOutput;
 COORD Cursor_an_Pos = { x, y };
 hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
 SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}

void DrawBoard(char a[][4])
{
 int temp = 10;
 int i = 0;
 while(i < 4)
 {
   for(int j = 0; j < 4; j++)
      {
         gotoxy(POINT + j*11, temp);
         cout << " --------- ";
         gotoxy(POINT + j*11, temp+1);
         cout << "|         |";
         gotoxy(POINT + j*11, temp+2);
         cout << "|    "<< a[i][j] <<"    |";
         gotoxy(POINT + j*11, temp+3);
         cout << "|         |";
         gotoxy(POINT + j*11, temp+4);
         cout << " --------- ";
      }
    temp += 5;
    i++;
 }

}
void PrintArray(char arr[][4])
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
            cout << "   " << arr[i][j];
    cout << endl;
    }
}

int main()
{
    char arr[4][4];
    srand(time(0));
    GenerateCharacters((char*)arr);
    //PrintArray(arr);

    DrawBoard(arr);
    getch();
}