#include <windows.h>

#include <iostream>
#include <math.h>
#include <vector>
#include <thread>
#include <mutex>

#include "imageSolver.h"
#include "../vars/vars.h"

enum getScore_Select { GETSCORE_GEARNUMBER, GETSCORE_SPEEDNUMBER };

// std::mutex mtx;

int solveImage(HBITMAP characterSet[], HBITMAP characterSet2[], SerialData& serialData, HDC hDesktopDC, HDC hCaptureDC) {

    ::Region gearNumberMeter(156, 4, 195, 74, 250);
    ::Region rpmMeter(5, 120, 181, 125, 186);
    ::Region speedNumberMeter(214, 120, 272, 147, 250);

    Meter meter(rpmMeter, gearNumberMeter, speedNumberMeter);

    int data1 = 0, data2 = 0, data3 = 0;

    // rpmMeter.GetRpmLevel(hCaptureDC);
    // gearNum.GetGearNumber(characterSet, hDesktopDC, hCaptureDC);
    // speedNum.GetSpeedNumber(characterSet2, hDesktopDC, hCaptureDC);

    meter.GetRpmLevel(hCaptureDC);
    meter.GetGearNumber(characterSet, hDesktopDC, hCaptureDC);
    meter.GetSpeedNumber(characterSet2, hDesktopDC, hCaptureDC);

    //data1 = (meter.rpmLevel / ( rpmMeter.x2 - rpmMeter.x )) * 16;
    int rpmLevel = meter.rpmLevel;
    int rpmLevel_x = rpmMeter.x;
    int rpmLevel_x2 = rpmMeter.x2;
    data1 = ( (float)rpmLevel / (rpmLevel_x2 - rpmLevel_x) ) * 16;
    data2 = meter.gearNumber;
    data3 = meter.speedNumber;

    // std::cout << "RPM Level: " << meter.rpmLevel << std::endl;
    // std::cout << "Gear Number: " << meter.gearNumber << std::endl;
    // std::cout << "Speed Number: " << meter.speedNumber << std::endl;

    // std::cout << "RPM Level: (data1): " << data1 << std::endl;
    // std::cout << "Gear Number (data2): " << data2 << std::endl;
    // std::cout << "Speed Number (data3): " << data3 << std::endl;

    // std::cout << "RPM Level Solving Clock: " << meter.duration_solveImage_rpmMeter << std::endl << std::endl;
    // std::cout << "Gear Number Solving Clock: " << meter.duration_solveImage_gearNumberMeter << std::endl << std::endl;
    // std::cout << "Speed Number Solving Clock: " << meter.duration_solveImage_speedNumberMeter << std::endl;

    serialData.setData(data1, data2, data3);

    meter.printMeterValues();
    serialData.printData();
    meter.printMeterSolveDurations();

    return 0;
}

int getSpeedNumber(HBITMAP characterSet2[], HDC hWinDC, HDC hLocalDC, int threshold, digitRegionCoordinates digitRegions[]) {

    // struct digitRegionCoordinates{
    //     int x, y, x2, y2;
    // };

    // digitRegionCoordinates digit1Region = {214, 120, 228, 147};
    // digitRegionCoordinates digit2Region = {235, 120, 250, 147};
    // digitRegionCoordinates digit3Region = {257, 120, 272, 147};
    // digitRegionCoordinates digitRegions[3] = {digit1Region, digit2Region, digit3Region};

    int digits[3] = {};

    bool isTwoDigitNumber = false;
    bool isOneDigitNumber = false;

    for(int i = 2; i >= 0; i--) {
        const int length = 10;
        int scores[length] = {};

        int x = digitRegions[i].x;
        int y = digitRegions[i].y;
        int x2 = digitRegions[i].x2;
        int y2 = digitRegions[i].y2;

        // std::vector<std::thread> threads;

        for(int j = 0; j < length; j++) {
            if(!isTwoDigitNumber && i == 0 && j >= 4)
                break;

            getScore_SpeedNumber(i, isTwoDigitNumber, isOneDigitNumber, scores, j, characterSet2, hLocalDC, x, y, x2, y2, threshold);

            // threads.push_back(std::thread(getScore_SpeedNumber, i, std::ref(isTwoDigitNumber), std::ref(isOneDigitNumber), scores, j, characterSet2, hLocalDC, x, y, x2, y2, threshold));
        }

        // for(auto& th : threads) th.join();

        // threads.clear();

        digits[i] = indexOfGreatestNumber(scores, length) - 1;
    }

    int speedNumber = 0;

    if(isOneDigitNumber) {
        speedNumber = digits[0];
    } else if(isTwoDigitNumber) {
        speedNumber = digits[0] * 10 + digits[1];
    } else {
        speedNumber = digits[0] * 100 + digits[1] * 10 + digits[2];
    }

    return speedNumber;
}

