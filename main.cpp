#include <iostream>
#include <windows.h>
#include <gdiplus.h>

#include "imageSolver/imageSolver.h"
#include "captureScreen/captureScreen.h"
#include "serialDriver/serialDriver.h"
#include "utils/utils.h"

#pragma comment(lib, "User32")
#pragma comment(lib, "Gdi32")
#pragma comment(lib, "Kernel32")
#pragma comment(lib, "Shell32")
#pragma comment(lib, "Gdiplus.lib")

bool running = true;

int main(int argc, char* argv[]) {

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);

	// ==================================================================== //
	// SET CTRL-C KEYBOARD INTERRUPT HANDLER

	if(!SetConsoleCtrlHandler(consoleHandler, TRUE)) {
		printf("\nError: Couldn't Set Control Handler");
		return 1;
	}

	// ==================================================================== //
	// DECLARE AND INITIALIZE CHARACTERSET ARRAYS
	HBITMAP characterSet[6] = {0};
	HBITMAP characterSet2[10] = {0};

	HBITMAP hBitmap;

	// FUNCTIONS TO LOAD FILES INTO CHARACTERSET ARRAYS
	loadFilesToBitmapArray("./GearNumberCharacterSet/gearNumber", 6, true, characterSet);
	loadFilesToBitmapArray("./SpeedNumberCharacterSet/speedNumberDigit", 10, false, characterSet2);

	// ==================================================================== //
	// CREATE SERIAL OBJECT AND DECLARE VARIABLE TO STORE SERIAL DATA

	customSerial::Serial serial("");
	SerialData serialData;

	// ==================================================================== //
	// DECLARE AND INITIALIZE VARIABLES FOR SCREEN CAPTURE

	HWND hDesktopWnd = GetDesktopWindow();
	HDC hDesktopDC = GetDC(hDesktopWnd);
	HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
	HBITMAP hCaptureBitmap;
	HBITMAP hOldBmp = 0;

	// ==================================================================== //
	// DECLARE AND INITIALIZE VARIABLES FOR POINT COORDINATES AND DIMENSIONS OF THE SCREEN CAPTURE REGION

	utils::Point point(40, 15);
	Dimensions finalDimensions(310, 175);

	// CREATE HBITMAP TO HOLD BITMAP OF THE CAPTURED SCREEN
	hCaptureBitmap = CreateCompatibleBitmap(hDesktopDC, finalDimensions.width, finalDimensions.height);

	// ==================================================================== //

	const bool saveBitmap = false;

	// ==================================================================== //

	Sleep(3000);

	// ==================================================================== //

	Stopwatch stopwatch1;
	Stopwatch stopwatch2("Serial Write Data");

	int i = 0;

	Dimensions dimensionsOf_hCaptureBitmap;

	bool runOnce = true;
	
	while(running) {
		std::cout << "i: " << i << std::endl;
		stopwatch1.start();

		CaptureScreen(i++, point, finalDimensions, saveBitmap, hDesktopDC, hCaptureDC, hCaptureBitmap, hOldBmp);
		if(runOnce) {
			getBitmapDimensions(dimensionsOf_hCaptureBitmap, hCaptureDC, hCaptureBitmap, hOldBmp);
			runOnce = false;
		}

		solveImage(characterSet, characterSet2, serialData, hDesktopDC, hCaptureDC);

		stopwatch2.start();
		serial.WriteData(stringBuilder(serialData.data1, serialData.data2, serialData.data3));
		stopwatch2.stop();

		stopwatch1.stop(false);

		std::cout << "Loop Single Iteration Clock: " << stopwatch1.getDuration() << std::endl;
		std::cout << "------------------------------------------------------" << std::endl;
	}

	// ==================================================================== //

	std::cout << "Loop Exited..." << std::endl;

	// ==================================================================== //

	ReleaseDC(hDesktopWnd,hDesktopDC); // USE A FUNCTION TO CLEANUP. THE FUNCTION FUNCTIONS LIKE A MODULE AND MAKES THE CODE MORE READABLE
	DeleteDC(hCaptureDC);
	DeleteObject(hCaptureBitmap);

	// ==================================================================== //

	return 0;
}