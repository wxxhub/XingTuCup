#ifndef _LOCKING_ID_
#define _LOCKING_ID_

#include <list>

#include <stdio.h>
#include <iostream>

#define CACHE_SIZE 5
#define MAX_LOSE_TIMES 20

// AREA_ERROR
#define FILTER_AREA_ERROR 0.4  //   避免出现突然变小的面积带走ID
#define MIN_AREA_ERROR  0.2
#define MUST_AREA_ERROR 0.6

// cache_list_ abnormal origion_id
#define MATCH_ID 0
#define INIT_ID -1
#define CLEANER_ID -2
#define NO_MATCH_ID -10

#define OUT_RANGE   0
#define UP_RIGHT    1
#define UP_LEFT     2
#define DOWN_LEFT   3
#define DOWN_RIGHT  4
#define IN_RANGE    5

struct Rectangle
{
    int up_x;
    int up_y;
    int down_x;
    int down_y;
};

struct LockingTarget
{
    bool no_empty;
    int origin_id;
    Rectangle rectangle;
    int lose_times;
};

class LockingID
{
public:
    LockingID();
    void addObject(Rectangle rectangle, int origin_id);
    void addObject(int up_x, int up_y, int down_x, int down_y, int origin_id);
    void matchID();
    int errorRange(int x_site, int y_site, int judge_x_site, int judge_y_site, int allowable_error = 100);
    bool ifCoincide(Rectangle rectangle, Rectangle judge_rectangle);
    float errorArea(Rectangle rectangle, Rectangle judge_rectangle);
    int getID(int origin_id);
    int getResult(int id, int& up_x, int& up_y, int& down_x, int& down_y);


private:
    int target_match_size;
    LockingTarget cache_list_[CACHE_SIZE];
    std::list<LockingTarget> add_target_list_;
    
    void init();
    void cleanCache(int site);
    void checkClean();
};

#endif /* _LOCKING_ID_ */