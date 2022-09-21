#ifndef _KINEMATICS_H_
#define _KINEMATICS_H_

#include <math.h>

#define WHEEL_RADIUS 97.5
#define ROBOT_LENGTH 381

float vel_res[1][0];
float vel_inp[1][0];
float logic[][2] = {
        {WHEEL_RADIUS/2,                    WHEEL_RADIUS/2},
        {WHEEL_RADIUS/(ROBOT_LENGTH*2),     -WHEEL_RADIUS/(ROBOT_LENGTH*2)},
};

void swap(float* a, float* b)
{
    float temp = *a;
    *a = *b;
    *b = temp;
}

float reverseArray(float arr[2][2]){
    for (int i = 0; i < 2; i++) {
        int start = 0;
        int end = 2 - 1; 
        while (start < end) {
            swap(&arr[i][start],
                 &arr[i][end]);
            start++;
            end--;
        }
    }
}

float forwardKinematics(int _vRightWheel, int _vLeftWheel){    
    vel_res[0][0] = (logic[0][0] * _vRightWheel) + (logic[0][1] * _vLeftWheel);
    vel_res[0][1] = (logic[1][0] * _vRightWheel) + (logic[1][1] * _vLeftWheel);

    return vel_res[0][0], vel_res[0][1];
}

float inreverseKinematics(float _vx, float _vy){

    return (logic[0][0] * _vx) + (logic[0][1] * _vy), 
    (logic[1][0] * _vx) + (logic[1][1] * _vy);
}



#endif