#ifndef UTILS_H
#define UTILS_H

#include <windows.h>
#include <string>
#include <chrono>

extern bool running;

namespace utils {
	struct RGB {
		int r = 256, g = 256, b = 256;

		void setToUndefined();
		void setColor(COLORREF colorValue);
	};
}

void onKeyDown(WPARAM wParam);
void onMouseMove(LPARAM lParam, HWND hWnd, HDC hLocalDC);
int indexOfGreatestNumber(int a[], const int length);
BOOL WINAPI consoleHandler(DWORD signal);

struct Stopwatch {
	std::string stopwatchLabel;
	double d_duration = 0;

	std::chrono::duration<double> time_span;
	std::chrono::steady_clock::time_point t1;
	std::chrono::steady_clock::time_point t2;

	bool isStopwatchLabelSet = false;

	Stopwatch();
	Stopwatch(std::string stopwatchLabel);

	void displayTime();
	void start();
	void stop(bool bDisplayTime = true);
	double getDuration();
};

struct Dimensions {
	int width;
	int height;

	Dimensions();
	Dimensions(int width, int height);
};

int loadFilesToBitmapArray(std::string fileNamePrefix, const int suffixMaxValue, const bool bAddOne, HBITMAP characterSet[]);
int getBitmapDimensions(Dimensions& size, HDC hCaptureDC, HBITMAP hCaptureBitmap, HBITMAP hOldBmp);

struct SerialData {
	int data1, data2, data3;

	SerialData();
	SerialData(int data1, int data2, int data3);

	void setData(int data1, int data2, int data3);
	void printData();
};

namespace utils {
	struct Point {
		int x, y;

		Point(int x, int y);
	};
}

struct digitRegionCoordinates{
	int x, y, x2, y2;
};

#endif