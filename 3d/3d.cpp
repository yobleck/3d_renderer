//g++ -lncurses -o 3dexe 3d.cpp     -lncurses see usr_input.cpp
#include <iostream>
//#include <fstream> //file reading
#include <cstdlib>
#include <stdlib.h> //changing console text color
#include <ctime>
#include <chrono> //for measuring code speed
#include <cmath> //sin cos etc.
#include <ncurses.h> //for screen rendering stuff
#include <unistd.h> //for sleeping
#include <locale.h> //for proper printw of suare character
using namespace std;

int main(){//auto start = chrono::high_resolution_clock::now();
    //declare and initialize variables
    setlocale(LC_ALL,""); //for proper wprintw of square unicode character
    int player_pos[3] = {50, 50, 50}; //={500,500,500} //store player position in 3d space
    const double pi = 3.14159265358979323846; //defines pi duh
    double cam_angle[2] = {pi/2, 0}; //={0,0} //store the vertical and horizontal camera angle
    const double res[2] = {96, 55}; //={1920,1080} //horizontal then vertical resolution    //BUG:perfect 16:9 causes problem
    const int render_dist = 50;
    const double fov[2] = {pi/2, (5*pi)/12}; //={90,75} //horizontal then vertical field of view
    
    //TODO:replace this with read from vmf file and k-d tree or octree and stuff
    int map[100][100][100]; //map data with each x,y,z point holding color info
    //populates cubic map with walls of different colors/numbers
    for(int x=0;x<100;x++){
        for(int y=0;y<100;y++){
            for(int z=0;z<100;z++){
                if(x==0){
                    map[x][y][z]=1; //=red https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
                }
                else if(x==99){
                    map[x][y][z]=2; //=blue
                }
                else if(y==0){
                    map[x][y][z]=3; //=green
                }
                else if(y==99){
                    map[x][y][z]=4; //=yellow
                }
                else if(z==0){
                    map[x][y][z]=5; //=cyan
                }
                else if(z==99){
                    map[x][y][z]=6; //=white
                }
                else{
                    map[x][y][z]=0; //=black //9 for error values =magenta
                }//cout << map[x][y][z];
            }//cout << endl;
        }//cout << "new slice" << endl;
    }//cout << map[0][0][0] << endl;
    map[50][50][60]=1,map[55][50][50]=4,map[50][55][50]=3,map[50][50][45]=2,map[45][50][50]=5,map[50][45][50]=6; //floating color points for testing
    
    //declares and initializes frame buffer of what player sees
    int framebuffer[(int)res[1]][(int)res[0]];
    for(int x=0;x<res[1];x++){
        for(int y=0;y<res[0];y++){
            framebuffer[x][y]=8; //=bold magenta
        }
    }
    //////////////////////////////////////////end of initialization
    
    
    int input;
    int temp_loop=0;
    initscr(); //start ncurses
    noecho();
    start_color();//curses color initialization
        init_pair(0,COLOR_BLACK,COLOR_BLACK);
        init_pair(1,COLOR_RED,COLOR_BLACK);
        init_pair(2,COLOR_BLUE,COLOR_BLACK);
        init_pair(3,COLOR_GREEN,COLOR_BLACK);
        init_pair(4,COLOR_YELLOW,COLOR_BLACK);
        init_pair(5,COLOR_CYAN,COLOR_BLACK);
        init_pair(6,COLOR_WHITE,COLOR_BLACK);
        init_pair(7,COLOR_RED,COLOR_YELLOW);
        init_pair(8,COLOR_RED,COLOR_GREEN);
        init_pair(9,COLOR_MAGENTA,COLOR_BLACK);
    curs_set(0); //makes cursor invisible
    refresh();//very necessary. world will end without
    WINDOW *win = newwin(0,0,0,0); //creates window    TODO:might want to set this to res
    nodelay(win,TRUE); //makes wgetch non blocking
    
    while(temp_loop<200){ //MAIN LOOP
        //temp_loop++;
        input = wgetch(win); //w=119 a=97 s=115 d=100 q=113 e=101 i=105 j=106 k=107 l=108 p=112
        //TODO:insert code to handle user inputs here
        //all movement directions are absolute and independent of rotation
        if(input==112){break;} //break out of loop and end program cleanly
        else if(input==119){if(player_pos[0]<98){player_pos[0]++;}} //forward
        else if(input==97){if(player_pos[2]<98){player_pos[2]++;}}
        else if(input==115){if(player_pos[0]>0){player_pos[0]--;}} //backwards    //BUG: remove >0 limit to see some wierd shit
        else if(input==100){if(player_pos[2]>0){player_pos[2]--;}}
        else if(input==113){if(player_pos[1]>0){player_pos[1]--;}} //up
        else if(input==101){if(player_pos[1]<98){player_pos[1]++;}} //down
        else if(input==105){cam_angle[1]-=pi/180;} //look up  ///BUG: cant look all the way up or down
        else if(input==106){cam_angle[0]-=pi/180;}
        else if(input==107){cam_angle[1]+=pi/180;} //look down
        else if(input==108){cam_angle[0]+=pi/180;}
        
    
    //actual rendering calculations done here
    auto start = chrono::high_resolution_clock::now();
    double temp_x=0, temp_y=0, temp_z=0;
    int frame_hor=0, frame_vert=0;
    //double for loop runs through screen resolution pixel by pixel
    for(double theta = cam_angle[0]-(fov[0]/2); theta <= cam_angle[0]+(fov[0]/2); theta += fov[0]/res[0]){ //horizontal
        for(double phi = cam_angle[1]-(fov[1]/2); phi <= cam_angle[1]+(fov[1]/2); phi+= fov[1]/res[1]){ //vertical
            //cout << theta << " " << phi << endl;
            for(int radius = 1; radius <= render_dist; radius++){ //ray view distance
                //this code calculates new coordinate along ray path and gets closest map value
               temp_x = ((double)radius * sin(theta) * cos(phi))+(double)player_pos[0];
               temp_y = ((double)radius * sin(theta) * sin(phi))+(double)player_pos[1];
               temp_z = ((double)radius * cos(theta))+(double)player_pos[2];
               //cout << temp_x << " " << temp_y << " " << temp_z << endl;
               
               //this code checks to see if ray intersects with non zero value
               if(map[(int)round(temp_x)][(int)round(temp_y)][(int)round(temp_z)]!=0){
                    try{ //try catch to avoid index out of range errors with the map array
                        framebuffer[frame_hor][frame_vert] = map[(int)round(temp_x)][(int)round(temp_y)][(int)round(temp_z)];
                    }catch(...){
                        framebuffer[frame_hor][frame_vert] = 9;
                    }
                    break; //get out of loop if collision detected and color value retrieved
               }
               else if(radius==render_dist){ //maximum view distance exception
                    try{
                        framebuffer[frame_hor][frame_vert] = map[(int)round(temp_x)][(int)round(temp_y)][(int)round(temp_z)];
                    }
                    catch(...){
                        framebuffer[frame_hor][frame_vert] = 9;
                    }
               }
            }
            frame_hor++;
        }
        frame_vert++; frame_hor=0;
    }
    

    
    //print frame buffer to cmd with "\u25A0" black geometric square character
    for(int x=0;x<res[1];x++){
        for(int y=0;y<res[0];y++){
            
            ///* //unicode square character as pixel with ascii color encoding   see above for color coding
            wattron(win,COLOR_PAIR(framebuffer[x][y]));  //BUG: colors are inverted?
            wprintw(win, "%s", "■");
            wattroff(win, COLOR_PAIR(framebuffer[x][y]));
             //*/
            /*
            if(framebuffer[x][y]==0){
                wattron(win,COLOR_PAIR(0));
                wprintw(win, "%s", "■");
                wattroff(win, COLOR_PAIR(0));
            }
            else if(framebuffer[x][y]==1){
                wattron(win,COLOR_PAIR(1));
                wprintw(win, "%s", "■");
                wattroff(win, COLOR_PAIR(1));
            }
            else if(framebuffer[x][y]==2){
                wattron(win,COLOR_PAIR(2));
                wprintw(win, "%s", "■");
                wattroff(win, COLOR_PAIR(2));
            }
            else if(framebuffer[x][y]==3){
                wattron(win,COLOR_PAIR(3));
                wprintw(win, "%s", "■");
                wattroff(win, COLOR_PAIR(3));
            }
            else if(framebuffer[x][y]==4){
                wattron(win,COLOR_PAIR(4));
                wprintw(win, "%s", "■");
                wattroff(win, COLOR_PAIR(4));
            }
            else if(framebuffer[x][y]==5){
                wattron(win,COLOR_PAIR(5));
                wprintw(win, "%s", "■");
                wattroff(win, COLOR_PAIR(5));
            }
            else if(framebuffer[x][y]==6){
                wattron(win,COLOR_PAIR(6));
                wprintw(win, "%s", "■");
                wattroff(win, COLOR_PAIR(6));
            }
            else if(framebuffer[x][y]==7){
                wattron(win,COLOR_PAIR(7));
                wprintw(win, "%s", "■");
                wattroff(win, COLOR_PAIR(7));
            }
            else if(framebuffer[x][y]==8){
                wattron(win,COLOR_PAIR(8));
                wprintw(win, "%s", "■");
                wattroff(win, COLOR_PAIR(8));
            }
            else if(framebuffer[x][y]==9){
               wattron(win,COLOR_PAIR(9));
                wprintw(win, "%s", "■");
                wattroff(win, COLOR_PAIR(9));
            }*/
        }wprintw(win, "%s", "\n");
    }
    
        
    auto stop = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = stop - start;
    wprintw(win, "%s", "time: "); wprintw(win, "%f", elapsed.count()); wprintw(win, "%s", "\n");
    wrefresh(win);
    wmove(win,0,0);
    

        //usleep(1000); //pauses loop so output is slower
    }
    endwin();
    
    return 0;
}



//optional TODO:ray splitting for distant objects average rgb values of the 2 sub rays

/*worry about this later. easier to just generate map in c++ for now
//read map file into var
ifstream input_file;
input_file.open("map_100.wtf");

input_file.seekg(0,input_file.end);
int file_length = input_file.tellg();
input_file.seekg(0, input_file.beg);

char * buffer = new char[file_length];
input_file.read(buffer,file_length);

cout << buffer << endl;
cout << file_length << endl;
cout << buffer[0];
input_file.close();
delete[] buffer; //temporary?
*/
