#include <ncurses.h>
#include <vector>
#include <string>
#include <menu.h>
#include <cstring>
using namespace std;

void printOnMap(WINDOW* mapWin, string toPrint, int row, int col);
void setupWindows(WINDOW*& menuWin, WINDOW*& mapWin);
pair<int, int> getWindowSize(WINDOW*);
void printCenteredText(string str);
void printCenteredTextWait(string str);
string printCenteredTextForInput(string str);
void initialize();

const int MENU_HORIZ_MARGIN = 4;
const int MENU_VERT_MARGIN = 2;

int main() 
{
    std::vector<std::string> moveOptions = {
        "North",
        "East",
        "South",
        "West"
    };

    initialize();

    chgat(-1, A_BOLD, 0, NULL);
    printCenteredTextWait("Welcome, wanderer.");
    
    string playerName;
    playerName = printCenteredTextForInput("Tell me, what is your name? \n");

    WINDOW* menuWin;
    WINDOW* mapWin;
    setupWindows(menuWin, mapWin);
				
	int numberOfChoices = moveOptions.size();
    // create list of menu items
	ITEM **myItems = (ITEM **)calloc(numberOfChoices + 1, sizeof(ITEM *));

    // add list items to list... it probably doesn't make sense to define them in a vector...?
    // since we have to add them to this other "list" anyway.
    for(int i = 0; i < numberOfChoices; ++i)
    {
        myItems[i] = new_item(moveOptions[i].c_str(), "");
    }

    // I don't yet understand the point of this and it seems dangerous
    myItems[numberOfChoices] = (ITEM *)NULL;

    // used to track spacing down the page vertically
    int menuItemSpacing = 0;

    mvprintw(MENU_VERT_MARGIN+menuItemSpacing, MENU_HORIZ_MARGIN, ("Which direction do you wish to walk, " + playerName + "?").c_str());

    menuItemSpacing += 2;

    MENU *choicesMenu = new_menu((ITEM **)myItems);
	set_menu_win(choicesMenu, menuWin);
    set_menu_sub(choicesMenu, derwin(menuWin, 6, 38, MENU_VERT_MARGIN + menuItemSpacing, MENU_HORIZ_MARGIN));

    auto [startMapRow, startMapCol] = getWindowSize(mapWin);
    int player_row = startMapRow / 2;
    int player_col = (startMapCol - playerName.length()) / 2;
    printOnMap(mapWin, playerName, player_row, player_col);

    // print the exit prompt
    mvprintw(LINES - (MENU_VERT_MARGIN + 1), MENU_HORIZ_MARGIN, "F1 to Exit");

    // print the menu
	post_menu(choicesMenu);
	wrefresh(menuWin);

    int keyValue;
    ITEM *currentItem;
    const char *currentItemName;
    // control loop
	while((keyValue = getch()) != KEY_F(1))
	{   switch(keyValue)
	    {	
            case '\n':
                currentItem = current_item(choicesMenu);
                currentItemName = item_name(currentItem);
                if (strcmp(currentItemName, "North") == 0)
                {
                    player_row--;
                }
                else if (strcmp(currentItemName, "East") == 0)
                {
                    player_col++;
                }
                else if (strcmp(currentItemName, "South") == 0)
                {
                    player_row++;
                }
                else
                {
                    player_col--;
                }
                werase(mapWin);
                box(mapWin, 0, 0);
                printOnMap(mapWin, playerName, player_row, player_col);
                break;
            case KEY_DOWN:
		        menu_driver(choicesMenu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(choicesMenu, REQ_UP_ITEM);
				break;
		}

        wrefresh(menuWin);
	}	

	for(int i = 0; i < numberOfChoices; ++i)
	{
		free_item(myItems[i]);
	}
	free_menu(choicesMenu);

    endwin(); // End Curses mode

	return 0;
}

void printOnMap(WINDOW* mapWin, string toPrint, int row, int col)
{
    mvwprintw(mapWin, row, col, "%s", toPrint.c_str());
    wrefresh(mapWin);
}

void setupWindows(WINDOW*& menuWin, WINDOW*& mapWin)
{
    auto [ rows, cols ] = getWindowSize(stdscr);
    int menuWidth = cols / 3;
    int mapWidth = cols - menuWidth;

    menuWin = newwin(rows, menuWidth, 0, 0);
    mapWin = newwin(rows, mapWidth, 0, menuWidth);

    box (menuWin, 0, 0);
    box (mapWin, 0, 0);
    
    wrefresh(menuWin);
    wrefresh(mapWin);
}

pair<int, int> getWindowSize(WINDOW* win)
{
    /* to store the number of rows and  the number of colums of the screen */
    int row,col;			
    /* get the number of rows and columns */	
    getmaxyx(win,row,col);

    return { row, col };
}

void printCenteredText(string str)
{
    auto [row, col] = getWindowSize(stdscr);
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
    
    auto [row, col] = getWindowSize(stdscr);

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
