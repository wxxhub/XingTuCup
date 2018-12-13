#include "locking_id.h"

LockingID::LockingID()
{
    init();
}

void LockingID::init()
{
    for (int i = 0; i < CACHE_SIZE; i++)
    {
        cache_list_[i].rectangle.up_x = 0;
        cache_list_[i].rectangle.up_y = 0;
        cache_list_[i].rectangle.down_x = 0;
        cache_list_[i].rectangle.down_y = 0;
        cache_list_[i].lose_times = 0;
        cache_list_[i].no_empty = false;
    }
}

void LockingID::addObject(Rectangle rectangle, int origin_id)
{
    LockingTarget new_target;
    new_target.rectangle = rectangle;
    new_target.origin_id = origin_id;

    add_target_list_.push_back(new_target);
}

void LockingID::addObject(int up_x, int up_y, int down_x, int down_y, int origin_id)
{
    LockingTarget new_target;
    new_target.rectangle.up_x = up_x;
    new_target.rectangle.up_y = up_y;
    new_target.rectangle.down_x = down_x;
    new_target.rectangle.down_y = down_y;
    new_target.origin_id = origin_id;

    add_target_list_.push_back(new_target);
}

// match by errorRange
/*
void LockingID::matchID()
{
    for (int i = 0; i < CACHE_SIZE; i++)
    {
        if (cache_list_[i].no_empty)
        {
            std::list<LockingTarget>::iterator iter = add_target_list_.begin();
            for (; iter != add_target_list_.end(); iter++)
            {
                int up_error = errorRange(cache_list_[i].rectangle.up_x, cache_list_[i].rectangle.up_y,
                                          iter->rectangle.up_x, iter->rectangle.up_y);
                int down_error = errorRange(cache_list_[i].rectangle.down_x, cache_list_[i].rectangle.down_y,
                                          iter->rectangle.down_x, iter->rectangle.down_y);

                if (up_error > 0 && down_error > 0)
                {
                    cache_list_[i].origin_id        = iter->origin_id;
                    cache_list_[i].rectangle.up_x   = iter->rectangle.up_x;
                    cache_list_[i].rectangle.up_y   = iter->rectangle.up_y;
                    cache_list_[i].rectangle.down_x = iter->rectangle.down_x;
                    cache_list_[i].rectangle.down_y = iter->rectangle.down_y;
                    cache_list_[i].lose_times = 0;
                    add_target_list_.erase(iter);
                    break;
                }
            }

            cache_list_[i].lose_times++;

            if (cache_list_[i].lose_times > MAX_LOSE_TIMES)
                cleanCache(i);
        }
    }

    for (int i = 0; i < CACHE_SIZE; i++)
    {
        if (!cache_list_[i].no_empty)
        {
            std::list<LockingTarget>::iterator iter = add_target_list_.begin();
            for (; iter != add_target_list_.end(); iter++)
            {
                cache_list_[i].origin_id        = iter->origin_id;
                cache_list_[i].rectangle.up_x   = iter->rectangle.up_x;
                cache_list_[i].rectangle.up_y   = iter->rectangle.up_y;
                cache_list_[i].rectangle.down_x = iter->rectangle.down_x;
                cache_list_[i].rectangle.down_y = iter->rectangle.down_y;
                cache_list_[i].lose_times = 0;
                cache_list_[i].no_empty = true;
                add_target_list_.erase(iter);
                break;
            }
        }
    }
}
*/

