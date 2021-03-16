//#define DEBUG

#include <TM1637Display.h>

#define DISP1_CLKPIN 2
#define DISP1_DATAPIN 3
#define DISP1_LATCHPIN 4

#define DISP2_CLKPIN 5
#define DISP2_DATAPIN 6
#define DISP2_LATCHPIN 7

#define CLK 11
#define DIO 12

int ledArray[17][16] = {{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
                       { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
                       { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
                       { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
                       { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
                       { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
                       { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
                       { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }};

////                         { a, b, c, d, e, f, g, h }
int sevenSegDisp[10][16] = {{ 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // 0
                             { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // 1
                             { 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // 2
                             { 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // 3
                             { 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // 4
                             { 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // 5
                             { 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // 6
                             { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // 7
                             { 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },  // 8
                             { 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }}; // 9

void shiftBits(const int clkPin, const int dataPin, const int data, int dataArray[][16], const int numOfBits);
void splitString();
uint16_t unpackBinaryToUInt16(byte *bIncomingData, int i);

class Display {
  int clkPin;
  int dataPin;
  int latchPin;

  int numOfBits;

  public:

  Display(const int clkPin, const int dataPin, const int latchPin, const int numOfBits) {
    this->clkPin = clkPin;
    this->dataPin = dataPin;
    this->latchPin = latchPin;

    this->numOfBits = numOfBits;

    pinMode(clkPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
  }

  void setDisplay(const int data, int dataArray[][16]) {
    digitalWrite(latchPin, LOW);
    shiftBits(clkPin, dataPin, data, dataArray, numOfBits);
    digitalWrite(latchPin, HIGH);
  }
};

Display display1(DISP1_CLKPIN, DISP1_DATAPIN, DISP1_LATCHPIN, 16);
Display display2(DISP2_CLKPIN, DISP2_DATAPIN, DISP2_LATCHPIN, 8);

TM1637Display display(CLK, DIO);

void setup() {
  Serial.begin(115200);

  display.setBrightness(0x0f);
}

bool preloader = true;
String inStr = "";
bool stringComplete = false;
//int data1 = 0, data2 = 0, data3 = 0;

uint16_t data1 = 0, data2 = 0, data3 = 0;

void loop() {
  while(Serial.available()) {
    /*int inComingByte = Serial.read();

    preloader = false;

    // Convert the incoming byte to a char and add it to the string:
    inStr += (char)inComingByte; 

    if (inComingByte == '\n') {
      stringComplete = true;
      Serial.println(inStr);
    }
    */

    byte bIncomingData[64];
    Serial.readBytes(bIncomingData, 64);
    
    Serial.write(bIncomingData, 64);
    
    data1 = unpackBinaryToUInt16(bIncomingData, 0);
    data2 = unpackBinaryToUInt16(bIncomingData, 1);
    data3 = unpackBinaryToUInt16(bIncomingData, 2);

    Serial.println(data1);
    Serial.println(data2);
    Serial.println(data3);

    preloader = false;
  }
//  if(stringComplete) {

    #ifdef DEBUG
    Serial.print("Received String: " + inStr);
    #endif

//    splitString();

    display1.setDisplay(data1, ledArray);
    display2.setDisplay(data2, sevenSegDisp);
    display.showNumberDec(data3, false);

//    stringComplete = false;
//  }

  #ifdef DEBUG
  Serial.println("Parsed Data Values: " + String(data1) + "," + String(data2) + "," + String(data3));
  #endif

    if(preloader) {
      display1.setDisplay(data1, ledArray);
      display2.setDisplay(data2, sevenSegDisp);
      display.showNumberDec(data3, false);
      data1++;
      data2++;
      data3++;
      delay(90);
      if(data1 > 16) data1 = 0;
      if(data2 > 9) data2 = 0;
      if(data3 > 300) data3 = 0;
    }

}

void shiftBits(const int clkPin, const int dataPin, const int data, int dataArray[][16], const int numOfBits) {
  digitalWrite(dataPin, 0);
  digitalWrite(clkPin, 0);

  for(int i = (numOfBits-1); i >=0; i--) {
    digitalWrite(clkPin, 0);

    digitalWrite(dataPin, dataArray[data][i]);
    digitalWrite(clkPin, 1);

    digitalWrite(dataPin, 0);
  }

  digitalWrite(clkPin, 0);
}

/*void serialEvent() {
  while(Serial.available()) {
    int inComingByte = Serial.read();

    preloader = false;

    // Convert the incoming byte to a char and add it to the string:
    inStr += (char)inComingByte; 

    if (inComingByte == '\n') {
      stringComplete = true;
    }
    Serial.println(inStr);
  }
}*/

//void splitString() {
//  String delimiter = ",";   // Delimiter Definition
//      String subString = "";    // Temporary Substring
//      subString = inStr;
//
//      size_t strPos = 0;        // String position index
//      String token;             // Token variable
//      int i = 0;                // Array iteration Variable
//
//      String tokens[3];
//
//      while(( strPos = subString.indexOf(delimiter) ) != -1) {
//        token = subString.substring(0, strPos);
//
//        tokens[i++] = token;
//
//        subString.remove(0, strPos + delimiter.length());
//      }
//
//      tokens[i] = subString;
//
//      data1 = tokens[0].toInt();
//      data2 = tokens[1].toInt();
//      data3 = tokens[2].toInt();
//
//      inStr = "";
//}

uint16_t unpackBinaryToUInt16(byte *bIncomingData, int i) {
      uint16_t unpackedValue = 0;
      unpackedValue = bIncomingData[i] << 8 | bIncomingData[i+1] << 0;
      return unpackedValue;
}
