#ifndef MODEL_H_
#define MODEL_H_
#include<iostream>

namespace model
{
    // GM跟驰模型，注意后车速度为0时无法启动
    // 参数s：两车间距/直线距离，参数v_front：前车当前时间步速度，参数v_after：后车当前时间步速度，
    // 参数m,l：GM模型参数，参数rate_Hz：消息发布频率，与TF发布频率相同
    double GM_Model(double s, double v_front, double v_after, int m, int l, int rate_Hz);
    // IDM跟驰模型
    // 参数s：两车间距/直线距离，参数v_front：前车当前时间步速度，参数v_after：后车当前时间步速度，
    // 参数rate_Hz：消息发布频率，与TF发布频率相同
    double IDM_Model(double s, double v_front, double v_after, int rate_Hz);
}
#endif