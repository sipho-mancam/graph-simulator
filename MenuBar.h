#ifndef MENUBAR_H_INCLUDED
#define MENUBAR_H_INCLUDED
#include <windows.h>

HMENU hMenuBar;
HMENU hFile;
HMENU hSetup;
HMENU hHelp;

#define ID_FILE_SAVE 201
#define ID_FILE_OPEN 202
#define ID_FILE_QUIT 203

#define ID_SETUP_SINE 301
#define ID_SETUP_COS 302
#define ID_SETUP_EDIT 303
#define ID_SETUP_SSETTINGS 304

#define ID_HELP_AUTHOR 401
#define ID_HELP_ABOUT 402
#define ID_HELP_DRAW 403


void initMenu(HWND hwnd)
{

    hFile = CreateMenu();
    hSetup = CreateMenu();
    hHelp = CreateMenu();
    hMenuBar = CreateMenu();

    AppendMenuW(hFile, MF_STRING, ID_FILE_OPEN, L"Open");
    AppendMenuW(hFile, MF_STRING, ID_FILE_SAVE, L"Save");
    AppendMenuW(hFile, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hFile, MF_STRING, ID_FILE_QUIT, L"Quit");

    AppendMenuW(hSetup, MF_STRING, ID_SETUP_SINE, L"Sine");
    AppendMenuW(hSetup, MF_STRING, ID_SETUP_COS, L"Cosine");
    AppendMenuW(hSetup,MF_SEPARATOR, 0, NULL);
    AppendMenuW(hSetup, MF_STRING, ID_SETUP_EDIT, L"Edit");
    AppendMenuW(hSetup, MF_STRING, ID_SETUP_SSETTINGS, L"Settings");

    AppendMenuW(hHelp, MF_STRING, ID_HELP_AUTHOR, L"Author");
    AppendMenuW(hHelp, MF_STRING, ID_HELP_ABOUT, L"About");
    AppendMenuW(hHelp, MF_STRING, ID_HELP_DRAW, L"Draw Graph");

    AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hFile, L"File");
    AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hSetup, L"Setup");
    AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hHelp, L"Help");

    SetMenu(hwnd, hMenuBar);

}




#endif // MENUBAR_H_INCLUDED
