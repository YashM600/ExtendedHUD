#ifndef VARS_H
#define VARS_H

#include "../imageSolver/imageSolver.h"
#include "../saveBitmapImage/saveBitmapImage.h"
#include "../utils/utils.h"

struct Region {
    int x, y, x2, y2;
    int threshold;

    Region() {}
    Region(int x, int y, int x2, int y2, int threshold);
};

struct Meter {
        // int x, y, x2, y2;
        // int threshold;

        ::Region rpmMeter;
        ::Region gearNumberMeter;
        ::Region speedNumberMeter;

        int gearNumber;
        int rpmLevel;
        int speedNumber;

        Stopwatch stopwatch;

        double duration_convertToBinaryImage;
        double duration_solveImage_rpmMeter;
        double duration_solveImage_gearNumberMeter;
        double duration_solveImage_speedNumberMeter;

        digitRegionCoordinates digit1Region = {214, 120, 228, 147};
        digitRegionCoordinates digit2Region = {235, 120, 250, 147};
        digitRegionCoordinates digit3Region = {257, 120, 272, 147};
        digitRegionCoordinates digitRegions[3] = {digit1Region, digit2Region, digit3Region};

        // Meter(int x, int y, int x2, int y2, int threshold);
        Meter(::Region rpmMeter, ::Region gearNumberMeter, ::Region speedNumberMeter);

        void convert2BinaryImage(HDC hLocalDC, int width, int height);
        void save2Bitmap(HDC hWinDC, HDC hLocalDC);
        void save2BitmapSpeedNumberDigit(HDC hWinDC, HDC hLocalDC);
        void GetGearNumber(HBITMAP characterSet[], HDC hWinDC, HDC hLocalDC);
        void GetRpmLevel(HDC hLocalDC);
        void GetSpeedNumber(HBITMAP characterSet2[], HDC hWinDC, HDC hLocalDC);

        void printMeterValues();
        void printMeterSolveDurations();
};

#endif