#ifndef INTERFACES_H_INCLUDED
#define INTERFACES_H_INCLUDED

#include <windows.h>
#include <iostream>

#define BUTTON_LEFT_ARROW 100
#define BUTTON_RIGHT_ARROW 101

static HWND hAmp, hFreq, hPhase;
static HWND hAmpVal, hFreqVal, hPhaseVal;

static HWND indexBox, editBox, leftArrow, rightArrow;

void editInterface(HWND hwnd)
{
    indexBox = CreateWindowW(L"Static", L"Index: 0", WS_CHILD | WS_VISIBLE, 20, 110, 100, 30, hwnd, (HMENU)0, NULL, NULL);
    editBox = CreateWindowW(L"Edit", L"0.00000", WS_CHILD | WS_VISIBLE, 170, 110, 100, 30, hwnd, (HMENU)0, NULL, NULL);
    leftArrow = CreateWindowW(L"Button", L"<", WS_CHILD | WS_VISIBLE , 20+70, 150, 30, 30, hwnd, (HMENU)BUTTON_LEFT_ARROW, NULL, NULL);
    rightArrow = CreateWindowW(L"Button", L">", WS_CHILD | WS_VISIBLE , 80+20+70, 150, 30, 30, hwnd, (HMENU)BUTTON_RIGHT_ARROW, NULL, NULL);
}

void settingsLayout(HWND hwnd)
{
    //Static Boxes
    hAmp = CreateWindowW(L"Static", L"Amplitude: ", WS_CHILD | WS_VISIBLE, 10, 80, 140, 30, hwnd, (HMENU)0, NULL, NULL);
    hFreq = CreateWindowW(L"Static", L"Frequency: ", WS_CHILD | WS_VISIBLE, 10, 120, 140, 30, hwnd, (HMENU)0, NULL, NULL);
    hPhase = CreateWindowW(L"Static", L"Phase: ", WS_CHILD | WS_VISIBLE, 10, 160, 140, 30, hwnd, (HMENU)0, NULL, NULL);

    hAmpVal = CreateWindowW(L"Edit", L"10", WS_CHILD | WS_VISIBLE, 160, 80, 140, 30, hwnd, (HMENU)0, NULL, NULL);
    hFreqVal = CreateWindowW(L"Edit", L"1", WS_CHILD | WS_VISIBLE, 160, 120, 140, 30, hwnd, (HMENU)0, NULL, NULL);
    hPhaseVal = CreateWindowW(L"Edit", L"0", WS_CHILD | WS_VISIBLE, 160, 160, 140, 30, hwnd, (HMENU)0, NULL, NULL);

}

void HideSettingsLayout()
{
    ShowWindow(hAmp,SW_HIDE);
    ShowWindow(hAmpVal,SW_HIDE);
    ShowWindow(hFreq,SW_HIDE);
    ShowWindow(hFreqVal,SW_HIDE);
    ShowWindow(hPhase,SW_HIDE);
    ShowWindow(hPhaseVal,SW_HIDE);
}


void HideEditLayout()
{
    ShowWindow(indexBox,SW_HIDE);
    ShowWindow(editBox,SW_HIDE);
    ShowWindow(leftArrow,SW_HIDE);
    ShowWindow(rightArrow,SW_HIDE);
}

#endif // INTERFACES_H_INCLUDED
