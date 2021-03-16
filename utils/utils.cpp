#include <iostream>
#include <windowsx.h>
#include "utils.h"
#include "../vars/vars.h"

void utils::RGB::setToUndefined() {
    r = 256;
    g = 256;
    b = 256;
}

void utils::RGB::setColor(COLORREF colorValue) {
    if(colorValue == CLR_INVALID) {
        setToUndefined();
    } else {
        r = GetRValue(colorValue);
        g = GetGValue(colorValue);
        b = GetBValue(colorValue);
    }
}

void onKeyDown(WPARAM wParam) {
    POINT pt;
    GetCursorPos(&pt);
    switch(wParam) {
    case VK_UP:
        SetCursorPos(pt.x, pt.y-1);
        break;
    case VK_DOWN:
        SetCursorPos(pt.x, pt.y+1);
        break;
    case VK_LEFT:
        SetCursorPos(pt.x-1, pt.y);
        break;
    case VK_RIGHT:
        SetCursorPos(pt.x+1, pt.y);
        break;
    }
}

void onMouseMove(LPARAM lParam, HWND hWnd, HDC hLocalDC) {
    int xPos = GET_X_LPARAM(lParam);
    int yPos = GET_Y_LPARAM(lParam);
    COLORREF colorValue;
    colorValue = GetPixel(hLocalDC, xPos, yPos);
    utils::RGB rgb;
    rgb.setColor(colorValue);
    std::string text = std::to_string(xPos);
    text += ", " + std::to_string(yPos);
    text += ": " + std::to_string(rgb.r);
    text += ", " + std::to_string(rgb.g);
    text += ", " + std::to_string(rgb.b);
    SetWindowTextA(hWnd, text.c_str());
}

int indexOfGreatestNumber(int a[], const int length) {
    int tmp = 0;
    int b[10] = {};
    int c[10] = {};
    for(int i=0;i<length;i++) {
        b[i]=a[i];
        c[i]=a[i];
    }

    for(int i = 0; i < length; i++) {
        for(int j = 0; j < length - 1; j++) {
            if(c[j] > c[j+1]) {
                tmp = c[j];
                c[j] = c[j+1];
                c[j+1] = tmp;
            }
        }
    }
    int index = -1;
    for(int i=0;i<length;i++) {
        if(c[length-1]==b[i]) {
            index = i+1;
            break;
        }
    }

    return index;
}

BOOL WINAPI consoleHandler(DWORD signal) {
	if(signal == CTRL_C_EVENT) {
		running = false;
		printf("Ctrl-C Handled\n");
	}

	return TRUE;
}

int loadFilesToBitmapArray(std::string fileNamePrefix, const int suffixMaxValue, const bool bAddOne, HBITMAP characterSet[]) {
	std::string fileName = "";
	HBITMAP hBitmap;
	for(int i = 0; i < suffixMaxValue; i++) {
		fileName = fileNamePrefix + std::to_string(i + (bAddOne ? 1 : 0)) + ".bmp";

		hBitmap = (HBITMAP)::LoadImageA(NULL, fileName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if(hBitmap == NULL) {
			MessageBox(NULL, TEXT("LoadImage Failed1"), TEXT("Error"), MB_OK);
			return FALSE;
		}

		characterSet[i] = hBitmap;
	}

    return 0;
}

int getBitmapDimensions(Dimensions& dimensions, HDC hCaptureDC, HBITMAP hCaptureBitmap, HBITMAP hOldBmp) {
	hOldBmp = (HBITMAP)SelectObject(hCaptureDC, hCaptureBitmap);

	BITMAP qBitmap;
	int retVal = GetObject(reinterpret_cast<HGDIOBJ>(hCaptureBitmap), sizeof(BITMAP), reinterpret_cast<LPVOID>(&qBitmap));
	if(!retVal) {
		MessageBox(NULL, TEXT("GetObject Failed"), TEXT("Error"), MB_OK);
		return FALSE;
	}

	dimensions.width = qBitmap.bmWidth;
	dimensions.height = qBitmap.bmHeight;

	return 0;
}

Stopwatch::Stopwatch() {}

Stopwatch::Stopwatch(std::string stopwatchLabel) {
    this->stopwatchLabel = stopwatchLabel;
    isStopwatchLabelSet = true;
}

void Stopwatch::displayTime() {
    if(!isStopwatchLabelSet)
        std::cout << "Time Elapsed for Unlabeled Block: " << d_duration << std::endl;
    else
        std::cout << "Time Elapsed for " << stopwatchLabel << ": " << d_duration << std::endl;

    std::cout << std::endl;
}

void Stopwatch::start() {
    t1 = std::chrono::steady_clock::now();
}

void Stopwatch::stop(bool bDisplayTime) {
    t2 = std::chrono::steady_clock::now();

    time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    d_duration = time_span.count() * 1000;

    if(bDisplayTime) displayTime();
}

double Stopwatch::getDuration() {
    return d_duration;
}

Dimensions::Dimensions() {}
Dimensions::Dimensions(int width, int height) {
    this->width = width;
    this->height = height;
}

SerialData::SerialData() {
    data1 = 0;
    data2 = 0;
    data3 = 0;
}

SerialData::SerialData(int data1, int data2, int data3) {
    this->data1 = data1;
    this->data2 = data2;
    this->data3 = data3;
}

void SerialData::setData(int data1, int data2, int data3) {
    this->data1 = data1;
    this->data2 = data2;
    this->data3 = data3;
}

void SerialData::printData() {
    std::cout << "RPM Level: (data1): " << data1 << std::endl;
    std::cout << "Gear Number (data2): " << data2 << std::endl;
    std::cout << "Speed Number (data3): " << data3 << std::endl;
    std::cout << std::endl;
}

utils::Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}