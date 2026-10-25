#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <cstring>
#include <fstream>
#include <cmath>




int wth = 30*4;
int hht = 30;

int pY = 1;
int pX = 3;

char wall = '#';
char flr = ' ';
char pl = 'V';
char en = 'X';
char hh = '+';

int maxEnemy = 5;
int maxHeal = 3;

int kills = 0;
int lives = 100;
int maxKills = 0;

int attackH = 5;
int inits = 20;

int heal = 15;
int maxLives = 150;

int infoA = 5;

int now = 1;

std::string *nowR = nullptr;

struct cord{
    int x;
    int y;
};

std::string room1[20] = {
        "############E#################",
        "#............................#",
        "#########.............#......#",
        "#.....................#......#",
        "#.....................#......#",
        "#...........#.........#......#",
        "#...........#.........#......#",
        "E...........#.........#......#",
        "#...........#................#",
        "#...........#................#",
        "#...........#................#",
        "#####....################....#",
        "#...................#........#",
        "#...................#........#",
        "#...................#........#",
        "#.......#######.....#........#",
        "#...................#........E",
        "#...................#........#",
        "#...................#........#",
        "##################E###########"
};
cord spawn1u = {12, 1};
cord spawn1d = {18, 18};
cord spawn1l = {1, 7};
cord spawn1r = {28, 16};

std::string room2[20] = {
        "##################E###########",
        "#............................#",
        "#.......#....................#",
        "E.......#....................#",
        "#########............#########",
        "#.......#....................#",
        "#.......#....................#",
        "#.......#....................#",
        "#.......#....................#",
        "#.......#....................#",
        "#.......###############......#",
        "#.............#..............#",
        "#............................#",
        "#............................E",
        "#............................#",
        "#.............#..............#",
        "#.............################",
        "#............................#",
        "#............................#",
        "##############E###############"
};

cord spawn2u = {18, 1};
cord spawn2d = {14, 18};
cord spawn2l = {1, 3};
cord spawn2r = {28, 13};

std::string room3[20] = { 
    "##############E###############", 
    "#............................#", 
    "#....#########....########...#", 
    "#....#.......#....#......#...#", 
    "#....#.......#....#......#...#", 
    "#....#...................#...#", 
    "#....#########....########...#", 
    "#............................#", 
    "######...############...######", 
    "E............................#", 
    "#............................E", 
    "######...############...######", 
    "#............................#", 
    "#....#########....########...#", 
    "#....#...................#...#", 
    "#....#.......#....#......#...#", 
    "#....#.......#....#......#...#", 
    "#....#########....########...#", 
    "#............................#", 
    "##################E###########"
};
cord spawn3u = {14, 1};
cord spawn3d = {18, 18};
cord spawn3l = {1, 9};
cord spawn3r = {28, 10};

std::string room4[20] = { 
    "##########E###################", 
    "#............................#", 
    "#...####........####........##", 
    "#...####........####........##", 
    "#............................#", 
    "E............................#", 
    "#........######........#######", 
    "#........######........#######", 
    "#............................#", 
    "#............................#", 
    "#............................E", 
    "#............................#", 
    "#........######........#######", 
    "#........######........#######", 
    "#............................#", 
    "#............................#", 
    "#...####........####........##", 
    "#...####........####........##", 
    "#............................#", 
    "#######################E######"
};
cord spawn4u = {10, 1};
cord spawn4d = {23, 18};
cord spawn4l = {1, 5};
cord spawn4r = {28, 10};




void ClearScreen(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cursorPosition = { 0, 0 };
    SetConsoleCursorPosition(hConsole, cursorPosition);
}



void setWindowSize(int wth, int hht){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD bufferSize = { (SHORT)wth, (SHORT)hht};
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    SMALL_RECT windowSize = { 0, 0, (SHORT)(wth - 1), (SHORT)(hht-1)};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

}




