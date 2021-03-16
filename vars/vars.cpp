#include "vars.h"
#include <iostream>

Region::Region(int x, int y, int x2, int y2, int threshold) {
        this->x = x;
        this->y = y;
        this->x2 = x2;
        this->y2 = y2;
        this->threshold = threshold;
}

// Meter::Meter(int x, int y, int x2, int y2, int threshold) {
//         this->x = x;
//         this->y = y;
//         this->x2 = x2;
//         this->y2 = y2;
//         this->threshold = threshold;
// }

Meter::Meter(::Region rpmMeter, ::Region gearNumberMeter, ::Region speedNumberMeter) {
        this->rpmMeter = rpmMeter;
        this->gearNumberMeter = gearNumberMeter;
        this->speedNumberMeter = speedNumberMeter;
}

// void Meter::convert2BinaryImage(HDC hLocalDC, int width, int height) {
//         stopwatch.start();
//         convertToBinaryImage(hLocalDC, width, height, threshold, x, y, x2, y2);
//         stopwatch.stop(false);
//         duration_convertToBinaryImage = stopwatch.getDuration();
// }

// void Meter::save2Bitmap(HDC hWinDC, HDC hLocalDC) {
//         saveBitmap(hWinDC, hLocalDC, x, y, x2, y2);
// }

// void Meter::save2BitmapSpeedNumberDigit(HDC hWinDC, HDC hLocalDC) {
//         struct digitRegionCoordinates{
//                 int x, y, x2, y2;
//         } digit2Region;

//         digit2Region = {235, 120, 250, 147};
//         saveBitmap(hWinDC, hLocalDC, digit2Region.x, digit2Region.y, digit2Region.x2, digit2Region.y2);
// }

void Meter::GetGearNumber(HBITMAP characterSet[], HDC hWinDC, HDC hLocalDC) {
        stopwatch.start();
        gearNumber = getGearNumber(characterSet, hWinDC, hLocalDC, gearNumberMeter.x, gearNumberMeter.y, gearNumberMeter.x2, gearNumberMeter.y2, gearNumberMeter.threshold);
        stopwatch.stop(false);
        duration_solveImage_gearNumberMeter = stopwatch.getDuration();
}

void Meter::GetRpmLevel(HDC hLocalDC) {
        stopwatch.start();
        rpmLevel = getRpmLevel(hLocalDC, rpmMeter.x, rpmMeter.y, rpmMeter.x2, rpmMeter.y2, rpmMeter.threshold);
        stopwatch.stop(false);
        duration_solveImage_rpmMeter = stopwatch.getDuration();
}

void Meter::GetSpeedNumber(HBITMAP characterSet2[], HDC hWinDC, HDC hLocalDC) {
        stopwatch.start();
        speedNumber = getSpeedNumber(characterSet2, hWinDC, hLocalDC, speedNumberMeter.threshold, digitRegions);
        stopwatch.stop(false);
        duration_solveImage_speedNumberMeter = stopwatch.getDuration();
}

void Meter::printMeterValues() {
        std::cout << "RPM Level: " << rpmLevel << std::endl;
        std::cout << "Gear Number: " << gearNumber << std::endl;
        std::cout << "Speed Number: " << speedNumber << std::endl;
        std::cout << std::endl;
}

void Meter::printMeterSolveDurations() {
        std::cout << "RPM Level Solving Clock: " << duration_solveImage_rpmMeter << std::endl;
        std::cout << "Gear Number Solving Clock: " << duration_solveImage_gearNumberMeter << std::endl;
        std::cout << "Speed Number Solving Clock: " << duration_solveImage_speedNumberMeter << std::endl;
        std::cout << std::endl;
}