#include "global.hpp"
#include "board.hpp"
#include "display.hpp"
#include "path.hpp"
#include "test.hpp"

#define MAIN_MENU_NUM 3
std::string mainMenu[MAIN_MENU_NUM] = {"PLAY", "LEADERBOARD" , "EXIT"};

#define PLAY_MENU_NUM 3
std::string playMenu[PLAY_MENU_NUM] = {"NORMAL MODE", "DIFFICULT MODE", "BACK"};

#define SIZE_MENU_NUM 3
std::string sizeMenu[SIZE_MENU_NUM] = {"2 X 3", "4 X 6", "6 X 8"};

using namespace std;

int main() {

    // Setup terminal
    initscr();

    raw();                 // using raw buffer
    noecho();              // to not print buffer on screen
    start_color();         // enable color support
    keypad(stdscr, TRUE);  // enable support for arrow keys
    curs_set(0);           // disable typing indicator

    init_pair(1, COLOR_CYAN, COLOR_WHITE);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);

    refresh();

    bool isRunning = true;
    while (isRunning) {
        switch (ChooseMenu(mainMenu, MAIN_MENU_NUM)) {
            case 0: // Play
            {
                int mode = -1;
                int sizeOption;

                switch(ChooseMenu(playMenu, PLAY_MENU_NUM)){
                    case 0: // normal mode
                        mode = MODE_NORMAL;
                        break;

                    case 1: // difficult mode
                        mode = MODE_DIFFICULT;
                        break;

                    case 2: // back
                        break;

                    default:
                        break;
                }

                // if mode is set
                if (mode != -1) {
                    int height, width;
                    
                    switch(ChooseMenu(sizeMenu, SIZE_MENU_NUM)) {
                        case 0:
                            height = 2; width = 3;
                            break;

                        case 1: // 4 x 6
                            height = 4; width = 6;
                            break;

                        case 2: // 6 x 8
                            height = 6; width = 8;
                            break;

                        default:
                            break;
                    }

                    int timeFinished;
                    switch(PlayGame(height, width, mode, timeFinished)) {
                        case ST_FORCE_OUT:
                            isRunning = false;
                            break;

                        case ST_SURRENDER:
                            DisplayEndScreen(ST_SURRENDER, height, width);
                            break;

                        case ST_FINISHED:
                            DisplayEndScreen(ST_FINISHED, height, width, timeFinished);
                            break;
                            
                        default:
                            break;
                    }
                }

                break;
            }

            case 1: // Leaderboard
                int height, width;
                    
                switch(ChooseMenu(sizeMenu, SIZE_MENU_NUM)) {
                    case 0:
                        height = 2; width = 3;
                        break;

                    case 1: // 4 x 6
                        height = 4; width = 6;
                        break;

                    case 2: // 6 x 8
                        height = 6; width = 8;
                        break;

                    default:
                        break;
                }

                DisplayLeaderboard(height, width);
                
                break;

            case 2: // Exit
                isRunning = false;
                break;
        }
    }

    clear();
    refresh();
    endwin();

    return 0;
}