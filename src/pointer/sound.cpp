#include"sound.hpp"

//Win or lose
void WinSound()
{
    PlaySound(TEXT("resources/Winning.wav"), NULL, SND_FILENAME);
}

void LoseSound()
{
    PlaySound(TEXT("resources/Losing.wav"), NULL, SND_FILENAME);
}

//Correct or incorrect
void CorrectSound()
{
    PlaySound(TEXT("resources/Correct.wav"), NULL, SND_FILENAME);
}

void ErrorSound()
{
    PlaySound(TEXT("resources/Error.wav"), NULL, SND_FILENAME);
}

//Moving between cards
void MovingSound()
{
    PlaySound(TEXT("resources/Moving.wav"), NULL, SND_FILENAME);
}