#### 开发日志

#### 目录
> [2018-12-10](#2018-12-10)
> [2018-12-12](#2018-12-12)
> [2018-12-13](#2018-12-13)

#### 2018-12-10
> Locking ID序需求
> 传入坐标信息
> 根据坐标信息和大小跟上次矩形匹配
> 返回对应ID

#### 2018-12-12
> 修复多目标跟踪BUG，getId()结果判定最大值判定改为CACHE_SIZE
>
> 待解决问题:
> 1. 没有匹配成功时,lose_times没有变化的问题
> 2. judge[id]重置

#### 2018-12-13
> 解决没有匹配成功时,lose_times没有变化的问题和judge[id]重置问题
> 测试
> 1. 忽略掉面积突然变小的目标
> 2. 重新判断时不初始化前面judge_size_个judge_list