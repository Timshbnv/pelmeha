#include "TXLib.h"
#include "Personages.h"

enum { WALL = 'X', SPACE = '-', WIN = '!', KNIFE = 4 };

const int SIZEB = 30;
const int MASSSIZE = 24;
const int MASSLAYER = 3;
const int MASSLEVEL = 2;

#define XM x / SIZEB
#define YM y / SIZEB

/*-----------------------------------------------PROTOTYPES----------------------------------------------------------*/
void block (int x, int y, COLORREF color = TX_WHITE);
void ReadKarta (char map [MASSLAYER][MASSSIZE][MASSSIZE], int lvl);
void DrawKarta (int x, int y, int dx, int dy, char map[MASSLAYER][MASSSIZE][MASSSIZE], int layer);
void MovePelByXY (int* x, int* y, int* vx, int* vy, char map [MASSLAYER][MASSSIZE][MASSSIZE], int layer);
int MovingOn(char allMap [MASSLAYER][MASSSIZE][MASSSIZE], int layer);

//choosing color for portals here:
const COLORREF MEOW_COLOR = TX_BLUE;
//every portal will be painted in this color




int main()
    {

    txCreateWindow (MASSSIZE * SIZEB, MASSSIZE * SIZEB);
    char map [MASSLAYER][MASSSIZE][MASSSIZE] = {};


    //for (
           int lvl = 1; //lvl <= MASSLEVEL; lvl ++)

            ReadKarta (map, lvl);
            int layer = 0;
            txBegin();
            while (true)
             {
                int whrgo = MovingOn (map, layer);

                if (whrgo == 0) break;

                switch (whrgo)
                {
                case 1: layer = 0; break;
                case 2: layer = 1; break;
                case 3: layer = 2; break;
                default: layer = 0;
                }
             }
            txEnd();

    }

int MovingOn(char allMap [MASSLAYER][MASSSIZE][MASSSIZE], int layer)
    {

    int x = 360;
    int y = 120;
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

         if (allMap [layer][YM][XM] == WIN) return 0;
         if (allMap [layer][YM][XM] == 'A') return 1;
         if (allMap [layer][YM][XM] == 'B') return 2;
         if (allMap [layer][YM][XM] == 'C') return 3;






         t++;
         txSleep (30);


        }



    txEnd;
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
        for (int layer = 0; layer < 4; layer ++)
        {
            for (int y = 0; y < MASSSIZE; y++ )
            {
                for (int x = 0; x < MASSSIZE; x++ )
                {
                assert (0 <= x && x < MASSSIZE);
                assert (0 <= y && y < MASSSIZE);


                fscanf (f, " %c", &map [layer][y][x]); // fgetc




                }

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
            /*if (map [layer][y][x] == 'A')   block (x0 + x * dx, y0 + y * dy, TX_ORANGE);
            if (map [layer][y][x] == 'B')   block (x0 + x * dx, y0 + y * dy, TX_PINK);
            if (map [layer][y][x] == 'C')   block (x0 + x * dx, y0 + y * dy, TX_YELLOW);
            */
            }
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



void block (int x, int y, COLORREF color)
    {
    txSetColor (color);
    txSetFillColor (color);
    txRectangle (x, y, x+1*SIZEB, y+1*SIZEB);
    }
