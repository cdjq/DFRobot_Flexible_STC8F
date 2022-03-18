/*!
 * @file DFRobot_Flexible_STC8F.cpp
 * @brief Define the basic structure of class DFRobot_Flexible_STC8F
 * @n 这是一个柔性RGB显示库，它目前支持以下两种分辨率的显示屏
 * @n 1.  7x71  Flexible RGB LED Matrix
 * @n 2.  12x48 Flexible RGB LED Matrix
 * @n 此库具有以下功能：
 * @n 1. 显示字符；
 * @n 2. 预先存储11个信息列表，设置显示一个或多个信息列表；
 * @n 3. 8级别显示亮度设置；
 * @n 4. 可设置字符的前景色和背景色；
 * @n 5. 全屏点亮屏幕，全屏点亮，支持红色、绿色、青色、蓝色、紫色、白色、黑色；
 * @n 6. 8级移动速度设置；
 * @n 7. 6种移动动态显示设置：左移、右移、保持停止、上移、下移、闪烁
 * @n 8. 支持软串口通讯，波特率19200
 * @note 在设置6种动态显示设置时，如果设置为除左移或右移以外的设置，字符不能超过屏幕的最大宽度分辨率，否则会一直显示左移或右移
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0.0
 * @date  2022-03-17
 * @url https://github.com/DFRobot/DFRobot_Flexible_STC8F
 */

#include "DFRobot_Flexible_STC8F.h"

#define TYPE_INFROMTION   0X01   //< type information
#define TYPE_BRIGHT       0X02   //< type bright
#define TYPE_SPEED        0x03   //< type speed
#define TYPE_LIST         0X04   //< type list


DFRobot_Flexible_STC8F::DFRobot_Flexible_STC8F(Stream &s_, uint16_t width)
  :_s(&s_),_width(width), _order(0){
      _moveMode    = eMoveHold;
      _backgroud   = eColorBlack;
      _font        = eColorBlue;
      _color      = false;
      _brightLevel = eBrightLevel_5;
      _speedLevel  = eSpeedLevel_1;
    
      memcpy(_M, "ABCDEFGHIJK", BANNER);
}


bool DFRobot_Flexible_STC8F::begin(){
    if(_s == NULL){
        return false;
    }
    return true;
}

void DFRobot_Flexible_STC8F::setMoveMode(eMoveMode_t m_){

    if((m_ < eMoveLeft) | (m_ == 0x34) | (m_ > eMoveFlash)){
        return false;
    }
    _moveMode = m_;
}


void DFRobot_Flexible_STC8F::displayMessage(const char *message_){
    if(!message_) return;
    _color = false;
    pPacketHeader_t header = packed(TYPE_INFROMTION, message_, strlen(message_));
    if(header == NULL){
        DBG("Memory ERROR!");
        return;
    }
    /*DBG("payload:");
    for(int i = 0; i< header->length.lenL; i++){
        DBG(header->payload[i],HEX);
        DBG(i);
    }*/
    sendPacket(header);
    free(header);
}

void DFRobot_Flexible_STC8F::displayMessage(const char *message_, eColorMode_t font, eColorMode_t shading){
    if(!message_) return;
    _backgroud = shading;
    _font = font;
    _color = true;

    pPacketHeader_t header = packed(TYPE_INFROMTION, message_, strlen(message_));
    if(header == NULL){
        DBG("Memory ERROR!");
        return;
    }
    sendPacket(header);
    free(header);
    _color = false;
}

bool DFRobot_Flexible_STC8F::setBrightness(eBrightLevel_t b_){
    char mess[1];
    mess[0] = b_;
    _brightLevel = b_;
    //message[0] = b_;
    pPacketHeader_t header = packed(TYPE_BRIGHT, mess, strlen(mess));
    if(header == NULL){
        DBG("Memory ERROR!");
        return false;
    }
    DBG("payload:");DBG((const char *)header->payload);
    sendPacket(header);
    DBG("payload:");DBG((const char *)header->payload);
    free(header);
    return true;
}

bool DFRobot_Flexible_STC8F::setMoveSpeed(eSpeedLevel_t s_){
    char mess[1];
    mess[0] = s_;
    //message[0] = b_;
    pPacketHeader_t header = packed(TYPE_SPEED, mess, strlen(mess));
    if(header == NULL){
        DBG("Memory ERROR!");
        return false;
    }
    DBG("payload:");DBG((const char *)header->payload);
    sendPacket(header);
    DBG("payload:");DBG((const char *)header->payload);
    free(header);
    return true;
}

bool DFRobot_Flexible_STC8F::setMessageList(eBanner_t banN, const char *message_){
    return setMessageList((uint16_t)banN, message_);
}

