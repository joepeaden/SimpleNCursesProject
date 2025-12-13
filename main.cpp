#include <ncurses.h>
#include <vector>
#include <string>
using namespace std;

pair<int, int> getScreenSize()
{
    /* to store the number of rows and  the number of colums of the screen */
    int row,col;			
    /* get the number of rows and columns */	
    getmaxyx(stdscr,row,col);

    return { row, col };
}

void printCenteredText(string str)
{
    auto [row, col] = getScreenSize();
    mvprintw(row/2,(col-strlen(str.c_str()))/2,"%s",str.c_str());

	refresh();			/* Print it on to the real screen */
}

void printCenteredTextWait(string str)
{
    printCenteredText(str);
	getch();			/* Wait for user input */
}

string printCenteredTextForInput(string str)
{
    printCenteredText(str);
    
    auto [row, col] = getScreenSize();

    echo();
    curs_set(1);

    char inputStr[100];
    // getstr gets input from the user and waits for return key
    mvgetstr(row/2 + 1, col/2, inputStr);
}

void initialize()
{
    initscr(); // Start Curses mode
    
    // removes "line buffering" - not really sure, but I think so we can handle all
    // key input ourselves.       
    raw();   

    // This stops echo of characters on screen as the user inputs them I think
    noecho();
    // hide cursor for now 
    curs_set(0);

    // allows special keys
    keypad(stdscr, TRUE);
}

int main() 
{
    std::vector<std::string> options = {
        "Start Game",
        "Load Game",
        "Options",
        "Quit"
    };

    initialize();

    printCenteredTextWait("Welcome, warrior.");
    printCenteredTextForInput("Tell me, before you go off to die - what is your name? \n");

    int selected = 0;

    while (true) {
        clear();

        for (int i = 0; i < options.size(); ++i) {
            if (i == selected) {
                attron(A_REVERSE);
            }

            mvprintw(5 + i, 10, options[i].c_str());

            if (i == selected) {
                attroff(A_REVERSE);
            }
        }

        int ch = getch();

        if (ch == KEY_UP) {
            selected = (selected - 1 + options.size()) % options.size();
        }
        else if (ch == KEY_DOWN) {
            selected = (selected + 1) % options.size();
        }
        else if (ch == '\n') {
            break;
        }
    }

    endwin(); // End Curses mode

    // selected now contains the chosen option index
    // return selected;
	return 0;
}