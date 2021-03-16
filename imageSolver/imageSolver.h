#ifndef IMAGESOLVER_H
#define IMAGESOLVER_H

#include <windows.h>
#include "..\utils\utils.h"

int solveImage(HBITMAP characterSet[], HBITMAP characterSet2[], SerialData& serialData, HDC hDesktopDC, HDC hCaptureDC);

int getGearNumber(HBITMAP characterSet[], HDC hWinDC, HDC hLocalDC, int x, int y, int x2, int y2, int threshold);
void getScore_GearNumber(int scores[], int i, HDC hCharacterDC, HBITMAP characterSet[], HDC hWinDC, HDC hLocalDC, int x, int y, int x2, int y2, int threshold);

int getRpmLevel(HDC hLocalDC, int x, int y, int x2, int y2, int threshold);

int getSpeedNumber(HBITMAP characterSet2[], HDC hWinDC, HDC hLocalDC, int threshold, digitRegionCoordinates digitRegions[]);
void getScore_SpeedNumber(int i, bool& isTwoDigitNumber, bool& isOneDigitNumber, int scores[], int j, HBITMAP characterSet2[], HDC hLocalDC, int x, int y, int x2, int y2, int threshold);

int getScore(int characterSetSelect, HDC hWinDC, HDC hLocalDC, HDC hCharacterDC, int x, int y, int x2, int y2, int threshold);

void convertToBinaryImage(HDC hLocalDC, int width, int height, int threshold, int x, int y, int x2, int y2);

#endif