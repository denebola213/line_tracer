

#include "stdafx.h"
#include "map.h"
#include <EEPROM.h>

void Map::addPoint(unsigned char type, unsigned int place){
        last_point_->next_point_ = new Point(type, place);
        last_point_ = last_point_->next_point_;

        event_count_++;
}

void Map::save(){
    // EEPROM initialization
    for(int i = 0;i < EEPROM.length(); i++){
        EEPROM.write(i, 0);
    }

    int rom_pointer = 0;
    
    // write event count
    EEPROM.write(rom_pointer, event_count_);
    rom_pointer++;

    // write points data
    Point* event_point = start_point_;
    while(event_point != nullptr){
        EEPROM.write(rom_pointer, event_point->type_);
        rom_pointer++;
        for(char i = 3;i >= 0; i--){
            EEPROM.write(rom_pointer, (unsigned char)(event_point->place_ >> i * 8));
            rom_pointer++;
        }

        event_point = event_point->next_point_;
    }
}

void Map::load(){
        // read event count
        const unsigned char event_count = EEPROM.read(0);

        // read points data
        int rom_pointer = 1;
        for (int i = 1; i < event_count; i++) {
            unsigned int tmp_place = 0;
            for (int j = 1; j <= 4; j++){
                tmp_place |= (unsigned int)EEPROM.read(1 + 5 * i + j) << (8 * (4 - j));
            }

            Serial.print("type:");
            Serial.print(EEPROM.read(1 + 5 * i));
            Serial.print(" place:");
            Serial.println(tmp_place);

            addPoint(EEPROM.read(1 + 5 * i), tmp_place);
        }
    }


void Branch::begin(unsigned int point){
    if(turn_){
        course_->addPoint(BRANCH_L, begin_point_);
        begin_point_ = point;
    }
    else{
        begin_point_ = point;
        turn_ = true;
    }
}

unsigned int Branch::end(){
    course_->addPoint(BRANCH_R, begin_point_);
    turn_ = false;
    return begin_point_;
}