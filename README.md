# Xinput_Fightstick_MLX90333
Xinput arcade controller using an arduino with MLX90333 hall effect sensors for analog stick inputs

# Reference
Credits to Knuckleslee
https://knuckleslee.blogspot.com/2017/11/twin-stick-ex.html

Arduino XInput Library
https://github.com/dmadison/ArduinoXInput

# To Do
- Add a extra button to toggle LStick between analog mode and Dpad mode (see "LStickmode" in the ino file) 
  * use ISCP (eg MOSI / MISO / SCK) pin for button?
    * Micro pin 14 is MISO, 15 is SCKL and 16 is MOSI
  * OR button conbination (eg start + back))
- Make the stick deadzone as a circle (see "//Deadzone Adjust" in the ino) (coders plz help me) 
- RGB leds (WS2812) 
    * Micro pin 14 is MISO, 15 is SCKL and 16 is MOSI
- Make a 3d printed mount to fix the sensor on to a sanwa stick (I have a clone) (3d modellers plz help me) 

https://forum.arduino.cc/index.php?topic=466897.0
