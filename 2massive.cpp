#include "TXLib.h"

enum { WALL = 'X', SPACE = '-', WIN = '!', KNIFE = 4 };

const int SIZEB = 30;
const int MASSSIZE = 24;
const int MASSLAYER = 2;


#define XM x / SIZEB
#define YM y / SIZEB


void block (int x, int y, COLORREF color = TX_WHITE);

void ReadKarta (char map [MASSLAYER][MASSSIZE][MASSSIZE], int lvl);
void DrawKarta (int x, int y, int dx, int dy, char map[MASSLAYER][MASSSIZE][MASSSIZE], int layer);

void MovePelByXY (int* x, int* y, int* vx, int* vy, char map [MASSLAYER][MASSSIZE][MASSSIZE], int layer);

int MovingOn(int lev, char allMap [MASSLAYER][MASSSIZE][MASSSIZE], int layer);
void warrior(int x, int y, int r);

void pelmen(int x, int y, int r, double eyeR, double eyeL, double morgR, double morgL);

const COLORREF MEOW_COLOR = TX_BLUE;

int main()
    {

    txCreateWindow (MASSSIZE * SIZEB, MASSSIZE * SIZEB);
    char map [MASSLAYER][MASSSIZE][MASSSIZE] = {};
    int lay = 0;
    ReadKarta (map, 1);
    if (MovingOn (1, map, lay) == 1)
        {

        MovingOn (1, map, lay + 1);
        }

    }

void block (int x, int y, COLORREF color)
    {
    txSetColor (color);
    txSetFillColor (color);
    txRectangle (x, y, x+1*SIZEB, y+1*SIZEB);
    }

void ReadKarta (char map [MASSLAYER][MASSSIZE][MASSSIZE], int lvl)
    {


    char fname [50] = "";
    sprintf (fname, "mapLvl%d.txt", lvl);

    FILE* f = fopen (fname, "r");
    if (!f) { printf ("%s: error: cannot open level file\n", fname); return; }

    int sizeX = 0, sizeY = 0, verMajor = 0, verMinor = 0;


    fscanf (f, "MEOW %d.%d %d %d", &verMajor, &verMinor, &sizeX, &sizeY);
    if (verMajor != 1 || verMinor != 0 || sizeX != MASSSIZE || sizeY != MASSSIZE)
        {
        printf ("%s: error: cannot open meowing header in the level file, "
                "format not recognized\n", fname);
        fclose (f);
        return;
        }
    for (int layer = 0; layer < MASSLAYER; layer ++)
        {
        for (int y = 0; y < MASSSIZE; y++ )
            {
                for (int x = 0; x < MASSSIZE; x++ )
                {
                assert (0 <= x && x < MASSSIZE);
                assert (0 <= y && y < MASSSIZE);


                fscanf (f, " %c", &map [layer][y][x]); // fgetc



                if (map[layer][y][x] != WALL && map[layer][y][x] != SPACE && map[layer][y][x] != WIN)
                printf ("%s:%d:%d: warning: unexpected symbol '%c' (%d)\n", fname, y, x, map[layer][y][x], map[layer][y][x]);
                }

        printf ("\n");
        }
        }
    fclose (f);
    }


void DrawKarta (int x0, int y0, int dx, int dy, char map [MASSLAYER][MASSSIZE][MASSSIZE], int layer)
    {

    for (int y = 0; y < MASSSIZE; y++ )
        for (int x = 0; x < MASSSIZE; x++ )
            {
            if (map [layer][y][x] == SPACE) block (x0 + x * dx, y0 + y * dy, TX_BLACK);
            if (map [layer][y][x] == WALL)  block (x0 + x * dx, y0 + y * dy, TX_WHITE);
            if (map [layer][y][x] == WIN)   block (x0 + x * dx, y0 + y * dy, TX_LIGHTBLUE);

            }
    }


