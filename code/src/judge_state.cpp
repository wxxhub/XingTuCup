#include "judge_state.h"

#include <stdio.h>

JudgeState::JudgeState()
    : state_(0),
      judge_size_(20),
      judge_times_(0),
      n_average_x_(210),
      x_data_(665)
{

}

void JudgeState::setFPS(int fps)
{
    judge_size_ = 2 * fps;
    int sum_xx = 0;
    double average_x = (1 + judge_size_)/2;
    double n_square_average_x = judge_size_ * average_x * average_x;

    n_average_x_ = judge_size_ * average_x;

    for (int i = 1; i <= judge_size_; i++)
    {
        sum_xx += i * i;
    }

    x_data_ = sum_xx - n_square_average_x;
}

int JudgeState::getFilttingResult(std::list<int> judge_site_list)
{
    int sum_y = 0;
    int sum_xy = 0;
    double average_y;

    std::list<int>::iterator iter = judge_site_list.begin();
    int i = 1;
    for (; iter != judge_site_list.end(); iter++)
    {
        sum_y += *iter;
        sum_xy += *iter * i;
        i++;
    }

    average_y = 1.0 * sum_y / judge_size_;

    double b = (sum_xy - n_average_x_ * average_y)/x_data_;
    return b;
}

int JudgeState::getJudgeResult(int up_x, int up_y, int down_x, int down_y, int img_h, bool show_result)
{
    if (judge_times_ < judge_size_)
    {
        // for (int i = judge_times_; judge_times_ < judge_size_; judge_times_++)
        // {
        //     up_site_list_.push_back(up_y);
        //     down_site_list_.push_back(down_y);
        //     vertical_site_list_.push_back(down_y - up_y);
        //     horizontal_distance_list_.push_back(down_x - up_x);
        // } 
        up_site_list_.push_back(up_y);
        down_site_list_.push_back(down_y);
        vertical_site_list_.push_back(down_y - up_y);
        horizontal_distance_list_.push_back(down_x - up_x);
        judge_times_++;
        return NORMAL;
    }
    else
    {
        up_site_list_.pop_front();
        down_site_list_.pop_front();
        vertical_site_list_.pop_front();
        horizontal_distance_list_.pop_front();

        up_site_list_.push_back(up_y);
        down_site_list_.push_back(down_y);
        vertical_site_list_.push_back(down_y - up_y);
        horizontal_distance_list_.push_back(down_x - up_x);
    }

    double up_b = getFilttingResult(up_site_list_);
    double down_b = getFilttingResult(down_site_list_);
    double vertical_b = getFilttingResult(vertical_site_list_);
    double horizontal_b = getFilttingResult(horizontal_distance_list_);
    if (show_result)
   {
        printf("b: %f\n", up_b);
        printf("vertical_b: %f\n", vertical_b);
        printf("horizontal_b: %f\n", horizontal_b);
   }
    int judge_result;
    if (up_b >= FALLDOWN_B &&  vertical_b <= FALLDOWN_B)
    {
        // edge error judge
        /*
        if(1.0 * down_y / img_h > EDGE_ERROR)
            if(horizontal_b < HORIZONTAL_B)
                return state_;
        */
        
        judge_result = FALLDOWN;
    }
    else if (up_b <= STANDUP_B)
        judge_result = STANDUP;

    if (judge_result == FALLDOWN)
        state_ = FALLDOWN;
    else if(judge_result == STANDUP)
        state_ = NORMAL;

    return state_;
}

float JudgeState::judgeShape(int up_x, int up_y, int down_x, int down_y)
{
    return (down_y - up_y) / (down_x - up_x);
}

void JudgeState::cleanJudgeList()
{
    judge_times_ = 0;
    up_site_list_.clear();
    down_site_list_.clear();
    vertical_site_list_.clear();
    horizontal_distance_list_.clear();
    state_ = NORMAL;
}