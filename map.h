/* 
 * File: map.h
 * This class that holds the state of the course and reads from and writes to EEPROM.
 */

#ifndef __MAP_H__
#define __MAP_H__

#define START 0
#define BREAK 1
#define BRANCH_R 2
#define BRANCH_L 3
#define STOP 4

class Point {
    public:
    unsigned char type_;         // start:0 break:1 branch_right:2 branch_left:3 stop:4
    unsigned int place_;
    Point* next_point_ = nullptr;
    Point(unsigned char type, unsigned int place): type_(type), place_(place) {}
    ~Point(){
        if (next_point_ != this && next_point_ != nullptr)
            delete next_point_;
    }
};

/** EEPROM format **
 * |  0-3  |   4   |   5  |  6-9  |   10  | 11-14  | ...
 * |-------|-------|------|-------|-------|--------| ...
 * |  red  | event |  last event  | previous event | ...
 * | color | count | type | place |  type | place  | ...
 */

class Map {
    private:
    Point* last_point_;
    Point* start_point_;
    Point* current_point_;
    unsigned char event_count_ = 0;

    public:
    Map(){
        //start point
        last_point_ = new Point(0, 0);
        start_point_ = last_point_;
        current_point_ = start_point_;

        event_count_++;
    }
    ~Map(){
        delete last_point_;
    }

    void addPoint(unsigned char type, unsigned int place);  // add point to this map
    void save();    // save this map in EEPROM
    void load();    // load map from EEPROM
    bool isNextPoint(unsigned int now_place){
        if(current_point_->next_point_ == nullptr){
            return false;
        }
        else if (current_point_->next_point_->place_ < now_place + 250){
            return true;
        }
        else{
            return false;
        }
    }
    unsigned char getNextPointType(){
        return current_point_->next_point_->type_;
    }
    void goNext(){
        current_point_ = current_point_->next_point_;
    }
};

class Branch{
    private:
    unsigned int begin_point_;
    bool turn_;
    Map* course_;

    public:
    Branch(Map* course) : begin_point_(0), turn_(false), course_(course) {}
    void begin(unsigned int point);
    unsigned int end();
    void end_drive(){
        if(turn_ == true)
            course_->addPoint(BRANCH_L, begin_point_);
    }
};

#endif /* __MAP_H__ */