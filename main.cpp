#include <iostream>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <cstdio>
#include <ctime>   
#include <cmath>

#include<thread>
#include<chrono>

using namespace std;

const int width = 65;
const int height = 25; //строки
char map[height][width];
const float M_PI = 3.1415;

int countD{};
int countU{};
int lvl{};

// для отмены мерцания карты
void gotoxy(int x, int y) {
    COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// невидимый курсор
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;        // Размер курсора (1-100%)
    cursorInfo.bVisible = FALSE;  // Скрыть курсор
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

struct racket {
    int x, y;
    int w;
};

struct ball {
    float x, y;
    int ix, iy;
    float alpha;
    float speed;
};
racket racket1;
racket racket2;
ball ball1;

static bool random_wall_placed = false;
static int wall_x = -1;
static int wall_y = -1;



void moveBall(float x, float y);
void initRacket();

void initBall() {
    moveBall(racket1.x + racket1.w / 2.0f - 1.0f, racket1.y - 1.0f);

    ball1.alpha = -M_PI / 4.5f; // 45° вверх-вправо НА СТАРТЕ Угол -45 градусов (вверх-вправо) в радианах
   ball1.speed = 1.2f;         // скорость мячика
//    ball1.speed = 0.9f; //для тестов
}
void initBallSecond() {
    moveBall(racket2.x + racket2.w / 2.0f - 1.0f, racket2.y + 1.0f);

    ball1.alpha = -M_PI / 4.5f; // 45° вверх-вправо НА СТАРТЕ Угол -45 градусов (вверх-вправо) в радианах
    ball1.speed = 1.3f;         // скорость мячика
//    ball1.speed = 0.9f; //для тестов
}

void showBall(){
    if (ball1.iy >= 0 && ball1.iy < height &&
        ball1.ix >= 0 && ball1.ix < width) {
        map[ball1.iy][ball1.ix] = 'O';
    }
}

void moveBall(float x, float y) {
    ball1.x = x;
    ball1.y = y;
    ball1.ix = (int)round(ball1.x);
    ball1.iy = (int)round(ball1.y);
    
}

void autoMoveBall()
{
    if (ball1.alpha < 0) ball1.alpha += M_PI * 2;
    if (ball1.alpha > M_PI * 2) ball1.alpha -= M_PI * 2;
    ball bl = ball1;
    moveBall(ball1.x + cos(ball1.alpha) * ball1.speed,
        ball1.y + sin(ball1.alpha) * ball1.speed);

    if ((map[ball1.iy][ball1.ix] == '#') || (map[ball1.iy][ball1.ix] == '@'))
    {
        if ((ball1.ix != bl.ix) && (ball1.iy != bl.iy))
            {
            if (map[bl.iy][ball1.ix] == map[ball1.iy][bl.ix])
                bl.alpha = bl.alpha + M_PI;
            else
            {
                if (map[bl.iy][ball1.ix] == '#')
                    bl.alpha = (2 * M_PI - bl.alpha) + M_PI;
                else
                    bl.alpha = (2 * M_PI - bl.alpha);
            }
            }
            
        else if (ball1.iy == bl.iy) 
            bl.alpha = (2 * M_PI - bl.alpha) + M_PI;
        else bl.alpha = (2 * M_PI - bl.alpha);
        ball1 = bl;
        autoMoveBall();
    }
}



void initRacket() {
    racket1.w = 7;
    racket1.x = (width - racket1.w) / 2;
    racket1.y = height - 2;
}

void showRacket() {
    for (int i = racket1.x; i < racket1.x + racket1.w; i++) {
        map[racket1.y][i] = '@';
    }
}

void moveRacket(int x) {
    racket1.x = x;

    if (racket1.x < 1) racket1.x = 1;
    if (racket1.x + racket1.w >= width - 1) racket1.x = width - racket1.w - 1;
}

//______________2 РАКЕТКА______________//

void initRacketSecond() {
    racket2.w = 7;
    racket2.x = (width - racket2.w) / 2;
    racket2.y = 0+1;
}

void showRacketSecond() {
    for (int i = racket2.x; i < racket2.x + racket2.w; i++) {
        map[racket2.y][i] = '@';
    }
}

void moveRacketSecond(int x) {
    racket2.x = x;

    if (racket2.x < 1) racket2.x = 1;
    if (racket2.x + racket2.w >= width - 1) racket2.x = width - racket2.w - 1;
}



void initMap (int lvl) {
    memset(map, ' ', sizeof(map));
    for (int j = 0; j < width; j++) {
        map[0][j] = ' ';
        map[height - 1][j] = ' ';
    }
    for (int i = 0; i < height; i++) {
        map[i][0] = map[i][width - 1] = '#';
    }

    // 2 УРОВЕНЬ
    if (lvl == 2) {
        for (int j = width / 2 - 10; j < width / 2 + 10; j++) {  //стена посередине
            map[height / 2][j] = '#';
        }
    }
    // 3 УРОВЕНЬ
    if (lvl == 3) {
        for (int j = 7; j < 15; j++) {  //маленькая стена слева
            map[height / 4][j] = '#';
            map[18][j] = '#';
        }
        for (int i = 50; i < 58; i++) {  //маленькая стена слева
            map[height / 4][i] = '#';
            map[18][i] = '#';
        }
    }
    // 4 уровень
    if (lvl == 4) {
        for (int j = 0; j < 18; j++) {
            map[0][j] = '#';
            map[height - 1][j] = '#';
        }
        for (int j = 47; j < width; j++) {
            map[0][j] = '#';
            map[height - 1][j] = '#';
        }
    }
 }
       
void showMap() {
    static char buffer[height * (width + 1) + 1]; // Буфер для всей карты
    int pos = 0;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            buffer[pos++] = map[i][j];
        }
        buffer[pos++] = '\n';
    }
    buffer[pos] = '\0';
    // Один системный вызов вместо 1625!
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), buffer, pos, NULL, NULL);
}