void drawScreen(std::string room[20]){
    
    ClearScreen();

    for(int i = 0; i < 20; i++){

        for(int j = 0; j < 30; j++){
            if (i == pY && j == pX) {
                std::cout << pl;
            }
            else {
                if(room[i][j] == '#') std::cout << wall;
                if(room[i][j] == '.') std::cout << flr;
                if(room[i][j] == 'X') std::cout << en;
                if(room[i][j] == '+') std::cout << hh;
                if(room[i][j] == 'E') std::cout << 'E';
            }
        }

        if(i == infoA) std::cout << "       " << "kills: " << kills << "        ";
        if(i == infoA+1) std::cout << "       " << "hp: "<< lives << "        ";
        if(i == infoA+2) std::cout << "       " << "max kills: "<< maxKills << "        ";

        std::cout << std::endl;
    }

}




void SpawnEnemy(){
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 30; j++){
            if(nowR[i][j] == 'X'){
                nowR[i][j] = '.';
            }
        }
    }

    for(int e = 0; e < maxEnemy; e++) { 
        int rx = rand() % 28 + 1;
        int ry = rand() % 18 + 1; 
        if(nowR[ry][rx] == '.') {
            nowR[ry][rx] = 'X';
        }
    }

    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 30; j++){
            if(nowR[i][j] == '+'){
                nowR[i][j] = '.';
            }
        }
    }

    for(int e = 0; e < maxHeal; e++) { 
        int rx = rand() % 28 + 1;
        int ry = rand() % 18 + 1; 
        if(nowR[ry][rx] == '.') {
            nowR[ry][rx] = '+';
        }
    }
}

void StatsSave(){
    std::ofstream ostt("stats.txt");
    
    maxKills = std::max(maxKills, kills);
    if(ostt.is_open()){
        ostt << maxKills;
        ostt.close();
    }

}


void chooseRandRoom(){
    
    int res{};
    do{
        res = rand() % 4 + 1;
    }
    while(res == now);

    bool isU = false;
    bool isD = false;
    bool isL = false;
    bool isR = false;

    if(pY == 1) isU = true;
    if(pY == 18) isD = true;
    if(pX == 1) isL = true;
    if(pX == 28) isR = true;


    switch(res){
        case 1:
            nowR = room1;


            if(isL){
                pX = spawn1r.x;
                pY = spawn1r.y;
            }
            else if(isR){
                pX = spawn1l.x;
                pY = spawn1l.y;
            }
            else if(isU){
                pX = spawn1d.x;
                pY = spawn1d.y;
            }
            else if(isD){
                pX = spawn1u.x;
                pY = spawn1u.y;
            }


            now = 1;
            SpawnEnemy();
            break;
        case 2:
            nowR = room2;

            if(isL){
                pX = spawn2r.x;
                pY = spawn2r.y;
            }
            else if(isR){
                pX = spawn2l.x;
                pY = spawn2l.y;
            }
            else if(isU){
                pX = spawn2d.x;
                pY = spawn2d.y;
            }
            else if(isD){
                pX = spawn2u.x;
                pY = spawn2u.y;
            }

            now = 2;
            SpawnEnemy();
            break;

        case 3:
            nowR = room3;

            if(isL){
                pX = spawn3r.x;
                pY = spawn3r.y;
            }
            else if(isR){
                pX = spawn3l.x;
                pY = spawn3l.y;
            }
            else if(isU){
                pX = spawn3d.x;
                pY = spawn3d.y;
            }
            else if(isD){
                pX = spawn3u.x;
                pY = spawn3u.y;
            }

            now = 3;
            SpawnEnemy();
            break;
        
        case 4:
            nowR = room4;

            if(isL){
                pX = spawn4r.x;
                pY = spawn4r.y;
            }
            else if(isR){
                pX = spawn4l.x;
                pY = spawn4l.y;
            }
            else if(isU){
                pX = spawn4d.x;
                pY = spawn4d.y;
            }
            else if(isD){
                pX = spawn4u.x;
                pY = spawn4u.y;
            }

            now = 4;
            SpawnEnemy();
            break;
    }
    drawScreen(nowR);

}

