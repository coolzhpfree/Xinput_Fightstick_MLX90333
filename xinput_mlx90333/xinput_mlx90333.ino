/*
 *  Project     Arduino XInput Library
 *  @author     David Madison
 *  @link       github.com/dmadison/ArduinoXInput
 *  @license    MIT - Copyright (c) 2019 David Madison
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *  Example:      GamepadPins
 *  Description:  Uses all of the available pin inputs to build a 'complete'
 *                Xbox gamepad, with both analog joysticks, both triggers,
 *                and all of the main buttons.
 *
 *                * Joysticks should be your typical 10k dual potentiometers.
 *                * Triggers can be either analog (pots) or digital (buttons).
 *                  Set the 'TriggerButtons' variable to change between the two.
 *                * Buttons use the internal pull-ups and should be connected
 *                  directly to ground.
 *
 *                These pins are designed around the Leonardo's layout. You
 *                may need to change the pin numbers if you're using a
 *                different board type
 *
 */
#include <FastLED.h>

#define LED_PIN     15
#define NUM_LEDS    1
#define BRIGHTNESS  127
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100
//int AnalogModeSwitch;
int AnalogModeSwitch_prev;
int AnalogModeSwitch_now;

#include <XInput.h>
//Analog stick smoothing (value int)
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

//LStick switch between Dpad and analog mode (value int)
int Lstickmode = 1; // 0 is dpad only, 1 is analog only

// Setup
const boolean UseLeftJoystick   = true;  // set to true to enable left joystick
const boolean InvertLeftYAxis   = false;  // set to true to use inverted left joy Y

const boolean UseRightJoystick  = false;  // set to true to enable right joystick
const boolean InvertRightYAxis  = false;  // set to true to use inverted right joy Y

const boolean UseTriggerButtons = true;   // set to false if using analog triggers

const int ADC_Max = 1023;  // 10 bit

// Joystick Pins
const int Pin_LeftJoyX  = A0;
const int Pin_LeftJoyY  = A1;
const int Pin_RightJoyX = A2;
const int Pin_RightJoyY = A3;

// Trigger Pins
const int Pin_TriggerL = A4;
const int Pin_TriggerR = A5;

// Button Pins
const int Pin_ButtonA = 0;
const int Pin_ButtonB = 1;
const int Pin_ButtonX = 2;
const int Pin_ButtonY = 3;

const int Pin_ButtonLB = 4;
const int Pin_ButtonRB = 5;

const int Pin_ButtonBack  = 6;
const int Pin_ButtonStart = 7;

const int Pin_ButtonL3 = 8;
const int Pin_ButtonR3 = 9;

// Directional Pad Pins
const int Pin_DpadUp    = 10;
const int Pin_DpadDown  = 11;
const int Pin_DpadLeft  = 12;
const int Pin_DpadRight = 13;

// stick mode
const int PIN_AnalogModeSwitch = 14;
const int LED = 15;


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
  
	// If using buttons for the triggers, use internal pull-up resistors
	if (UseTriggerButtons == true) {
		pinMode(Pin_TriggerL, INPUT_PULLUP);
		pinMode(Pin_TriggerR, INPUT_PULLUP);
	}
	// If using potentiometers for the triggers, set range
	else {
		XInput.setTriggerRange(0, ADC_Max);
	}

	// Set buttons as inputs, using internal pull-up resistors
	pinMode(Pin_ButtonA, INPUT_PULLUP);
	pinMode(Pin_ButtonB, INPUT_PULLUP);
	pinMode(Pin_ButtonX, INPUT_PULLUP);
	pinMode(Pin_ButtonY, INPUT_PULLUP);

	pinMode(Pin_ButtonLB, INPUT_PULLUP);
	pinMode(Pin_ButtonRB, INPUT_PULLUP);

	pinMode(Pin_ButtonBack, INPUT_PULLUP);
	pinMode(Pin_ButtonStart, INPUT_PULLUP);

	pinMode(Pin_ButtonL3, INPUT_PULLUP);
	pinMode(Pin_ButtonR3, INPUT_PULLUP);

	pinMode(Pin_DpadUp, INPUT_PULLUP);
	pinMode(Pin_DpadDown, INPUT_PULLUP);
	pinMode(Pin_DpadLeft, INPUT_PULLUP);
	pinMode(Pin_DpadRight, INPUT_PULLUP);

  pinMode(PIN_AnalogModeSwitch, INPUT_PULLUP);
  
	XInput.setJoystickRange(300, 700);  // Rescaling for sensitivity
	XInput.setAutoSend(false);  // Wait for all controls before sending

	XInput.begin();

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
}

