/*!
 * @file setDisplayColor.ino
 * @brief Set the color of the display background and font, a total of 8 colors of red, yellow, green, cyan, blue, purple, white, black.
 * @n The string "DFRobot" is displayed, the background is black, and the font color changes in red, yellow, green, cyan, blue, purple, and white colors every 5 seconds.
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0.0
 * @date  2022-03-17
 * @url https://github.com/DFRobot/DFRobot_Flexible_STC8F
 */

#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include "DFRobot_Flexible_STC8F.h"

#ifdef ARDUINO_AVR_UNO
SoftwareSerial Serial1(2, 3); //RX, TX
#endif


DFRobot_SerialScreen1248 screen(Serial1);  //12x48  Flexible RGB LED Matrix
//DFRobot_SerialScreen771 screen(Serial1); // 7x71  Flexible RGB LED Matrix

void setup() {
    /*Initialize communication interface (Serial1) and debug interface (Serial)*/
    Serial.begin(115200);
    Serial1.begin(19200);
    screen.begin();

    delay(5);
}

void loop() {
    /*Switch one font color every 5s*/
    DFRobot_Flexible_STC8F::eColorMode_t backgroud,font;
    backgroud = screen.eColorBlack;
    DFRobot_Flexible_STC8F::eColorMode_t buf[]= {screen.eColorRed, screen.eColorYellow, screen.eColorGreen, screen.eColorCyan, screen.eColorBlue, screen.eColorPurple, screen.eColorWhite};
    for(int i = 0; i < sizeof(buf)/sizeof(DFRobot_Flexible_STC8F::eColorMode_t); i++){
        font = buf[i];
        /*Set the string to display the background and font color*/
        screen.displayMessage("DFRobot", font, backgroud);
        delay(5000);
    }
}
