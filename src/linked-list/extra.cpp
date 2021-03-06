#include "extra.hpp"

using namespace std;

//Win or lose
void WinSound()
{
    PlaySound(TEXT("resources/Winning.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void LoseSound()
{
    PlaySound(TEXT("resources/Losing.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

//Correct or incorrect
void CorrectSound()
{
    PlaySound(TEXT("resources/Correct.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void ErrorSound()
{
    PlaySound(TEXT("resources/Error.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

//Moving between cards
void MovingSound()
{
    PlaySound(TEXT("resources/Moving.wav"), NULL, SND_FILENAME | SND_ASYNC);
}


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

void UpdateLeaderboard(Stat player, int height, int width)
{
    string path = ORG_PATH + to_string(height) + "x" + to_string(width) + ".bin";

    ofstream ofs;
    ofs.open(path, ios::out | ios::app | ios::binary);

    if(!ofs.is_open()) return;
    
    ofs.write(reinterpret_cast<char*> (&player), sizeof(Stat));

    ofs.close();
}

Stat *ReadLeaderboard(int height, int width, int &size)
{
    string path = ORG_PATH + to_string(height) + "x" + to_string(width) + ".bin";
    
    ifstream ifs;
    ifs.open(path, ios::in | ios::binary);

    //If the file does not exist
    if(!ifs) return NULL;
    
    //Else
    ifs.seekg(0, ios::end);
    int sizeOfBytes = ifs.tellg();
    
    //If the contents of the file is empty
    if(sizeOfBytes == 0) return NULL;

    //Else
    size = sizeOfBytes/sizeof(Stat);
    Stat *leaderboard = new Stat [size];

    string temp_name;
    string temp_time;

    ifs.seekg(0, ios::beg);
    for(int i = 0; i < size; i++)
    {
        ifs.read(reinterpret_cast<char*> (&leaderboard[i]), sizeof(Stat));
    }

    //Sort the leaderboard
    SortAscendingOrder(leaderboard, size);

    ifs.close();
    return leaderboard;
}

Time GetCurrTime() {
    return chrono::system_clock::now();
}

int ElapsedTime(Time end, Time start) {
    chrono::duration<double> time = end - start;
    
    return int(time.count());
}