void loop() {
//Analog stick smoothing (calc)
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

AnalogModeSwitch_now=digitalRead(PIN_AnalogModeSwitch);
  if(AnalogModeSwitch_prev==0 && AnalogModeSwitch_now==1){
  if (Lstickmode==0){
    Lstickmode=1;
  }
  else{
    Lstickmode=0;
  }
}
AnalogModeSwitch_prev=AnalogModeSwitch_now;

/*讀值校正
Serial.print("Xaverage=");
Serial.print(Xaverage);
Serial.print("\t");
Serial.print("Yaverage=");
Serial.print(Yaverage);
Serial.print("\t");

Serial.println("\t");
*/
  
	// Read pin values and store in variables
	// (Note the "!" to invert the state, because LOW = pressed)
	boolean buttonA = !digitalRead(Pin_ButtonA);
	boolean buttonB = !digitalRead(Pin_ButtonB);
	boolean buttonX = !digitalRead(Pin_ButtonX);
	boolean buttonY = !digitalRead(Pin_ButtonY);

	boolean buttonLB = !digitalRead(Pin_ButtonLB);
	boolean buttonRB = !digitalRead(Pin_ButtonRB);

	boolean buttonBack  = !digitalRead(Pin_ButtonBack);
	boolean buttonStart = !digitalRead(Pin_ButtonStart);

	boolean buttonL3 = !digitalRead(Pin_ButtonL3);
	boolean buttonR3 = !digitalRead(Pin_ButtonR3);

	boolean dpadUp    = !digitalRead(Pin_DpadUp);
	boolean dpadDown  = !digitalRead(Pin_DpadDown);
	boolean dpadLeft  = !digitalRead(Pin_DpadLeft);
	boolean dpadRight = !digitalRead(Pin_DpadRight);

	// Set XInput buttons
	XInput.setButton(BUTTON_A, buttonA);
	XInput.setButton(BUTTON_B, buttonB);
	XInput.setButton(BUTTON_X, buttonX);
	XInput.setButton(BUTTON_Y, buttonY);

	XInput.setButton(BUTTON_LB, buttonLB);
	XInput.setButton(BUTTON_RB, buttonRB);

	XInput.setButton(BUTTON_BACK, buttonBack);
	XInput.setButton(BUTTON_START, buttonStart);

	XInput.setButton(BUTTON_L3, buttonL3);
	XInput.setButton(BUTTON_R3, buttonR3);

	// Set XInput DPAD values
	if (Lstickmode == 0) {
  XInput.setJoystick(JOY_LEFT, 500, 500);
	XInput.setDpad(dpadUp, dpadDown, dpadLeft, dpadRight);
  
    for(int i = 0; i < NUM_LEDS; i++ )
      {
       leds[i].setRGB(127,0,0); // Set Color HERE!!!
      }
    FastLED.show();
	}

	// Set XInput trigger values
	if (UseTriggerButtons == true) {
		// Read trigger buttons
		boolean triggerLeft  = !digitalRead(Pin_TriggerL);
		boolean triggerRight = !digitalRead(Pin_TriggerR);

		// Set the triggers as if they were buttons
		XInput.setButton(TRIGGER_LEFT, triggerLeft);
		XInput.setButton(TRIGGER_RIGHT, triggerRight);
	}
	else {
		// Read trigger potentiometer values
		int triggerLeft  = analogRead(Pin_TriggerL);
		int triggerRight = analogRead(Pin_TriggerR);

		// Set the trigger values as analog
		XInput.setTrigger(TRIGGER_LEFT, triggerLeft);
		XInput.setTrigger(TRIGGER_RIGHT, triggerRight);
	}


	//Deadzone adjust
	if (Xaverage < 575 and Xaverage > 425) {
		Xaverage = 500;
	}
	if (Yaverage < 575 and Yaverage > 425) {
		Yaverage = 500;
	}

	// Set left joystick
	if (UseLeftJoystick == true and Lstickmode == 1) {
    XInput.setDpad(LOW, LOW, LOW, LOW);
		if (InvertLeftYAxis == true) {
			Yaverage = ADC_Max - Yaverage;
		}

//		XInput.setJoystick(JOY_LEFT, ADC_Max, ADC_Max);
		XInput.setJoystick(JOY_LEFT, Xaverage, Yaverage);

    for(int i = 0; i < NUM_LEDS; i++ )
      {
       leds[i].setRGB(0,0,127); // Set Color HERE!!!
      }
    FastLED.show();
	}

	// Set right joystick 
	if (UseRightJoystick == true) {
		//int rightJoyX = Rxaverage;
		//int rightJoyY = Ryaverage;

		if (InvertRightYAxis == true) {
			Ryaverage = ADC_Max - Ryaverage;
		}

//		XInput.setJoystick(JOY_RIGHT, rightJoyX, rightJoyY);
		XInput.setJoystick(JOY_RIGHT, Rxaverage, Ryaverage);
	}

	// Send control data to the computer
	XInput.send();
}
