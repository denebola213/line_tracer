/* 
 * File: motor.cpp
 */

#include "stdafx.h"
#include "motor.h"

Motor::Motor(unsigned char fpin, unsigned char bpin, unsigned char rpin){
    forward_pin_ = fpin;
    back_pin_ = bpin;
    power_ = 0;
    encoder_ = new RotaryEncoder(rpin);
}
Motor::~Motor(){
    delete encoder_;
}

void Motor::forward(unsigned char pwr){
    power_ = pwr;
    analogWrite(forward_pin_, pwr);
    analogWrite(back_pin_, 0);
    encoder_->chengeDirection(true);
}

void Motor::back(unsigned char pwr){
    power_ = -pwr;
    analogWrite(back_pin_, pwr);
    analogWrite(forward_pin_, 0);
    encoder_->chengeDirection(false);
}

void Motor::stop(){
    power_ = 0;
    analogWrite(back_pin_, 0);
    analogWrite(forward_pin_, 0);
}

RotaryEncoder* Motor::encoder(){
    return encoder_;
}
