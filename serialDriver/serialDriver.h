#ifndef SERIALDRIVER_H
#define SERIALDRIVER_H

#include <string>
#include <windows.h>

using namespace std;

namespace customSerial {
    class Serial {
        private:
            HANDLE hPort;

        public:
            Serial(string port);
            ~Serial();

            string ReadData();
            void WriteData(string data);
    };
}

std::string stringBuilder(int data1, int data2, int data3);

#endif