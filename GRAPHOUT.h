#ifndef GRAPHOUT_H_INCLUDED
#define GRAPHOUT_H_INCLUDED

struct graphData{
int freq;
float phase;
int amp;
float values[361];
};

typedef struct graphData DATA;

void DrawSineGrap(HWND hwnd, HPEN hPen, DATA graph)
{

    HDC hdc;
    int x, y;
    hdc = GetDC(hwnd);
    HPEN hMyPen = CreatePen(PS_SOLID, 3, RGB(80, 60, 200));

     SelectObject(hdc, hMyPen);
     for(int i = 0; i<361;i++)
     {
         x = 60+ i * 1.2; // getting the x co-ordinate
         y = 70+170-170*graph.amp/10* graph.values[i]/ graph.amp;

         MoveToEx(hdc, x, y, NULL);
         if(i<359)LineTo(hdc, x+1.39, 70+170-170*graph.amp/10*graph.values[i+1]/ graph.amp);

     }
     ReleaseDC(hwnd, hdc);
}


void DrawGrid(HWND hwnd, HPEN hPen)
{
    RECT myRect;
    HDC hdc;
    HPEN hThickPen = CreatePen(PS_SOLID, 3, RGB(0,0,0));


    hdc = GetDC(hwnd);
    SetRect(&myRect, 0, 35, 540, 540);
    FillRect(hdc, &myRect, CreateSolidBrush(RGB(180, 180, 0)));
    SelectObject(hdc, hPen);
    int toggle = 1;

    for(int i=0; i<13; i++)
    {
        if(toggle)
        {
            if(i == 6)
            {
                SelectObject(hdc, hThickPen);
            }
            MoveToEx(hdc, 60, 60+i*30, NULL);
            LineTo(hdc, 60+440, 60+i*30);
            if(i == 6)
            {
                SelectObject(hdc, hPen);
            }

        }
        else
        {
            if(i == 1)
            {
                SelectObject(hdc, hThickPen);
            }
            MoveToEx(hdc, 60+i*40 - 40, 60, NULL);
            LineTo(hdc, 60+i*40 -40, 60+360);
            if(i == 1)
            {
                SelectObject(hdc, hPen);
            }
        }

        if(i==12 && toggle == 1)
        {
            toggle = 0;
            i=0;
        }

    }
    ReleaseDC(hwnd, hdc);
}


#endif // GRAPHOUT_H_INCLUDED