//  first cache_list_
/*
void LockingID::matchID()
{
    for (int i = 0; i < CACHE_SIZE; i++)
    {
        if (cache_list_[i].no_empty)
        {
            std::list<LockingTarget>::iterator iter = add_target_list_.begin();
            cache_list_[i].origin_id = -1;
            for (; iter != add_target_list_.end(); iter++)
            {
                if (!ifCoincide(cache_list_[i].rectangle, iter->rectangle))
                    continue;

                float area_error = errorArea(cache_list_[i].rectangle, iter->rectangle);
                float area_error2 = errorArea(iter->rectangle, cache_list_[i].rectangle);

                printf("error: %f\n", area_error);
                printf("error2: %f\n", area_error2);
                if ((area_error > MUST_AREA_ERROR && area_error2 > MIN_AREA_ERROR)||
                    (area_error2 > MUST_AREA_ERROR && area_error > MIN_AREA_ERROR))
                {
                    cache_list_[i].origin_id        = iter->origin_id;
                    cache_list_[i].rectangle.up_x   = iter->rectangle.up_x;
                    cache_list_[i].rectangle.up_y   = iter->rectangle.up_y;
                    cache_list_[i].rectangle.down_x = iter->rectangle.down_x;
                    cache_list_[i].rectangle.down_y = iter->rectangle.down_y;
                    cache_list_[i].lose_times = 0;
                    add_target_list_.erase(iter);
                    break;
                }
            }

            cache_list_[i].lose_times++;

            if (cache_list_[i].lose_times > MAX_LOSE_TIMES)
                cleanCache(i);
        }
    }

    for (int i = 0; i < CACHE_SIZE; i++)
    {
        if (!cache_list_[i].no_empty)
        {
            std::list<LockingTarget>::iterator iter = add_target_list_.begin();
            for (; iter != add_target_list_.end(); iter++)
            {
                cache_list_[i].origin_id        = iter->origin_id;
                cache_list_[i].rectangle.up_x   = iter->rectangle.up_x;
                cache_list_[i].rectangle.up_y   = iter->rectangle.up_y;
                cache_list_[i].rectangle.down_x = iter->rectangle.down_x;
                cache_list_[i].rectangle.down_y = iter->rectangle.down_y;
                cache_list_[i].lose_times = 0;
                cache_list_[i].no_empty = true;
                add_target_list_.erase(iter);
                break;
            }
        }
    }
}
*/

//  first add_target_list_
void LockingID::matchID()
{  
    for (int i = 0; i < CACHE_SIZE; i++)
    {
        cache_list_[i].origin_id = -1;
    }

    std::list<LockingTarget>::iterator iter = add_target_list_.begin();
    while (iter != add_target_list_.end())
    {
        std::list<LockingTarget>::iterator erase_iter = iter;
        int a_switch = -1;
        for (int i = 0; i < CACHE_SIZE; i++)
        {
            if (!cache_list_[i].no_empty)
            {
                a_switch = 1;
                continue;
            }

            if (!ifCoincide(cache_list_[i].rectangle, iter->rectangle))
            {
                a_switch = 2;
                continue;
            }

            float area_error = errorArea(cache_list_[i].rectangle, iter->rectangle);
            float area_error2 = errorArea(iter->rectangle, cache_list_[i].rectangle);

            // printf("error: %f\n", area_error);
            // printf("error2: %f\n", area_error2);
            if ((area_error > MUST_AREA_ERROR && area_error2 > MIN_AREA_ERROR)||
                (area_error2 > MUST_AREA_ERROR && area_error > MIN_AREA_ERROR))
            {
                if (cache_list_[i].origin_id != -1)
                {
                    int origin_area = (cache_list_[i].rectangle.down_y - cache_list_[i].rectangle.up_y)
                                      *(cache_list_[i].rectangle.down_x - cache_list_[i].rectangle.up_x);

                    int present_area = (iter->rectangle.down_y - iter->rectangle.up_y)
                                      *(iter->rectangle.down_x - iter->rectangle.up_x);
                    if (origin_area < present_area)
                    {
                        cache_list_[i].origin_id        = iter->origin_id;
                        cache_list_[i].rectangle.up_x   = iter->rectangle.up_x;
                        cache_list_[i].rectangle.up_y   = iter->rectangle.up_y;
                        cache_list_[i].rectangle.down_x = iter->rectangle.down_x;
                        cache_list_[i].rectangle.down_y = iter->rectangle.down_y;
                        cache_list_[i].lose_times++;   
                    }
                }
                else
                {
                    cache_list_[i].origin_id        = iter->origin_id;
                    cache_list_[i].rectangle.up_x   = iter->rectangle.up_x;
                    cache_list_[i].rectangle.up_y   = iter->rectangle.up_y;
                    cache_list_[i].rectangle.down_x = iter->rectangle.down_x;
                    cache_list_[i].rectangle.down_y = iter->rectangle.down_y;
                }
                cache_list_[i].lose_times = 0;
                cache_list_[i].no_empty = true;
                a_switch = 0;
                break;
            }
            
            cache_list_[i].lose_times++;

            if (cache_list_[i].lose_times > MAX_LOSE_TIMES)
                cleanCache(i);
        }

        iter++;

        if (a_switch == 0)
        {
            add_target_list_.erase(erase_iter);
        }
    }

    if (add_target_list_.size() <= 0)
        return;

    for (int i = 0; i < CACHE_SIZE; i++)
    {
        if (!cache_list_[i].no_empty)
        {
            std::list<LockingTarget>::iterator iter = add_target_list_.begin();
            for (; iter != add_target_list_.end(); iter++)
            {
                cache_list_[i].origin_id        = iter->origin_id;
                cache_list_[i].rectangle.up_x   = iter->rectangle.up_x;
                cache_list_[i].rectangle.up_y   = iter->rectangle.up_y;
                cache_list_[i].rectangle.down_x = iter->rectangle.down_x;
                cache_list_[i].rectangle.down_y = iter->rectangle.down_y;
                cache_list_[i].lose_times = 0;
                cache_list_[i].no_empty = true;
                add_target_list_.erase(iter);
                break;
            }
        }
    }
}