bool checkPos(int x, int y, std::string *room, char caller = 'n'){
    if (room[y][x] != '#'){
        
        if(caller == 'p' && room[y][x] == en){
            room[y][x] = '.';
            kills++;
            StatsSave();
            return true;
        }

        if(caller == 'p' && room[y][x] == '+'){
            room[y][x] = '.';
            lives = std::min(maxLives, lives+heal);
            return true;
        }

        if(caller == 'p' && room[y][x] == 'E'){
            chooseRandRoom();
            return false;
        }

        if(caller == 'n' && pY == y && pX == x){
            lives -= attackH;
            return false;
        }

        if(caller == 'n' && room[y][x] == hh)
            return false;

        return true;
    }
    else
        return false;
}

bool isNearPlayer(int x, int y, cord &poss){
    cord pos[4] = {{x, y+1}, {x-1, y}, {x+1, y}, {x, y-1}};

    for(cord c : pos){
        if(c.x == pX && c.y == pY) {
            poss = c;
            return true;
        }
    }
    return false;
}

void EnemyAI(std::string* room) {
    static int frameCounter = 0;
    frameCounter++;

    if (frameCounter < inits) {
        return;
    }
    frameCounter = 0;

    cord moved[100];
    int movedCount = 0;

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 30; j++) {
            if (room[i][j] == 'X') {
                bool alreadyMoved = false;
                for (int m = 0; m < movedCount; m++) {
                    if (moved[m].x == j && moved[m].y == i) {
                        alreadyMoved = true;
                        break;
                    }
                }

                if (alreadyMoved) continue;
                cord plPos {};
                if(isNearPlayer(j, i, plPos)){
                    lives -= attackH;
                    moved[movedCount].x = j;
                    moved[movedCount].y = i;
                    movedCount++;
                    continue;
                }

                int res = rand() % 4;
                int x = 0, y = 0;

                switch (res) {
                    case 0: x--; break;
                    case 1: x++; break;
                    case 2: y++; break;
                    case 3: y--; break;
                }

                int nextX = j + x;
                int nextY = i + y;

                if (checkPos(nextX, nextY, room)) {
                    if (room[nextY][nextX] == '.') {
                        room[i][j] = '.';
                        room[nextY][nextX] = 'X';

                        moved[movedCount].x = nextX;
                        moved[movedCount].y = nextY;
                        movedCount++;
                    }
                }
            }
        }
    }
    drawScreen(room);
}



void StatsOpen(){
    std::ifstream istt("stats.txt");
    if(istt.is_open()){
        istt >> maxKills;
        istt.close();
    }
}

int main(){
    setlocale(LC_ALL, "ru");

    std::string version = "0.1";

    bool isRun = true;

    SetConsoleTitleA("Cyber grind v0.1");

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    setWindowSize(wth, hht);

    srand(time(NULL));

    StatsOpen();

    nowR = room1;
    SpawnEnemy();

    drawScreen(nowR);
    

    while(isRun){

        if(lives <= 0)return 0;

        

        if(_kbhit()){
            int k = _getch();

            if(k == 27) isRun = false;

            if(k == 'a' || k == 'A')
                if (checkPos(pX-1, pY, nowR, 'p')) pX--;

            if(k == 'd' || k == 'D') 
                if (checkPos(pX+1, pY, nowR, 'p')) pX++;

            if(k == 'w' || k == 'W') 
                if (checkPos(pX, pY-1, nowR, 'p')) pY--;

            if(k == 's' || k == 'S') 
                if (checkPos(pX, pY+1, nowR, 'p')) pY++;

            drawScreen(nowR);
        }

        EnemyAI(nowR);
        Sleep(10);

    }

    return 0;
}

