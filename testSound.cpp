#include<iostream>
#include<windows.h>
#include<MMsystem.h>

using namespace std;


int main()
{
    bool played = PlaySound(TEXT("sound.wav"), NULL, SND_FILENAME);
    if(played)
        cout << "The song has been played";
    else
        cout << "The song has not been played";
    return;
}