bool DFRobot_Flexible_STC8F::setMessageList(uint16_t banN, const char *message_){
    if(strlen(message_) > MESSAGE_SIZE ){
       return false;
    }
    if(!banN) return false;
    banN &= 0x7FF;
    for(int i = 0; i < BANNER; i++){
        if(banN & (1 << i)){
            _order = i;
            pPacketHeader_t header = packed(TYPE_INFROMTION, message_, strlen(message_));
            if(header == NULL){
                DBG("Memory ERROR!");
                return false;
            }
            sendPacket(header);
            free(header);
            delay(5);
        }
    }
    return true;
}

bool DFRobot_Flexible_STC8F::setMessageList(uint16_t banN, const char *message_, eMoveMode_t m_){
   if((m_ < eMoveLeft) | (m_ == 0x34) | (m_ > eMoveFlash)){
       return false;
   }
   _moveMode = m_;
   return setMessageList(banN, message_);
}
bool DFRobot_Flexible_STC8F::setMessageList(eBanner_t banN, const char *message_, eMoveMode_t m_){
    return setMessageList((uint16_t)banN, message_, m_);
}

void DFRobot_Flexible_STC8F::displayBanner(uint16_t banN){
    String message = "";
    if(!banN) return;
    for(int i = 0; i < BANNER; i++){
        if(banN & (1 << i)){
            switch(i){
                case 0:
                       message += 'A';
                       break;
                case 1:
                       message += 'B';
                       break;
                case 2:
                       message += 'C';
                       break;
                case 3:
                       message += 'D';
                       break;
                case 4:
                       message += 'E';
                       break;
                case 5:
                       message += "F";
                       break;
                case 6:
                       message += "G";
                       break;
                case 7:
                       message += "H";
                       break;
                case 8:
                       message += "I";
                       break;
                case 9:
                       message += "J";
                       break;
                case 10:
                       message += "K";
                       break;
                

            }
        }
    }
    DBG(message);
    pPacketHeader_t header = packed(TYPE_LIST, message.c_str(), strlen(message.c_str()));
    if(header == NULL){
        DBG("Memory ERROR!");
        return ;
    }
    sendPacket(header);
    free(header);
    return;
}
void DFRobot_Flexible_STC8F::displayBanner(eBanner_t banN){
    return displayBanner((uint16_t)banN);
}


bool DFRobot_Flexible_STC8F::setFullScreenColor(eColorMode_t color_){
    _backgroud = color_;
    _font = color_;
    _moveMode = eMoveHold;
    _order = 0;
    char s1[MESSAGE_SIZE] = {0x3C, 0x43,(char)(_font&0x00FF), (char)(_backgroud&0x00FF), 0x3E, '\0'};
    int x,y;
    memset(s1 + strlen(s1), '\0', MESSAGE_SIZE - strlen(s1));
    if(_width%(3+1)){
        x = _width/(3+1);
        y = _width - x*(3+1);
    }else{
        x = _width/(3+1) - 1;
        y = _width - x*(3+1);
    }

    memset((s1+strlen(s1)), 0x20, x);
    switch(y){
        case 3:
            s1[strlen(s1)] = 0x20;
            break;
        case 4:
            s1[strlen(s1)] = 0x31;
    }
    pPacketHeader_t header = packed(TYPE_INFROMTION, s1, strlen(s1));
    if(header == NULL){
        DBG("Memory ERROR!");
        return false;
    }
    sendPacket(header);
    free(header);
    return true;
}

DFRobot_Flexible_STC8F::pPacketHeader_t DFRobot_Flexible_STC8F::packed(uint8_t type, const char *pay_, uint16_t len){
    uint16_t length = len;
    if(type == TYPE_INFROMTION){
        len += 4 + length;
        if(_color) len += 10;
    }else{
        len += 1;
    }
    pPacketHeader_t header = (pPacketHeader_t)malloc(sizeof(sPacketHeader_t)+len+2);
    if(header == NULL){
        DBG("malloc chace failed.");
        return NULL;
    }
    header->head.head1 = 0x42; //B
    header->head.head2 = 0x54; //T
    header->head.head3 = 0x30; //0
    header->head.head4 = 0x31; //1
    header->head.head5 = 0x35; //5
    header->type = type;
    header->addr = 0x00; 
    header->length.lenH = (len>>8)&0xFF;
    header->length.lenL = (len)&0x00FF;


    char *dst = (char *)header->payload;
    char *src = (char *)pay_;
    memset(dst, 0, len+1);
    switch(type){
        case TYPE_INFROMTION:
                dst[0] = _M[_order];//_M[_order];
                dst[1] = 0x41;
                dst[2] = _moveMode;
                dst[3] = 0x41;
                if(_color){
                    dst[5] = '<'; //0x3C
                    dst[7] = 0x43; 
                    dst[9] = _font&0x00FF;
                    dst[11] = _backgroud&0x00FF;
                    dst[13] = '>';
                    for(uint16_t j = 0; j < length; j++ ){
                        dst[14 + 2*j+1] = src[j];
                    }
                }else{
                    for(uint16_t j = 0; j < length; j++ ){
                        dst[4 + 2*j+1] = src[j];
                    }
                }
                break;
        case TYPE_BRIGHT:
                dst[0] = 0x55;
                //for(uint16_t j = 1; j < len; j++ ){
                //    dst[j] = src[j-1];
                //}
                memcpy(dst+1, src, length);
                break;;
        case TYPE_SPEED:
                dst[0] = 0xAA;
                memcpy(dst+1, src, length);
                break;;
        case TYPE_LIST:
                dst[0] = 0x30;
                memcpy(dst+1, src, length);
                break;
        default: 
                free(header);
                return NULL;
                break;
    }
    dst[len] = getCs(header);
    dst[len + 1] = '\0';
    return header;
}

