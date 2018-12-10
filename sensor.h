/* 
* File: sensor.h
* class for a sensor
*/
#ifndef __SENSOR_H__
#define __SENSOR_H__

class Sensor {
    private:
    unsigned char pin_;  //pin number of sensor

    public:
    Sensor(unsigned char pin_num){
        pin_ = pin_num;
    }
    //update sensor value0
    unsigned int check(){
        return analogRead(pin_);
    }
};

class Sensors {
    private:
    Sensor* sensors_[8];
    unsigned int vals_[8];

    public:
    Sensors(const unsigned char* pin_nums){
        for(unsigned char i = 0; i < 8; i++){
            sensors_[i] = new Sensor(pin_nums[i]);
        }
    }
    ~Sensors(){
        for(unsigned char i = 0; i < 8; i++){
            delete sensors_[i];
        }
    }
    unsigned int* check(){
        for(unsigned char i = 0; i < 8; i++){
            vals_[i] = sensors_[i]->check();
        }
        return vals_;
    }
};

#endif /*__SENSOR_H__*/