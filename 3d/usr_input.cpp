//g++ -lncurses -lpthread -pthread -o usr_input_exe usr_input.cpp
#include <ncurses.h> //stdio include in curses
//#include <stdlib.h>
#include <iostream>
#include <string>
#include <wchar.h>
#include <locale.h>
#include<vector>
#include <pthread.h>
#include <thread>
using namespace std;
//http://www.faqs.org/docs/Linux-HOWTO/NCURSES-Programming-HOWTO.html#HELLOWORLD
void func(int input1, WINDOW *wini);

int main(){
    setlocale(LC_ALL,"");
    int input;
    int thread_count = 8;
    //pthread_t threads[thread_count];
    vector<thread> threads;
    
    initscr();
    noecho();
    cbreak();
    start_color();
        init_pair(1,COLOR_RED,COLOR_BLACK);
    refresh();
    WINDOW *win = newwin(0,0,0,0);
    //nodelay(win,TRUE);
    while(true){
        input = wgetch(win); //w=119 a=97 s=115 d=100 i=105 j=106 k=107 l=108 p=112
        
        wprintw(win, "%i", input); wprintw(win, "%s", "\n\r");   //https://alvinalexander.com/programming/printf-format-cheat-sheet
        wattron(win, COLOR_PAIR(1));
        wprintw(win, "%s", "■\n");
        wattroff(win, COLOR_PAIR(1));
        wrefresh(win);
        
        for(int i=0;i<thread_count;i++){
            threads.push_back(thread(func,i,win)); //https://stackoverflow.com/questions/19675134/how-to-spawn-multiple-threads-that-call-same-function-using-stdthread-c
        }
        for (auto& t : threads){
            if(t.joinable()==true){
               t.join(); 
            }
        }
        
        
        if(input==112){break;} //https://blogs.longwin.com.tw/lifetype/key_codes.html
    }
    //clear();
    //printw("test");
    //refresh();
    (void)wgetch(win);
    endwin();
    return 0;
}


void func(int input1, WINDOW *wini){
    wprintw(wini,"%i",input1); wprintw(wini,"%s","\n");
}
