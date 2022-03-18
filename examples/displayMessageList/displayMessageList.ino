/*!
 * @file displayMessageList.ino
 * @brief Initialize 11 information lists MA->MK and directly print the list of "DFRobot" in the information list.
 * @n Display a list of previously stored values of "DFRobot"
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

const char* MA = "DFRobot";                       // eBanner_A  Set and display list A: 不指定字体颜色和背景的字符串"DFRobot"
const char* MB = "<CRY>DFRobot";                  // eBanner_B  Set and display list B: 黄底红字的字符串"DFRobot"
const char* MC = "Hi!";                           // eBanner_C  Set and display list C: 不指定字体颜色和背景的字符串"Hi!"
const char* MD = "<CYR>Hello!";                   // eBanner_D  Set and display list D: 红底黄字的字符串"Hello!"
const char* ME = "World!";                        // eBanner_E  Set and display list E: 不指定字体颜色和背景的字符串"World!"
const char* MF = "<CRW>H";                        // eBanner_F  Set and display list F:  白底红字H
const char* MG = "<CRG>h";                        // eBanner_G  Set and display list G:  绿底红字h
const char* MH = "<CRW>H<CRG>h";                  // eBanner_H  Set and display list H:  白底红字H + 绿底红字h
const char* MI = "<CGG> <CRW>H<CGG> <CRW>h<CGG> ";// eBanner_I  Set and display list I:  绿色空格 + 白底红字H + 绿色空格 + 白底红字h + 绿色空格
const char* MJ = "<CWW>     <CRW>Ii<CWW>    .";   // eBanner_J  Set and display list J:  全屏显示白屏红字的"Ii"，格式为：5个空格 + Ii + 4个空格 + 小数点.
const char* MK = "<CWW>    <CRW>Hh<CWW>    ";     // eBanner_K  Set and display list K:  全屏显示白屏红字的"Hh"，格式为：4个空格 + Hh + 4个空格


void setup() {
    // Initialize communication interface (Serial1) and debug interface (Serial)
    Serial.begin(115200);
    Serial1.begin(19200);
    screen.begin();

    // Send 8 information lists to the serial screen
    screen.setMessageList(screen.eBanner_A, MA);
    screen.setMessageList(screen.eBanner_B, MB);
    screen.setMessageList(screen.eBanner_C, MC);
    screen.setMessageList(screen.eBanner_D, MD);
    screen.setMessageList(screen.eBanner_E, ME);
    screen.setMessageList(screen.eBanner_F, MF);
    screen.setMessageList(screen.eBanner_G, MG);
    screen.setMessageList(screen.eBanner_H, MH);
    screen.setMessageList(screen.eBanner_I, MI);
    screen.setMessageList(screen.eBanner_J, MJ);
    screen.setMessageList(screen.eBanner_K, MK);

    // Prints a list of "DFRobot" in the message list, and the data of the M0 message list
    screen.displayBanner(screen.eBanner_A);
    // Print data of MK information list
    //screen.displayBanner(screen.eBanner_K);
    // Print data for MA and MK information lists
    //screen.displayBanner(screen.eBanner_A | screen.eBanner_K);
    // Display all information lists for MA~MK
    //screen.displayBanner(screen.eBanner_A|screen.eBanner_B|screen.eBanner_C|screen.eBanner_D|screen.eBanner_E|screen.eBanner_F|screen.eBanner_G|screen.eBanner_H|screen.eBanner_I|screen.eBanner_J|screen.eBanner_K);
    //or
    //screen.displayBanner(screen.eBanner_ALL);
}

void loop() {


}