void getScore_SpeedNumber(int i, bool& isTwoDigitNumber, bool& isOneDigitNumber, int scores[], int j, HBITMAP characterSet2[], HDC hLocalDC, int x, int y, int x2, int y2, int threshold) {
    HWND hDesktopWnd = GetDesktopWindow();
	HDC hWinDC = GetDC(hDesktopWnd);
    HDC hCharacterDC = CreateCompatibleDC(hWinDC);
    if(hCharacterDC == NULL) {
        MessageBox(NULL, TEXT("CreateCompatibleDC Failed1"), TEXT("Error"), MB_OK);
        // return FALSE;
    }
    HBITMAP hOldBmp = (HBITMAP)SelectObject(hCharacterDC, characterSet2[j]);
    if(hOldBmp == NULL) {
        MessageBox(NULL, TEXT("SelectObject Failed1"), TEXT("Error"), MB_OK);
        // return FALSE;
    }


    int score = getScore(GETSCORE_SPEEDNUMBER, hWinDC, hLocalDC, hCharacterDC, x, y, x2, y2, threshold);

    SelectObject(hCharacterDC, hOldBmp);
    DeleteObject(hOldBmp);

    // mtx.lock();
    if(i == 2 && j == 0 && score == 0) {
        // mtx.lock();
        isTwoDigitNumber = true;
        // mtx.unlock();
    } else if(i == 1 && j == 0 && score == 0) {
        // mtx.lock();
        isOneDigitNumber = true;
        // mtx.unlock();
    }

    scores[j] = score;

    ReleaseDC(hDesktopWnd,hWinDC);
    DeleteDC(hCharacterDC);

    // mtx.unlock();
}

int getGearNumber(HBITMAP characterSet[], HDC hWinDC, HDC hLocalDC, int x, int y, int x2, int y2, int threshold) {

    HDC hCharacterDC = CreateCompatibleDC(hWinDC);
    if(hCharacterDC == NULL) {
        MessageBox(NULL, TEXT("CreateCompatibleDC Failed1"), TEXT("Error"), MB_OK);
        return FALSE;
    }

    const int length = 6; // CHANGE: TO 7 AFTER ADDING 7TH GEARNUMBER CHARACTER TO THE GEAR NUMBER CHARACTERSET
    int scores[length];

    for(int i = 0; i < length; i++) {
        getScore_GearNumber(scores, i, hCharacterDC, characterSet, hWinDC, hLocalDC, x, y, x2, y2, threshold);
    }

    DeleteDC(hCharacterDC);

    return indexOfGreatestNumber(scores, length);
}

void getScore_GearNumber(int scores[], int i, HDC hCharacterDC, HBITMAP characterSet[], HDC hWinDC, HDC hLocalDC, int x, int y, int x2, int y2, int threshold) {

    HBITMAP hOldBmp = (HBITMAP)SelectObject(hCharacterDC, characterSet[i]);
    if(hOldBmp == NULL) {
        MessageBox(NULL, TEXT("SelectObject Failed_GearNumSolve"), TEXT("Error"), MB_OK);
        // return FALSE;
    }
    int score = getScore(GETSCORE_GEARNUMBER, hWinDC, hLocalDC, hCharacterDC, x, y, x2, y2, threshold);

    SelectObject(hCharacterDC, hOldBmp);
    DeleteObject(hOldBmp);

    scores[i] = score;
}

int getScore(int characterSetSelect, HDC hWinDC, HDC hLocalDC, HDC hCharacterDC, int x, int y, int x2, int y2, int threshold) {

    int score = 0;
    int iStepCount = 0;
    int jStepCount = 0;

    if(characterSetSelect == GETSCORE_GEARNUMBER) {
        iStepCount = 5, jStepCount = 25;
    } else {
        iStepCount = 1/*1*/, jStepCount = 2/*7*/;
    }

    for(int j = y; j < y2; j += jStepCount) {
        for(int i = x; i < x2; i+=iStepCount) {
            if((GetRValue(GetPixel(hLocalDC, i, j)) >= threshold
            && GetGValue(GetPixel(hLocalDC, i, j)) >= threshold
            && GetBValue(GetPixel(hLocalDC, i, j)) >= threshold)
            && GetPixel(hCharacterDC, i - x, j - y)) {
                score++;
            }
        }
    }

    return score;
}

int getRpmLevel(HDC hLocalDC, int x, int y, int x2, int y2, int threshold) {

    int offsetFromLeft = 0;
    int j = (y2 - y) / 2;

    for(int i = x; i <= x2; ++i) {
        offsetFromLeft = i - 1;
        if(GetRValue(GetPixel(hLocalDC, i, y)) < threshold) {
            break;
        }
    }

    return offsetFromLeft - x + 1;
}

void convertToBinaryImage(HDC hLocalDC, int width, int height, int threshold, int x, int y, int x2, int y2) {

    for(int j = 0; j < height; j++) {
        for(int i = 0; i < width; i++) {
            if(i >= x && i <= x2 && j >= y && j <= y2) {
                if(GetRValue(GetPixel(hLocalDC, i, j)) >= threshold && GetGValue(GetPixel(hLocalDC, i, j)) >= threshold && GetBValue(GetPixel(hLocalDC, i, j)) >= threshold) {
                    SetPixel(hLocalDC, i, j, RGB(255, 255, 255));
                } else {
                    SetPixel(hLocalDC, i, j, RGB(0, 0, 0));
                }
            }
        }
    }

    return;
}