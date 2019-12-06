//g++ -lncurses -o usr_input_exe usr_input.cpp
#include <ncurses.h> //stdio include in curses
//#include <stdlib.h>
#include <iostream>
#include <string>
#include <wchar.h>
#include <locale.h>
//http://www.faqs.org/docs/Linux-HOWTO/NCURSES-Programming-HOWTO.html#HELLOWORLD
int main(){
    setlocale(LC_ALL,"");
    int input;
    initscr();
    noecho();
    cbreak();
    start_color();
        init_pair(1,COLOR_RED,COLOR_BLACK);
    refresh();
    WINDOW *win = newwin(0,0,0,0);
    //nodelay(win,TRUE);
    while(true){
        //werase(win);
        input = wgetch(win); //w=119 a=97 s=115 d=100 i=105 j=106 k=107 l=108 p=112
        //std::cout << input <<std::endl<<"\r";
        wprintw(win, "%i", input); wprintw(win, "%s", "\n\r");   //https://alvinalexander.com/programming/printf-format-cheat-sheet
        wattron(win, COLOR_PAIR(1));
        wprintw(win, "%s", "■\n");
        wattroff(win, COLOR_PAIR(1));
        wrefresh(win);
        if(input==112){break;} //https://blogs.longwin.com.tw/lifetype/key_codes.html
    }
    //clear();
    //printw("test");
    //refresh();
    (void)wgetch(win);
    endwin();
    return 0;
}
