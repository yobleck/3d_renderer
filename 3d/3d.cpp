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
    setlocale(LC_ALL,""); //for proper wprintw of square character
    int player_pos[3] = {50, 50, 50}; //={500,500,500} //store player position in 3d space
    const double pi = 3.14159265358979323846; //defines pi duh
    double cam_angle[2] = {pi/2, 0}; //={0,0} //store the vertical and horizontal camera angle
    const double res[2] = {72, 40}; //={1920,1080} //horizontal then vertical resolution
    const int render_dist = 65;
    const double fov[2] = {pi/2, (5*pi)/12}; //={90,75} //horizontal then vertical field of view
    
    //TODO:replace this with read from vmf file and k-d tree and stuff
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
            //cout << framebuffer[x][y];
        }//cout << endl;
    }
    //////////////////////////////////////////end of initialization
    
    
    int input;
    int temp_loop =0;
    initscr(); //start curses
    noecho();
    //curs_set(0) //makes cursor invisible
    refresh();
    WINDOW *win = newwin(0,0,0,0); //creates window    TODO:might want to uses this to set to res
    nodelay(win,TRUE); //makes wgetch non blocking
    
    while(temp_loop<20){ //MAIN LOOP
        temp_loop++;
        input = wgetch(win); //w=119 a=97 s=115 d=100 i=105 j=106 k=107 l=108 p=112
        //TODO:insert code to handle user inputs here
        if(input==112){break;} //check for user input to break out of loop and end program cleanly
        else if(input==119){player_pos[0]++;}
        else if(input==97){}
        else if(input==115){player_pos[0]--;}
        else if(input==100){}
        else if(input==105){}
        else if(input==106){}
        else if(input==107){}
        else if(input==108){}
        
        player_pos[1]++; //for testing
    
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
            }//cout << endl;
            frame_hor++;
        }
        frame_vert++; frame_hor=0;
    }
    
        //clear previous frame before drawing new one
        //wclear(win);
        //werase(win);
        //wmove(win,0,0);
        //wrefresh(win);    //why doesn't this work TODO:draw entire frame to buffer then refresh to draw all at once
    
    //print frame buffer to cmd with "\u25A0" black geometric square character
    for(int x=0;x<res[1];x++){
        for(int y=0;y<res[0];y++){
            if(framebuffer[x][y]==0){
               //cout << "\033[30m■\033[0m"; //unicode square character as pixel with ascii color encoding   see above for color coding
               wprintw(win, "%s", "■");
            }
            else if(framebuffer[x][y]==1){
                //cout << "\033[31m■\033[0m";
                wprintw(win, "%s", "■");
            }
            else if(framebuffer[x][y]==2){
                //cout << "\033[34m■\033[0m";
                wprintw(win, "%s", "■");
            }
            else if(framebuffer[x][y]==3){
                //cout << "\033[32m■\033[0m";
                wprintw(win, "%s", "■");
            }
            else if(framebuffer[x][y]==4){
                //cout << "\033[33m■\033[0m";
                wprintw(win, "%s", "■");
            }
            else if(framebuffer[x][y]==5){
                //cout << "\033[36m■\033[0m";
                wprintw(win, "%s", "■");
            }
            else if(framebuffer[x][y]==6){
                //cout << "\033[37m■\033[0m";
                wprintw(win, "%s", "■");
            }
            else if(framebuffer[x][y]==7){
                //cout << "\033[1;31m■\033[0m";
                wprintw(win, "%s", "■");
            }
            else if(framebuffer[x][y]==8){
                //cout << "\033[1;35m■\033[0m";
                wprintw(win, "%s", "■");
            }
            else if(framebuffer[x][y]==9){
               //cout << "\033[35m■\033[0m";
               wprintw(win, "%s", "■");
            }
            //cout << framebuffer[x][y];
        }/*cout << endl<<"\r";*/ wprintw(win, "%s", "\n");
    }
    
        
    auto stop = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = stop - start;
    //cout << "time: " << elapsed.count() <<endl<<"\r";
    wprintw(win, "%s", "time: "); wprintw(win, "%f", elapsed.count()); wprintw(win, "%s", "\n");
    wrefresh(win);
    wmove(win,0,0);
    

        usleep(500000); //pauses loop so output can be seen on screen   TODO:refine to cause less flickering    something to do with erase vs clear maybe
    }
    endwin();
    
    
    //cout << "\ntest" << endl;
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
