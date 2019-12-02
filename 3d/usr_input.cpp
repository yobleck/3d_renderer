//g++ -lncurses -o usr_input_exe usr_input.cpp
#include <ncurses.h> //stdio include in curses
//#include <stdlib.h>
#include <iostream>
//http://www.faqs.org/docs/Linux-HOWTO/NCURSES-Programming-HOWTO.html#HELLOWORLD
int main()
{
    int input;
    initscr();
    noecho();
    while(true){
        //clear();
        input = getch(); //w=119 a=97 s=115 d=100 i=105 j=106 k=107 l=108 p=112
        std::cout << input <<std::endl<<"\r";
        if(input==112){break;} //https://blogs.longwin.com.tw/lifetype/key_codes.html
    }
    //c = getch();
    //std::cout << c <<std::endl;
    //printw("\nYou entered %c.  Good job.\nPress any key to exit.\n", (char)c);
    //(void)getch();
    //clear();
    //printw("test");
    //refresh();
    (void)getch();
    endwin();
    return 0;
}