void warrior(int x, int y, int r)
    {
    txSetColor (TX_BLACK);
    txSetFillColor (TX_RED);
    txRectangle (x, y, x+15, y+30);
    txCircle (x+7.5, y, r);
    txRectangle (x-5, y+30, x+5, y+40);
    txRectangle (x+10, y+30, x+20, y+40);
    txSetFillColor (TX_WHITE);
    txSetColor (TX_BLACK);
    txCircle (x+7.5, y, r/3);
    txSetFillColor (TX_BLACK);
    txCircle (x+7.5, y, r/6);
    }

void pelmen(int x, int y, int r, double eyeR, double eyeL, double morgR, double morgL)
    {
    txSetColor (TX_BLACK);
    txSetFillColor (TX_LIGHTGRAY);
    txCircle  (x, y, r);

    txSetFillColor (TX_RED);
    txCircle  (x-7, y-7, eyeL);
    txCircle  (x+7, y-7, eyeR);

    txSetFillColor (TX_LIGHTGRAY);
    txSetColor  (TX_LIGHTGRAY);
    txRectangle  (x-7-eyeL, y-morgL, x-7+eyeL, y+7);
    txRectangle  (x+7-eyeR, y-morgR, x+7+eyeR, y+7);



    }


void MoveByAngle (int* vx, int* vy)

    {
    int alpha = atan(*vy / *vx);
    int v = hypot(*vx, *vy);

    if (GetAsyncKeyState (VK_RIGHT)) alpha += M_PI/12;
    if (GetAsyncKeyState (VK_LEFT))  alpha -= M_PI/12;
    if (GetAsyncKeyState (VK_UP))    v += 2;
    if (GetAsyncKeyState (VK_DOWN))  v -= 2;

    *vx = v * cos(alpha);
    *vy = v * sin(alpha);

    }

void MovePelByXY (int* x, int* y, int* vx, int* vy, char map [MASSLAYER][MASSSIZE][MASSSIZE], int layer)

    {
     *x = *vx * 1 + *x;
     *y = *vy * 1 + *y;

     *vy = *vy * 0.8;
     *vx = *vx * 0.8;

     // printf ("x = %d, x/SIZEB = %d + %lg\n", *x, *x/SIZEB, *x/SIZEB - round (*x/SIZEB));

     if (GetAsyncKeyState (VK_RIGHT) && map [layer][ROUND (*y/SIZEB)]     [ROUND (*x/SIZEB + 1)] != WALL) *vx += 2;
     if (GetAsyncKeyState (VK_LEFT)  && map [layer][ROUND (*y/SIZEB)]     [ROUND (*x/SIZEB - 1)] != WALL) *vx -= 2;
     if (GetAsyncKeyState (VK_DOWN)  && map [layer][ROUND (*y/SIZEB + 1)] [ROUND (*x/SIZEB)]     != WALL) *vy += 2;
     if (GetAsyncKeyState (VK_UP)    && map [layer][ROUND (*y/SIZEB - 1)] [ROUND (*x/SIZEB)]     != WALL) *vy -= 2;

    }


int MovingOn(int lev, char allMap [MASSLAYER][MASSSIZE][MASSSIZE], int layer)
    {

    int x = 310;
    int y = 370;
    int vx = 4;
    int vy = 4;
    int t = 0;


    txBegin ();



    while (true)

        {
         txSetFillColor (MEOW_COLOR);
         txClear();


         //SaveX = x / SIZEB * SIZEB + SIZEB / 2;
         //SaveY = y / SIZEB * SIZEB + SIZEB / 2;



         DrawKarta (0, 0, 1 * SIZEB, 1 * SIZEB, allMap, layer);


         MovePelByXY (&x, &y, &vx, &vy, allMap, layer);

         pelmen (x, y, 15, 6, 6, t % 15, t % 15);


         if (y >= SIZEB * MASSSIZE) return 1;



         t++;
         txSleep (30);
        }



    txEnd;
    }
