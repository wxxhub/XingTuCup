#ifndef _JUDGE_STATE_
#define _JUDGE_STATE_

#include <list>

#define NORMAL     0
#define FALLDOWN  -1
#define STANDUP    1
#define STANDUP_B -5
#define FALLDOWN_B 5

#define EDGE_ERROR 0.95
#define HORIZONTAL_B 1

class JudgeState
{
public:
    JudgeState();
    void setFPS(int fps);
    int getJudgeResult(int up_x, int up_y, int down_x, int down_y, int img_h = 680, bool show_result = false);
    void cleanJudgeList();

private:
    int state_;
    std::list<int> up_site_list_;
    std::list<int> down_site_list_;
    std::list<int> vertical_site_list_;
    std::list<int> horizontal_distance_list_;
    int judge_size_;
    int judge_times_;

    double n_average_x_;
    double x_data_;

    int getFilttingResult(std::list<int> judge_site_list);
    float judgeShape(int up_x, int up_y, int down_x, int down_y);
};

#endif /* _JUDGE_STATE_ */