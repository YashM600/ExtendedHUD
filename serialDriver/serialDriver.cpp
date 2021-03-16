#include <iostream>
#include <windows.h>
#include "serialDriver.h"

customSerial::Serial::Serial(std::string port) {
    DCB dcb;

    this->hPort = CreateFileA(
        port.c_str(),
        GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if(!GetCommState(hPort, &dcb)) {
        std::cout << "GetCommState Failed" << std::endl;
    } else {
        dcb.BaudRate = CBR_115200;
        dcb.ByteSize = 8;
        dcb.Parity = NOPARITY;
        dcb.StopBits = ONESTOPBIT;

        if(!SetCommState(hPort, &dcb)) {
            std::cout << "SetCommState Failed" << std::endl;
        } else {
            Sleep(2000);
        }
    }
}

customSerial::Serial::~Serial() {
    CloseHandle(hPort);
    std::cout << "Serial Port Closed Successfully" << std::endl;
}

void customSerial::Serial::WriteData(std::string data) {
    DWORD byteswritten;

    if(!WriteFile(hPort, (void *)(data.c_str()), data.length(), &byteswritten, NULL)) {
        std::cout << "Write Data Failed" << std::endl;
    }
}

std::string customSerial::Serial::ReadData() {
    return "";
}

std::string stringBuilder(int data1, int data2, int data3) {
    std::string delimiter = ",";
    return to_string(data1) + delimiter + to_string(data2) + delimiter + to_string(data3) + "\n";
}