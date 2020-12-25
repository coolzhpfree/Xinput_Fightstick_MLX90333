//from https://knuckleslee.blogspot.com/2017/11/twin-stick-ex.html/

#include <Joystick.h> //模擬0鍵遊戲手把,4個類比軸
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD, 0, 0,
true, true, false, true, true, false, false, false, false, false, false);

//數值平滑化
const int axisReadings = 10; //陣列取樣數
int Xreadings[axisReadings]; // 從腳位讀取數值
int XreadIndex = 0; // 目前讀值
int Xtotal = 0; // the running total
int Xaverage = 0; // 平均值

int Yreadings[axisReadings]; // 從腳位讀取數值
int YreadIndex = 0; // 目前讀值
int Ytotal = 0; // the running total
int Yaverage = 0; // 平均值

int Rxreadings[axisReadings]; // 從腳位讀取數值
int RxreadIndex = 0; // 目前讀值
int Rxtotal = 0; // the running total
int Rxaverage = 0; // 平均值

int Ryreadings[axisReadings]; // 從腳位讀取數值
int RyreadIndex = 0; // 目前讀值
int Rytotal = 0; // the running total
int Ryaverage = 0; // 平均值

void setup() {
Serial.begin(9600);
// initialize all the readings to 0:
for (int thisXReading = 0; thisXReading < axisReadings ; thisXReading++) {
Xreadings[thisXReading] = 0; }
for (int thisYReading = 0; thisYReading < axisReadings ; thisYReading ++) {
Yreadings[thisYReading ] = 0; }
for (int thisRxReading = 0; thisRxReading < axisReadings ; thisRxReading ++) {
Rxreadings[thisRxReading ] = 0; }
for (int thisRyReading = 0; thisRyReading < axisReadings ; thisRyReading ++) {
Ryreadings[thisRyReading ] = 0; }

pinMode(A0, INPUT);
pinMode(A1, INPUT);
pinMode(A2, INPUT);
pinMode(A3, INPUT);

Joystick.begin();
Joystick.setXAxisRange(400,600);
Joystick.setYAxisRange(580,390);
Joystick.setRxAxisRange(650,400);
Joystick.setRyAxisRange(600,370);
}

void loop() {

//
Xtotal = Xtotal - Xreadings[XreadIndex]; // subtract the last reading:
Xreadings[XreadIndex] = analogRead(A0); // read from the sensor:
Xtotal = Xtotal + Xreadings[XreadIndex]; // add the reading to the Xtotal:
XreadIndex = XreadIndex + 1; // advance to the next position in the array:
if(XreadIndex >= axisReadings ) // if we're at the end of the array...
{
XreadIndex = 0; // ...wrap around to the beginning:
}
Xaverage = Xtotal / axisReadings ; // calculate the Xaverage:
//
Ytotal = Ytotal - Yreadings[YreadIndex]; // subtract the last reading:
Yreadings[YreadIndex] = analogRead(A1); // read from the sensor:
Ytotal = Ytotal + Yreadings[YreadIndex]; // add the reading to the Ytotal:
YreadIndex = YreadIndex + 1; // advance to the next position in the array:
if(YreadIndex >= axisReadings ) // if we're at the end of the array...
{
YreadIndex = 0; // ...wrap around to the beginning:
}
Yaverage = Ytotal / axisReadings ; // calculate the Yaverage:
//
Rxtotal = Rxtotal - Rxreadings[RxreadIndex]; // subtract the last reading:
Rxreadings[RxreadIndex] = analogRead(A2); // read from the sensor:
Rxtotal = Rxtotal + Rxreadings[RxreadIndex]; // add the reading to the Rxtotal:
RxreadIndex = RxreadIndex + 1; // advance to the next position in the array:
if(RxreadIndex >= axisReadings ) // if we're at the end of the array...
{
RxreadIndex = 0; // ...wrap around to the beginning:
}
Rxaverage = Rxtotal / axisReadings ; // calculate the Rxaverage:
//
Rytotal = Rytotal - Ryreadings[RyreadIndex]; // subtract the last reading:
Ryreadings[RyreadIndex] = analogRead(A3); // read from the sensor:
Rytotal = Rytotal + Ryreadings[RyreadIndex]; // add the reading to the Rytotal:
RyreadIndex = RyreadIndex + 1; // advance to the next position in the array:
if(RyreadIndex >= axisReadings ) // if we're at the end of the array...
{
RyreadIndex = 0; // ...wrap around to the beginning:
}
Ryaverage = Rytotal / axisReadings ; // calculate the Ryaverage:
//

Joystick.setXAxis(Xaverage);
Joystick.setYAxis(Yaverage);
Joystick.setRxAxis(Rxaverage);
Joystick.setRyAxis(Ryaverage);

//讀值校正
Serial.print("A0=");
Serial.print(analogRead(A0));
Serial.print("\t");
Serial.print("Xaverage=");
Serial.print(Xaverage);
Serial.print("\t");
Serial.print("A1=");
Serial.print(analogRead(A1));
Serial.print("\t");
Serial.print("Yaverage=");
Serial.print(Yaverage);
Serial.print("\t");
Serial.print("A2=");
Serial.print(analogRead(A2));
Serial.print("\t");
Serial.print("Rxaverage=");
Serial.print(Rxaverage);
Serial.print("\t");
Serial.print("A3=");
Serial.print(analogRead(A3));
Serial.print("\t");
Serial.print("Ryaverage=");
Serial.print(Ryaverage);
Serial.print("\t");

Serial.println("\t");
}
