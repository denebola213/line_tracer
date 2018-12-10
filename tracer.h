
#ifndef __TRACER_H__
#define __TRACER_H__

#define GRAY_COLOR 590
#define WHITE_COLOR 250

class PD {
    private:
    const float kp_;
    const float kd_;

    float p_;
    float d_;
    float pre_val_;

    public:
    PD(const float kp, const float kd) : kp_(kp), kd_(kd) {}
    inline float compute(const float, const float);
};

inline float PD::compute(const float target, const float val) {
    pre_val_ = val;

    return kp_ * (val - target) + kd_ * (val - pre_val_);
}

class Tracer : public PD{
    private:
    bool branch_flag_;

    public:
    Tracer(const float kp, const float kd) : PD(kp, kd) {}
    float trace(const unsigned int sensors_val[]); //first run
    float trace(const unsigned int sensor_val);    //second run
    bool isBranch(){
        return branch_flag_;
    }
};

#endif /* __TRACER_H__ */