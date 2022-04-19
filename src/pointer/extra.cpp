#include "extra.hpp"


void Swap(Stat &x, Stat &y)
{
 Stat temp;
 temp = x;
 x = y;
 y = temp;
}

// This is a bubble sort
void SortAscendingOrder(Stat *a, int n)    
{
 int count;
 for(int i = 0; i < n - 1; i++){
     count = 0;

     for(int j = n - 1; j > i; j--){
        if(a[j].time < a[j-1].time){
            Swap(a[j], a[j - 1]);
            count++;
         }

        else if(a[j].name < a[j - 1].name){
            Swap(a[j], a[j - 1]);
            count++;
        }
     }
     if(count == 0)
        return;
 }
}

void UpdateLeaderboard(Stat player)
{
    ofstream ofs;
    ofs.open("data/leaderboard.csv", ios::out | ios::app);

    if(!ofs.is_open())
    {
        cout << "Cannot open the file";
        return;
    }
    
    ofs << player.name << "/";
    ofs << player.time << "\n";

    ofs.close();
}

Stat *ReadLeaderboard()
{
    ifstream ifs;
    ifs.open("data/leaderboard.csv");

    //If the file does not exist
    if(!ifs.is_open())
    {
        cout << "Cannot open the file";
        return;
    }

    //Else
    ifs.seekg(0, ios::beg);
    int sizeOfBytes = ifs.tellg();
    
        //If the contents of the file is empty
    if(sizeOfBytes == 0)
    {
        cout << "The file is empty";
        return;
    }

        //Else
    int size = sizeOfBytes/sizeof(Stat);
    Stat *leaderboard = new Stat [size];

    string temp_name;
    string temp_time;

    for(int i = 0; i < size; i++)
    {
        getline(ifs, temp_name, '/');
        getline(ifs, temp_time, '\n');

        leaderboard[i].name = temp_name;
        leaderboard[i].time = stoi(temp_time);
    }

    //Sort the leaderboard
    SortAscendingOrder(leaderboard, size);

    ifs.close();
    return leaderboard;
}