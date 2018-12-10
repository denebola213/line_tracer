
#include "stdafx.h"
#include "tracer.h"

float Tracer::trace(const unsigned int sensors_val[]){
    // find the convex number of the sensors value
    // and find not white value
    unsigned char brack_i_first = 8;
    unsigned char brack_i_last = 8;
    unsigned char max_i;
    unsigned char max_2nd_i;
    unsigned int max_val;
    unsigned int max_2nd_val;

    for(unsigned char i = 0; i < 8; i++){
        //convex
        if (max_val >= sensors_val[i] && max_2nd_val < sensors_val[i]){
            max_2nd_i = i;
        }
        else if (max_val < sensors_val[i]){
            max_i = i;
        }

        //white
        if (sensors_val[i] > WHITE_COLOR){
            if (brack_i_first == 8){
                brack_i_first = i;
                brack_i_last = i;
            }
            else{
                brack_i_last = i;
            }
        }
    }

    // branch decision
    // and find center position
    float center;
    // not branch
    if(-1 <= (max_2nd_i - max_i) && (max_2nd_i - max_i) <= 1){
        center = ((brack_i_first + brack_i_last) / (1 + sensors_val[brack_i_first] / sensors_val[brack_i_last])) + brack_i_first;
        branch_flag_ = false;
    }
    // branch
    else if(max_i > max_2nd_i){
        if(max_i == 0){
            center = (1 / (1 + sensors_val[max_i] / sensors_val[max_i+1]));
        }
        else if(max_i == 7){
            center = (1 / (1 + sensors_val[max_i-1] / sensors_val[max_i])) + 6;
        }
        else{
            center = (2 / (1 + sensors_val[max_i-1] / sensors_val[max_i+1])) + max_i - 1;
        }
        branch_flag_ = true;
    }
    else if(max_2nd_i > max_i){
        if(max_2nd_i == 0){
            center = (1 / (1 + sensors_val[max_2nd_i] / sensors_val[max_2nd_i+1]));
        }
        else if(max_2nd_i == 7){
            center = (1 / (1 + sensors_val[max_2nd_i-1] / sensors_val[max_2nd_i])) + 6;
        }
        else{
            center = (2 / (1 + sensors_val[max_2nd_i-1] / sensors_val[max_2nd_i+1])) + max_2nd_i - 1;
        }
        branch_flag_ = true;
    }

    return compute(3.5, center) * 20;
}

float Tracer::trace(const unsigned int sensor_val){
    return compute(GRAY_COLOR, sensor_val);
}
