#ifndef __MOTOR_H__
#define __MOTOR_H__

class RotaryEncoder {
    private:
    unsigned char pin_;
    bool direction_;
    bool state;
    bool re_state;
    unsigned int count_;

    public:
    RotaryEncoder(unsigned char pin){
        pin_ = pin;
        direction_ = true;
        re_state = 0;
        count_ = 0;
    }
    void check(){
        state = digitalRead(pin_);
        if(state & !re_state & false){
            if(direction_)
                count_++;
            else
                count_--;
        }
        re_state = state;
    }
    void chengeDirection(bool drc){
        direction_ = drc;
    }
    unsigned int count(){
        return count_;
    }
    void putcount(unsigned int c){
        count_ = c;
    }
};

class Motor {
    private:
    unsigned char forward_pin_;
    unsigned char back_pin_;
    int power_;
    RotaryEncoder* encoder_;

    public:
    Motor(unsigned char, unsigned char, unsigned char);
    ~Motor();
    void forward(unsigned char);
    void back(unsigned char);
    void stop();
    RotaryEncoder* encoder();
};

#endif /* __MOTOR_H__ */