uint8_t DFRobot_Flexible_STC8F::getCs(pPacketHeader_t header){
    uint8_t cs = 0;
    uint16_t length = (header->length.lenH<<8)|(header->length.lenL);
    for(uint16_t i = 0; i< length; i++){
        cs ^= header->payload[i];
        DBG("biao: ");DBG(i);DBG(header->payload[i], HEX);
    }
    DBG(cs, HEX);
    return cs;
}

const char * DFRobot_Flexible_STC8F::handleData(uint8_t type, const char *src, uint16_t &len){
    uint16_t length = len;
    DBG(src);
    uint16_t i = 0;
    if(type == TYPE_INFROMTION){
        len += 4 +length;
    }else{
        len += 1;
    }

    pPacketHeader_t header = (pPacketHeader_t)malloc(sizeof(sPacketHeader_t)+len+1);
    if(header == NULL){
        DBG("malloc chace failed.");
        return NULL;
    }
    header->head.head1 = 0x42; //B
    header->head.head2 = 0x54; //T
    header->head.head3 = 0x30; //0
    header->head.head4 = 0x31; //1
    header->head.head5 = 0x35; //5
    header->type = type;
    header->addr = 0x01; 
    header->length.lenH = (len>>8)&0xFF;
    header->length.lenL = (len)&0x00FF;


    char *dst = (char *)header->payload;
    memset(dst, 0, len+1);
    switch(type){
        case TYPE_INFROMTION:
                dst[0]   = 0x4B;//_M[_order];
                dst[1] = 0x00;
                dst[2] = _moveMode;
                dst[3] = 0x00;
                for(uint16_t j = 0; j < length; j++ ){
                    dst[4 + 2*j+1] = src[j];
                }
                break;
        case TYPE_BRIGHT:
                dst[i++] = 0x55;
                for(uint16_t j = i; j < len; j++ ){
                    dst[j] = src[j-i];
                }
                //memcpy();
                memcpy((char *)(dst+i), src, length);
                DBG("");
                break;;
        case TYPE_SPEED:
                dst[i++] = 0xAA;
                memcpy((char *)(dst+i), src, length);
                break;;
        case TYPE_LIST:
                dst[i++] = 0x30;
                memcpy((char *)(dst+i), src, length);
                break;
        default: 
                break;
    }
    dst[len]='\0';
    DBG("dst: ");DBG(dst);
    for(i=0;i<len;i++){
        //DBG(i);
        DBG(dst[i]);
    }
    return dst;
}
void DFRobot_Flexible_STC8F::sendPacket(pPacketHeader_t header){
    DBG("head1:");DBG(header->head.head1,HEX);
    DBG("head2:");DBG(header->head.head2,HEX);
    DBG("head3:");DBG(header->head.head3,HEX);
    DBG("head4:");DBG(header->head.head4,HEX);
    DBG("head5:");DBG(header->head.head5,HEX);
    DBG("type: ");DBG(header->type,HEX);
    DBG("addr: ");DBG(header->addr,HEX);
    DBG("lenH: ");DBG(header->length.lenH,HEX);
    DBG("lenL: ");DBG(header->length.lenL,HEX);
    DBG("payload:");
    uint16_t n = header->length.lenH<<8|header->length.lenL;
    for(uint16_t i=0;i< n+1;i++){
        //DBG(i);
        DBG(header->payload[i],HEX);
    }
    _s->write((uint8_t *)header, n+sizeof(sPacketHeader_t));
    delay(100);
}

DFRobot_SerialScreen771::DFRobot_SerialScreen771(Stream &s_)
 :DFRobot_Flexible_STC8F(s_, 71){}

DFRobot_SerialScreen1248::DFRobot_SerialScreen1248(Stream &s_)
 :DFRobot_Flexible_STC8F(s_, 48){}
