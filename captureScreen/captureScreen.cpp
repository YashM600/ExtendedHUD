#include <iostream>
#include <chrono>
#include <windows.h>

#include "captureScreen.h"
#include "../saveBitmapImage/saveBitmapImage.h"

int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

void CaptureScreen(int i, utils::Point point, Dimensions finalDimensions, bool saveToFile, HDC hDesktopDC, HDC hCaptureDC, HBITMAP hCaptureBitmap, HBITMAP hOldBmp) {

   Stopwatch stopwatch("Screen Capture");
   stopwatch.start();

   hOldBmp = (HBITMAP)SelectObject(hCaptureDC, hCaptureBitmap);

   BitBlt(hCaptureDC, 0, 0, finalDimensions.width, finalDimensions.height, hDesktopDC, nScreenWidth - point.x, nScreenHeight - point.y, SRCCOPY | CAPTUREBLT);

   if(saveToFile) {
      saveBitmap(hDesktopDC, hCaptureDC, 0, 0, finalDimensions.width, finalDimensions.height, i);
   }

   SelectObject(hCaptureDC, hOldBmp);

   stopwatch.stop();
}