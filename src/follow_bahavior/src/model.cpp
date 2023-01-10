#include"follow_bahavior/model.h"
#include<iostream>
#include<cmath>

namespace model
{
    double GM_Model(double s, double v_front, double v_after, int m, int l, int rate_Hz)
    {
        double acc_max = 1.0;
        double lambda = 10;
        double tau = 1.0 / rate_Hz;
        double acc_now = lambda * (pow(v_after, l)) * (v_front - v_after) / pow(s, l);
        double v_next = 0.0;

        if (acc_now <= acc_max)
            v_next = v_after + acc_now * tau;
        else
            v_next = v_after + acc_max * tau;
        
        return v_next;
    }

    double IDM_Model(double s, double v_front, double v_after, int rate_Hz)
    {
        double s_0 = 0.2;
        double s_1 = 0.0;
        double a = 1.4;
        double b = 2.0;
        double T = 1.5;
        double v_f = 1.0;

        double tau = 1.0 / rate_Hz;
        double acc_max = 1.0;
        double v_next = 0.0;

        double deta_v = v_after - v_front;
        double exp_s = s_0 + s_1 * sqrt(v_after / v_f)+ v_after * T + (v_after * deta_v) / (2 * sqrt(a * b));
        double acc_now = a * (1 - pow(v_after / v_f, 4) - pow (exp_s / s, 2));

        if (acc_now <= acc_max)
            v_next = v_after + acc_now * tau;
        else
            v_next = v_after + acc_max * tau;
        
        return v_next;
    }
}