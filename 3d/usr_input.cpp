#include <ncurses.h> //stdio include in curses
//#include <stdlib.h>
#include <iostream>
//http://www.faqs.org/docs/Linux-HOWTO/NCURSES-Programming-HOWTO.html#HELLOWORLD
int main()
{
    int c;
    initscr();
    noecho();
    while(true){
        c = getch();
        std::cout << c <<std::endl;
        if(c==112){break;} //https://blogs.longwin.com.tw/lifetype/key_codes.html
    }
    //c = getch();
    //std::cout << c <<std::endl;
    //printw("\nYou entered %c.  Good job.\nPress any key to exit.\n", (char)c);
    //(void)getch();
    clear();
    //printw("test");
    //refresh();
    (void)getch();
    endwin();
    return 0;
}
