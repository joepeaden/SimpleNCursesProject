#include <ncurses.h>
#include <vector>
#include <string>
using namespace std;

void setupWindows();
pair<int, int> getScreenSize();
void printCenteredText(string str);
void printCenteredTextWait(string str);
string printCenteredTextForInput(string str);
void initialize();

int main() 
{
    std::vector<std::string> options = {
        "Start Game",
        "Load Game",
        "Options",
        "Quit"
    };

    initialize();

    chgat(-1, A_BOLD, 0, NULL);
    // attron(A_STANDOUT);
    printCenteredTextWait("Welcome, warrior.");

    // attroff(A_STANDOUT);

    printCenteredTextForInput("Tell me, what is your name? \n");

    // auto [ height, width ]  = getScreenSize();
	// my_win = create_newwin(height, width / 3, starty, startx);

    int selected = 0;

    setupWindows();

    getch();

    // while (true) {
    //     clear();

    //     for (int i = 0; i < options.size(); ++i) {
    //         if (i == selected) {
    //             attron(A_REVERSE);
    //         }

    //         mvprintw(5 + i, 10, options[i].c_str());

    //         if (i == selected) {
    //             attroff(A_REVERSE);
    //         }
    //     }

    //     int ch = getch();

    //     if (ch == KEY_UP) {
    //         selected = (selected - 1 + options.size()) % options.size();
    //     }
    //     else if (ch == KEY_DOWN) {
    //         selected = (selected + 1) % options.size();
    //     }
    //     else if (ch == '\n') {
    //         break;
    //     }
    // }

    endwin(); // End Curses mode

    // selected now contains the chosen option index
    // return selected;
	return 0;
}

void setupWindows()
{
    auto [ rows, cols ] = getScreenSize();
    int menuWidth = cols / 3;
    int mapWidth = cols - menuWidth;

    WINDOW* menuWin;
    WINDOW* mapWin;

    menuWin = newwin(rows, menuWidth, 0, 0);
    mapWin = newwin(rows, mapWidth, 0, menuWidth);

    box (menuWin, 0, 0);
    box (mapWin, 0, 0);
    
    wrefresh(menuWin);
    wrefresh(mapWin);
}

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

    noecho();
    curs_set(0);

    return inputStr;
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
