#include <windows.h>
#include <tchar.h>
#include <iostream>
#include "MenuBar.h"
#include "Interfaces.h"
#include "GRAPHOUT.h"
#include <math.h>

#define TIMER_ID   20

LRESULT CALLBACK windowProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK windowProc2(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

using namespace std;


DATA sineValues(int index, int freq, float phase);
DATA cosineValues(int freq, int amp, float phase);
void pullData();

TCHAR winClassName [100] = _T("Main Window");
TCHAR win2Name [100] = _T("Child Window");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
    //create the required class variables
    MSG msg;
    HWND hwnd;
    WNDCLASSEX winclex;
    WNDCLASSEX win2;

    winclex.hInstance = hInstance;
    winclex.lpfnWndProc = windowProcedure;
    winclex.lpszClassName = winClassName;
    winclex.style = CS_DBLCLKS;
    winclex.lpszMenuName = NULL;
    winclex.cbSize = sizeof(WNDCLASSEX);
    winclex.hbrBackground = CreateSolidBrush(RGB(180,180,180));
    winclex.hCursor = LoadCursor(NULL, IDC_ARROW);
    winclex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    winclex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    winclex.cbClsExtra = 0;
    winclex.cbWndExtra = 0;

    win2.hInstance = hInstance;
    win2.lpfnWndProc = windowProc2;
    win2.lpszClassName = win2Name;
    win2.style = CS_DBLCLKS;
    win2.lpszMenuName = NULL;
    win2.cbSize = sizeof(WNDCLASSEX);
    win2.hbrBackground = CreateSolidBrush(RGB(180,180,180));
    win2.hCursor = LoadCursor(NULL, IDC_ARROW);
    win2.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    win2.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    win2.cbClsExtra = 0;
    win2.cbWndExtra = 0;

    // register the window class upon creation

    if(!RegisterClassEx(&winclex))return 2;
    if(!RegisterClassEx(&win2))return 2;

    // create the Window from the class template you just specified

    hwnd = CreateWindowEx(0,
                          winClassName,
                          _T("Assignment 3"),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          320,
                          320,
                          HWND_DESKTOP,
                          (HMENU)0,
                          hInstance,
                          NULL);

    // After creating the window, make it show up on the desktop

    ShowWindow(hwnd, nCmdShow);
    // Retrieve messages from the OS's message queue with GetMessage()
    while(GetMessage(&msg, 0, 0, NULL))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

static int freq, amp;
static float phase;
int len;
static DATA cosine, sine, activeData;

LRESULT CALLBACK windowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    //Process Messages received from the queue with the call back function
    static HWND hFunType, hSecondWindow;
    static int index;
    wchar_t tempTxt[100];
    //wchar_t readData [500];
    static int toggle = 1;
    static HPEN hPen;
    int messBoxButton;
    //cout<<"From Window 1: "<<msg<<endl;
    switch(msg)
    {
    case WM_CREATE:
        initMenu(hwnd);
        EnableMenuItem(hFile, ID_FILE_OPEN, MF_GRAYED);
        EnableMenuItem(hFile, ID_FILE_SAVE, MF_GRAYED);
        hFunType = CreateWindowW(L"Static", L"Sine-Wave", WS_CHILD|WS_VISIBLE, 100, 20, 100, 30, hwnd, (HMENU)0, NULL, NULL);
        editInterface(hwnd);
        settingsLayout(hwnd);
        HideSettingsLayout();
        CheckMenuItem(hSetup, ID_SETUP_EDIT, MF_CHECKED);
        CheckMenuItem(hSetup, ID_SETUP_SINE, MF_CHECKED);
        pullData();

        hSecondWindow = CreateWindowEx(0,win2Name, _T("New window"),
                                               WS_OVERLAPPED, CW_USEDEFAULT,
                                               CW_USEDEFAULT, 540, 540,
                                               hwnd, NULL, NULL, NULL);

        //initialize sine
        sine.amp = amp;
        sine.freq = freq;
        sine.phase = phase;
        sine = sineValues(sine.freq, sine.amp,sine.phase);
        //initialize cosine
        cosine.amp = amp;
        cosine.freq = freq;
        cosine.phase = phase;
        cosine = cosineValues(cosine.freq, cosine.amp, cosine.phase);

        activeData = sine;
        break;

    case WM_COMMAND:
        if(HIWORD(wParam)==BN_CLICKED)
        {
            switch(LOWORD(wParam))
            {
                //processing the scrolling button
            case BUTTON_LEFT_ARROW:

                //change data input
                len = GetWindowTextLengthW(editBox)+1;
                GetWindowTextW(editBox, tempTxt, len);
                activeData.values[index] = _wtof(tempTxt);

                if(index>0)index--;
                swprintf(tempTxt,L"Index: %d",index);
                SetWindowTextW(indexBox, tempTxt);
                swprintf(tempTxt,L"%.5f",activeData.values[index]);
                SetWindowTextW(editBox, tempTxt);
                len = GetWindowTextLengthW(editBox)+1;
                GetWindowTextW(editBox, tempTxt, len);
                activeData.values[index] = _wtof(tempTxt);
                if(toggle)sine = activeData;
                else cosine = activeData;
                SetFocus(hwnd);
                break;

            case BUTTON_RIGHT_ARROW:

                //change data input
                len = GetWindowTextLengthW(editBox)+1;
                GetWindowTextW(editBox, tempTxt, len);
                activeData.values[index] = _wtof(tempTxt);

                if(index<361)index++;
                swprintf(tempTxt,L"Index: %d",index);
                SetWindowTextW(indexBox, tempTxt);
                swprintf(tempTxt,L"%.5f",activeData.values[index]);
                SetWindowTextW(editBox, tempTxt);
                if(toggle)sine = activeData;
                else cosine = activeData;
                SetFocus(hwnd);
                break;

                //processing menu buttons
            case ID_FILE_QUIT:
                PostQuitMessage(0);
                break;

            case ID_SETUP_EDIT:
                pullData();
                HideSettingsLayout();
                HideEditLayout();
                CheckMenuItem(hSetup, ID_SETUP_EDIT, MF_CHECKED);
                CheckMenuItem(hSetup, ID_SETUP_SSETTINGS, MF_UNCHECKED);
                editInterface(hwnd);
                if(toggle)
                {
                    sine.amp = amp;
                    sine.freq = freq;
                    sine.phase = phase;
                    sine = sineValues(sine.freq, sine.amp, sine.phase);
                    activeData = sine;
                }
                else
                {
                    cosine.amp = amp;
                    cosine.freq = freq;
                    cosine.phase = phase;
                    cosine = cosineValues(cosine.freq, cosine.amp, cosine.phase);
                    activeData = cosine;
                }
                break;

            case ID_SETUP_SSETTINGS:
                HideEditLayout();
                CheckMenuItem(hSetup, ID_SETUP_EDIT, MF_UNCHECKED);
                CheckMenuItem(hSetup, ID_SETUP_SSETTINGS, MF_CHECKED);
                settingsLayout(hwnd);
                if(toggle)
                {
                    swprintf(tempTxt,L"%d", sine.amp);
                    SetWindowTextW(hAmpVal, tempTxt);
                    swprintf(tempTxt,L"%d", sine.freq);
                    SetWindowTextW(hFreqVal, tempTxt);
                    swprintf(tempTxt,L"%.2f", sine.phase);
                    SetWindowTextW(hPhaseVal, tempTxt);
                }
                else
                {
                    swprintf(tempTxt,L"%d", cosine.amp);
                    SetWindowTextW(hAmpVal, tempTxt);
                    swprintf(tempTxt,L"%d", cosine.freq);
                    SetWindowTextW(hFreqVal, tempTxt);
                    swprintf(tempTxt,L"%.5f", cosine.phase);
                    SetWindowTextW(hPhaseVal, tempTxt);
                }
                break;

            case ID_SETUP_SINE:

                pullData();
                CheckMenuItem(hSetup, ID_SETUP_SINE, MF_CHECKED);
                CheckMenuItem(hSetup, ID_SETUP_COS, MF_UNCHECKED);
                SetWindowTextW(hFunType, L"Sine-Wave");
                toggle = 1;
                activeData = sine;
                index = 0;
                swprintf(tempTxt, L"%.5f",sine.values[index]);
                SetWindowTextW(editBox, tempTxt);
                SetWindowTextW(indexBox, L"Index: 0");

                swprintf(tempTxt,L"%d", sine.amp);
                SetWindowTextW(hAmpVal, tempTxt);
                swprintf(tempTxt,L"%d", sine.freq);
                SetWindowTextW(hFreqVal, tempTxt);
                swprintf(tempTxt,L"%.2f", sine.phase);
                SetWindowTextW(hPhaseVal, tempTxt);

                break;

            case ID_SETUP_COS:

                CheckMenuItem(hSetup, ID_SETUP_COS, MF_CHECKED);
                CheckMenuItem(hSetup, ID_SETUP_SINE, MF_UNCHECKED);
                SetWindowTextW(hFunType, L"Cosine-Wave");
                toggle = 0;
                activeData = cosine;

                index = 0;
                swprintf(tempTxt, L"%.5f", cosine.values[index]);
                SetWindowTextW(editBox, tempTxt);
                SetWindowTextW(indexBox, L"Index: 0");

                swprintf(tempTxt,L"%d", cosine.amp);
                SetWindowTextW(hAmpVal, tempTxt);
                swprintf(tempTxt,L"%d", cosine.freq);
                SetWindowTextW(hFreqVal, tempTxt);
                swprintf(tempTxt,L"%.2f", cosine.phase);
                SetWindowTextW(hPhaseVal, tempTxt);

                break;
            case ID_HELP_ABOUT:
                MessageBox(hwnd, "Something about the application", "About the App", MB_OK | MB_ICONINFORMATION);
                break;
            case ID_HELP_AUTHOR:
                MessageBox(hwnd, "Something about Sthembiso Musana", "Author :Sthe", MB_OK| MB_ICONINFORMATION);
                break;

            case ID_HELP_DRAW:
                ShowWindow(hSecondWindow, SW_SHOW);
                DrawGrid(hSecondWindow, hPen);
                DrawSineGrap(hSecondWindow, hPen, activeData);
                SetFocus(hSecondWindow);
                messBoxButton = MessageBoxW(hSecondWindow, L"UP/Down Arrow to change Freq \nLEFT/RIGHT ARROW to change Amplitude", L"Help Window", MB_OK | MB_ICONINFORMATION);

                if(messBoxButton == IDOK)SetFocus(hSecondWindow);
                break;
            }
        }
        break;

    case WM_KEYDOWN:

        if(wParam == VK_ESCAPE)PostQuitMessage(0);

        switch(wParam)
        {
        case VK_LEFT:
            if(index>0)index--;
            swprintf(tempTxt,L"Index: %d",index);
            SetWindowTextW(indexBox, tempTxt);
            swprintf(tempTxt,L"%.5f",activeData.values[index]);
            SetWindowTextW(editBox, tempTxt);
            len = GetWindowTextLengthW(editBox)+1;
            GetWindowTextW(editBox, tempTxt, len);
            activeData.values[index] = _wtof(tempTxt);
            break;

        case VK_RIGHT:
            if(index<361)index++;
            swprintf(tempTxt,L"Index: %d",index);
            SetWindowTextW(indexBox, tempTxt);
            swprintf(tempTxt,L"%.5f",activeData.values[index]);
            SetWindowTextW(editBox, tempTxt);
            len = GetWindowTextLengthW(editBox)+1;
            GetWindowTextW(editBox, tempTxt, len);
            activeData.values[index] = _wtof(tempTxt);
            break;
        }

        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

LRESULT CALLBACK windowProc2(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    MSG msg1;
    static HWND hButton, hStopBut, hQuitBut;
    static int xPos, yPos;
    static HPEN hPen;
    static RECT myButton;
    static HDC hdc;
    static HBRUSH myBrush;
    static int exitLoop=1;
    //cout<<"From Window 2: "<<msg<<endl;
    switch(msg)
    {
    case WM_CREATE:
        hdc = GetDC(hwnd);
        hButton = CreateWindowW(L"Button", L"Start", WS_CHILD| WS_VISIBLE, 5, 5, 70, 30, hwnd, (HMENU)19, NULL, NULL);
        hStopBut = CreateWindowW(L"Button", L"Stop", WS_CHILD|WS_VISIBLE, 80, 5, 70, 30, hwnd, (HMENU)21, NULL, NULL);
        hQuitBut = CreateWindowW(L"Button", L"QUIT", WS_CHILD|WS_VISIBLE, 165, 5, 70, 30, hwnd, (HMENU)22, NULL, NULL);
        hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
        myBrush = CreateSolidBrush(RGB(130,200,140));
        break;

    case WM_TIMER:
        if(activeData.phase<90)
        {
            activeData.phase += 0.1;
        }
        else activeData.phase =0;

        if(activeData.values[0]==sine.values[0])
        {
            activeData = sineValues(activeData.freq, activeData.amp, activeData.phase);
        }
        else activeData = cosineValues(activeData.freq, activeData.amp, activeData.phase);

        DrawGrid(hwnd, hPen);
        DrawSineGrap(hwnd, hPen, activeData);
        break;

    case WM_KEYDOWN:
        switch(wParam)
        {
        case VK_LEFT:
            if(activeData.amp>1)activeData.amp--;
            break;
        case VK_RIGHT:
            if(activeData.amp<10)activeData.amp++;
            break;
        case VK_ESCAPE:
            ShowWindow(hwnd, SW_HIDE);
            break;
        case VK_UP:
            if(activeData.freq<5)activeData.freq++;
            break;
        case VK_DOWN:
            if(activeData.freq>1)activeData.freq--;
            break;
        }


        DrawGrid(hwnd, hPen);
        DrawSineGrap(hwnd, hPen, activeData);
        std::cout<<"I'm executing..."<<std::endl;
        break;

    case WM_COMMAND:
        if(HIWORD(wParam)== BN_CLICKED)
        switch(LOWORD(wParam))
        {
        case 19:
            SetTimer(hwnd, TIMER_ID, 30, (TIMERPROC)NULL);
            SetFocus(hwnd);
            break;
        case 21:
            KillTimer(hwnd, TIMER_ID);
            SetFocus(hwnd);
            //ShowWindow(hwnd, SW_HIDE);
            break;
        case 22:
            ShowWindow(hwnd, SW_HIDE);
            break;
        };
        break;

    case WM_DESTROY:
        ShowWindow(hwnd, SW_HIDE);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}


DATA sineValues(int freq, int amp, float phase)
{
    DATA sine;

    sine.phase = phase;
    sine.freq = freq;
    sine.amp = amp;

    for(int i=0; i<360; i++)
    {
        sine.values[i] = sine.amp * sin(sine.freq*(i/360.1 * 2* M_PI) + sine.phase);
    }

    return sine;
}


DATA cosineValues(int freq, int amp, float phase)
{
    DATA cosine;

    cosine.amp = amp;
    cosine.freq = freq;
    cosine.phase = phase;

    for(int i=0; i<360; i++)
    {
        cosine.values[i] = cosine.amp * cos(cosine.freq * (i/360.1 * 2* M_PI) + cosine.phase);
    }

    return cosine;
}

void pullData()
{
    wchar_t tempTxt[500];

    len = GetWindowTextLength(hFreqVal)+1;
    GetWindowTextW(hFreqVal,tempTxt, len);

    freq = _wtoi(tempTxt);

    len = GetWindowTextLength(hAmpVal)+1;
    GetWindowTextW(hAmpVal,tempTxt, len);

    amp = _wtoi(tempTxt);

    len = GetWindowTextLength(hPhaseVal)+1;
    GetWindowTextW(hPhaseVal,tempTxt, len);

    phase = _wtof(tempTxt);
}

