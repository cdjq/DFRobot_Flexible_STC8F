/*!
 * @file setScreenColor.ino
 * @brief Full screen lighting, full screen display red, yellow, green, blue, blue, purple, white and other colors.
 * @n Light up the full screen, switch the color every 5s, and cycle
 * @note 如果将显示亮度调大，需要外部供电，以防止因供电不足，导致显示颜色效果和预期有差异
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0.0
 * @date  2022-03-17
 * @url https://github.com/DFRobot/DFRobot_Flexible_STC8F
 */

#include <Arduino.h>
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
    /*Set screen color*/
    /*eColorMode_t: eColorRed = red
                    eColorYellow = yellow
                    eColorGreen = green
                    eColorCyan = cyan
                    eColorBlue = blue
                    eColorPurple = purple
                    eColorWhite = white
                    eColorBlack = black
    */
    screen.setFullScreenColor(screen.eColorBlack);
    screen.setBrightness(screen.eBrightLevel_1);
}

void loop() {
    /*Switch one screen color every 5s*/
    DFRobot_SerialScreen771::eColorMode_t buf[]= {screen.eColorRed, screen.eColorYellow, screen.eColorGreen, screen.eColorCyan, screen.eColorBlue, screen.eColorPurple, screen.eColorWhite,screen.eColorBlack};
    for(int i=0; i < sizeof(buf)/sizeof(DFRobot_SerialScreen771::eColorMode_t); i++){
        screen.setFullScreenColor(buf[i]);
        delay(5000);
    }
}