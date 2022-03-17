/*!
 * @file setMoveMode.ino
 * @brief Set the movement mode of the display, such as left, right, hold, up, down, flash display, etc.
 * @n The character string "DFRobot" is displayed, and the mobile display mode is switched once every 5s
 * @n The string "DFRobot" sequentially performs cyclic display such as left, right, hold, up, down, and flash.
 * @note 显示信息不同超过屏的分辨率，否则只能实现左移、右移等操作，无法实现停止、下移，上移、闪烁等功能
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
//DFRobot_SerialScreen771 screen(Serial1); // 7x71  Flexible RGB LED MatrixSerial Serial1(2, 3); //RX, TX

void setup() {
    /*Initialize communication interface (Serial1) and debug interface (Serial)*/
    Serial.begin(115200);
    Serial1.begin(19200);
    screen.begin();

    delay(5);
    /*Display string "DFRobot"*/
    //显示信息不同超过屏的分辨率，否则只能实现左移、右移等操作，无法实现停止、下移，上移、闪烁等功能
    screen.displayMessage("12356");
    screen.setMoveSpeed(screen.eSpeedLevel_8);
    /*Set the move mode to hold*/
    /*eMoveMode_t: eMoveLeft = left
                   eMoveRight = right
                   eMoveHold = hold
                   eMoveDown = down
                   eMoveUp = up
                   eMoveFlash = flash
    */
    screen.setMoveMode(screen.eMoveHold);
}

void loop() {
    /*Switch a mobile display mode every 5s*/
    DFRobot_Flexible_STC8F::eMoveMode_t buf[]= {screen.eMoveLeft,screen.eMoveRight,screen.eMoveHold,screen.eMoveDown,screen.eMoveUp,screen.eMoveFlash};
    for(int i = 0; i < sizeof(buf)/sizeof(DFRobot_Flexible_STC8F::eMoveMode_t); i++){
        screen.setMoveMode(buf[i]);
        delay(5000);
    }
}