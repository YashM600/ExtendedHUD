#ifndef CAPTURESCREEN_H
#define CAPTURESCREEN_H

#include <windows.h>
#include "../utils/utils.h"

void CaptureScreen(
    int i, utils::Point point, Dimensions finalDimensions, bool saveToFile, HDC hDesktopDC, HDC hCaptureDC, HBITMAP hCaptureBitmap, HBITMAP hOldBmp);

#endif