int LockingID::errorRange(int x_site, int y_site, int judge_x_site, int judge_y_site, int allowable_error)
{
    int x_error = judge_x_site - x_site;
    int y_error = judge_y_site - x_site;

    // printf("???");
    // printf("x_error: %d", x_error);
    // printf("y_error: %d", y_error);
    // printf("\n");
    if (x_error > allowable_error || y_error > allowable_error)
        return OUT_RANGE;
    // else if (x_error > 0 && y_error > 0)
    //     return UP_RIGHT;
    // else if (x_error < 0 && y_error > 0)
    //     return UP_LEFT;
    // else if (x_error < 0 && y_error < 0)
    //     return DOWN_LEFT;
    // else if (x_error > 0 && y_error < 0)
    //     return DOWN_RIGHT;
    else
        return IN_RANGE;
}

bool LockingID::ifCoincide(Rectangle rectangle, Rectangle judge_rectangle)
{
    if (rectangle.down_x < judge_rectangle.up_x||
        rectangle.down_y < judge_rectangle.up_y||
        judge_rectangle.down_x < rectangle.up_x||
        judge_rectangle.down_y < rectangle.up_y)
        return false;
    else
        return true;
}

float LockingID::errorArea(Rectangle rectangle, Rectangle judge_rectangle)
{
    int up_x = rectangle.up_x > judge_rectangle.up_x ? rectangle.up_x : judge_rectangle.up_x;
    int up_y = rectangle.up_y > judge_rectangle.up_y ? rectangle.up_y : judge_rectangle.up_y;
    int down_x = rectangle.down_x < judge_rectangle.down_x ? rectangle.down_x : judge_rectangle.down_x;
    int down_y = rectangle.down_y < judge_rectangle.down_y ? rectangle.down_y : judge_rectangle.down_y;

    float result = 1.0 * (down_x - up_x) * (down_y - up_y) / ((rectangle.down_x - rectangle.up_x) * (rectangle.down_y - rectangle.up_y));
    return result;
}

void LockingID::cleanCache(int site)
{
    printf("clear: %d\n", site);
    cache_list_[site].rectangle.up_x = 0;
    cache_list_[site].rectangle.up_y = 0;
    cache_list_[site].rectangle.down_x = 0;
    cache_list_[site].rectangle.down_y = 0;
    cache_list_[site].lose_times = 0;
    cache_list_[site].no_empty = false;
}

int LockingID::getID(int origin_id)
{
    printf("###################\n");
    for (int i = 0; i < CACHE_SIZE; i++)
    {
        if (cache_list_[i].no_empty)
        {
            printf("origin_id: %d  lost_times: %d\n", cache_list_[i].origin_id, cache_list_[i].lose_times);
        }
    }
    printf("*******************\n");
    for (int i = 0; i < CACHE_SIZE; i++)
    {
        if (cache_list_[i].no_empty)
        {
            if (origin_id == cache_list_[i].origin_id)
            {
                return i;
            }
        }
    }
    return -2;
}