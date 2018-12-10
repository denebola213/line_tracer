/* Team4 PBL - Line trace robot program
 * File: main.cpp, main loop, setup and macro definition
 */

#include "stdafx.h"
#include <MsTimer2.h>
#include "line_tracer.h"

#include "motor.h"
#include "map.h"
#include "tracer.h"
#include "sensor.h"

unsigned int timer = 0;

Motor right_motor(MOTOR_R_NO, MOTOR_R_RE, ROTARY_R);
Motor left_motor(MOTOR_L_NO, MOTOR_L_RE, ROTARY_L);

// interruption function
void count(){
    timer++;
}

void setup() {
    // pin initialization
    pinMode(BUTTON, INPUT_PULLUP);
    pinMode(MOTOR_R_NO, OUTPUT);
    pinMode(MOTOR_R_RE, OUTPUT);
    pinMode(MOTOR_L_NO, OUTPUT);
    pinMode(MOTOR_L_RE, OUTPUT);
    
    // debug
    Serial.begin(9600);

}

void loop() {
    
    // local
    bool first_run_flag = true;

    
    Sensor inside_r(SNSR_IR);
    Sensor inside_l(SNSR_IL);
    Sensor outside_r(SNSR_OR);
    Sensor outside_l(SNSR_OL);

    Map course;
    Tracer tracer(KP, KD);
    Branch branch(&course);

    /** setup process **
     * push BUTTON for 1 second: first run
     * push BUTTON for a moment: second run */
    while(1){
        if(digitalRead(BUTTON) == LOW){
            delay(1000);
            if(digitalRead(BUTTON) == LOW){
                first_run_flag = true;
                delay(1000); // 手を放すまで待機
                break;
            }
            else{
                first_run_flag = false;

                // load course map from EEPROM
                course.load();
                delay(1000); // 手を放すまで待機
                break;
            }
        }
    }


    /** driving process **/
    int turn_val;
    unsigned int inside_r_val;
    unsigned int inside_l_val;
    unsigned int outside_r_val;
    unsigned int outside_l_val;
    unsigned int now_time;

    // interruption setup
    MsTimer2::set(10, count);
    timer = 0;

    /* first running */
    if(first_run_flag == true){
        MsTimer2::start();
        while(1){
            now_time = timer;
            inside_r_val = inside_r.check();
            inside_l_val = inside_l.check();
            outside_r_val = outside_r.check();
            outside_l_val = outside_l.check();

            //Serial.println(now_time);
            
            // end of branch
            if(outside_r_val > BRACK_COLOR && outside_l_val > BRACK_COLOR){
                MsTimer2::stop();
                unsigned int branch_time = branch.end();

                // turn left 180°
                right_motor.stop();
                left_motor.stop();
                delay(1000);
                right_motor.back(70);
                left_motor.forward(70);
                delay(2000);
                while(inside_r.check() < BRACK_COLOR){}
                right_motor.stop();
                left_motor.stop();
                delay(1000);

                // back branch begin point
                while(outside_l.check() < BRACK_COLOR){
                    turn_val = (int)tracer.trace(inside_l.check());
                    right_motor.forward(70 + turn_val);
                    left_motor.forward(70 - turn_val);
                }
                right_motor.stop();
                left_motor.stop();
                delay(1000);
                right_motor.forward(70);
                left_motor.forward(70);
                delay(1000);
                right_motor.stop();
                left_motor.stop();
                delay(1000);
                right_motor.forward(70);
                left_motor.back(70);
                delay(1000);
                while(inside_l.check() < BRACK_COLOR){}
                right_motor.stop();
                left_motor.stop();
                delay(1000);

                timer = branch_time;

                MsTimer2::start();
            }
            // break point (end point)
            else if(inside_r_val < WHITE_COLOR && inside_l_val < WHITE_COLOR && outside_l_val < WHITE_COLOR && outside_l_val < WHITE_COLOR){
                right_motor.stop();
                left_motor.stop();
                branch.end_drive();
                course.addPoint(STOP, now_time);
                course.save();

                break;  // END RUN
            }
            // branch event
            else if(outside_r_val > BRACK_COLOR){
                branch.begin(now_time);
                while(outside_r.check() > WHITE_COLOR){
                    turn_val = (int)tracer.trace(inside_l.check());
                    right_motor.forward(80 + turn_val);
                    left_motor.forward(80 - turn_val);
                }
            }
            // nomal drive
            else{
                turn_val = (int)tracer.trace(inside_l_val);
                right_motor.forward(80 + turn_val);
                left_motor.forward(80 - turn_val);
                
            }
            
        }
    }

    /* second running */
    else{
        bool trace_side = false; // false:left, true:right


        MsTimer2::start();
        while(!(inside_r_val < WHITE_COLOR && inside_l_val < WHITE_COLOR && outside_l_val < WHITE_COLOR && outside_l_val < WHITE_COLOR)){
            now_time = timer;
            inside_r_val = inside_r.check();
            inside_l_val = inside_l.check();
            outside_r_val = outside_r.check();
            outside_l_val = outside_l.check();

            // event decision
            if(course.isNextPoint(now_time)){
                switch (course.getNextPointType()){
                    case BRANCH_R:
                        trace_side = true;
                        break;

                    case BRANCH_L:
                        trace_side = false;
                        break;
                
                    default:
                        break;
                }
                course.goNext();
            }

            // drive process
            if(trace_side == true){
                turn_val =  (int)tracer.trace(inside_r_val);
                right_motor.forward(80 - turn_val);
                left_motor.forward(80 + turn_val);
            }
            else{
                turn_val = (int)tracer.trace(inside_l_val);
                right_motor.forward(80 + turn_val);
                left_motor.forward(80 - turn_val);
            }
        }
        right_motor.stop();
        left_motor.stop();
        MsTimer2::stop();
    }

}