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

float inv_logic[][2] = {
        {5, 1},
        {5, 1},
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

// float inreverseKinematics(float _vx, float _vy){

//     return (logic[0][0] * _vx) + (logic[0][1] * _vy), 
//     (logic[1][0] * _vx) + (logic[1][1] * _vy);
// }

float inverseKinematics(float _vy, float _vz, float ret[2]){
    ret[0] = (inv_logic[0][0] * _vy) + (inv_logic[0][1] * _vz);
    ret[1] = (inv_logic[1][0] * _vy) - (inv_logic[1][1] * _vz);
    printf("logic: %f, %f, %f, %f\n", inv_logic[0][0], inv_logic[0][1], inv_logic[1][0], inv_logic[1][1]);
    printf("RET: %f, %f\n", ret[0], ret[1]);
    // ret[0] = (5 * _vy) + (1 * _vz);
    // ret[1] = (5 * _vy) - (1 * _vz);
}

float getProportional(float _kp, float _error){
    return _kp * _error;
}


#endif