void ShowLVL() {
    if (lvl == 1 || lvl == 2 || lvl == 3) {
        system("cls");
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n \t\t\t\t\t\t LEVEL " << lvl;
        Sleep(1500);
        system("cls");
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n \t\t\t\t\t\t FIGHT!!!";
        Sleep(1000);
    }
    else if (lvl == 4) {
        system("cls");
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n \t\t\t\t\t\t LEVEL " << lvl;
        Sleep(1500);
        system("cls");
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n \t\t\t\t\t\t FINAL LVL!!!";
        Sleep(1000);
    }
}

int main(void) {
    setlocale(LC_ALL, "RU");

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    hideCursor();
    system("cls");

    initRacket();
    initRacketSecond();
    initBall();
    showBall();
    showMap();
    bool run = false;
    bool game = true;
    bool ballAtBottom = false;
    bool uplvl = true;
    bool uplvlTo3 = true;
    bool uplvlTo4 = true;

    lvl = 1;
    ShowLVL();

    do {
        initMap(lvl);
        showRacket();
        showRacketSecond();

        if (GetAsyncKeyState('E') & 0x8001) break;
        if (GetAsyncKeyState('A') & 0x8000) moveRacket(racket1.x - 3); 
        if (GetAsyncKeyState('D') & 0x8000) moveRacket(racket1.x + 3);
        if (GetAsyncKeyState('J') & 0x8000) moveRacketSecond(racket2.x - 3);
        if (GetAsyncKeyState('L') & 0x8000) moveRacketSecond(racket2.x + 3);

        // Позиционирование шарика
        if (!run) {
            if (!ballAtBottom)
            {
                moveBall(racket1.x + racket1.w / 2.0f - 1.0f, racket1.y - 1.0f);
                if (GetAsyncKeyState('W') & 0x8000) run = true;
            }
            else if (ballAtBottom)
            {
                moveBall(racket2.x + racket2.w / 2.0f - 1.0f, racket2.y + 1.0f);
                if (GetAsyncKeyState('I') & 0x8000) run = true;
            }
        }
        else {
            // Движение по физике
            autoMoveBall();
            if (ball1.iy <= 0) {
                countD++;
                run = false;
                initBall();
                ballAtBottom = false;
            }
            else if (ball1.iy >= height) {
                countU++;
                run = false;
                initBallSecond();
                ballAtBottom = true;
            }
            showBall();
        }
        gotoxy(width + 2, 1);
        std::cout << "Scores Player 1 (up): " << countU;
        gotoxy(width + 2, 23);
        cout << "Scores Player 2 (down): " << countD;

        if (uplvl && (countD == 5 || countU == 5)) {
            this_thread::sleep_for(chrono::milliseconds(250));
            lvl++;
            ShowLVL();
            uplvl = false;
        }
        if (uplvlTo3 && (countD == 10 || countU == 10)) {
            this_thread::sleep_for(chrono::milliseconds(250));
            lvl++;
            ShowLVL();
            uplvlTo3 = false;
        }
        if (uplvlTo4 && (countD == 15 || countU == 15)) {
            this_thread::sleep_for(chrono::milliseconds(250));
            lvl++;
            ShowLVL();
            uplvlTo4 = false;
        }


        if (countD >= 16) {
            game = false;
            gotoxy(width + 2, height / 2 - 1);
            cout << "PLAYER 2 IS WINNER ! ! !";
            gotoxy(width + 2, height / 2); 
            cout << "count: " << countD;
            gotoxy(width + 2, height / 2 + 1);
            cout << "Нажмите Q чтобы выйти";
            char key = _getch();
            while (true) {
                char key = _getch();
                if (key == 'q' || key == 'Q') {
                    gotoxy(1, height);
                    return 0;
                }
            }
        }
        else if (countU >= 16) {
            game = false;
            gotoxy(width + 2, height /2 );
            cout << "PLAYER 1 IS WINNER ! ! !";
            gotoxy(width + 2, height / 2 + 1);
            cout << "count: " << countU;
            while (true) {
                gotoxy(width + 2, 25);
                char key = _getch();
                if (key == 'q' || key == 'Q') {
                    gotoxy(1, height);
                    return 0; 
                }
            }
        }

        showBall();
        gotoxy(0, 0);
        showMap();

        Sleep(20);
    } while (game);
    return 0;
}
