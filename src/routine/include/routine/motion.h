#ifndef _MOTION_H_
#define _MOTION_H_

#include "routine/pid.h"
#include <math.h>
#include <angles/angles.h>

void JalanManualPosisi(float _vx, float _vy)
{

}

bool JalanPosisiSudut(float _target_x, float _target_y, float _vposisi, float posisi_robot[2], float vOut[2]){
    static pid pid_posisi(0.1, 0.1, 0.1);

    float error_x = _target_x - posisi_robot[0];
    float error_z = _target_y - posisi_robot[1];
    float error_posisi = sqrt(error_x*error_x + error_z*error_z);

    float output_posisi = pid_posisi.calc(error_posisi, _vposisi);

    vOut[0] = output_posisi * cos(atan2(error_z, error_x));
    vOut[1] = output_posisi * sin(atan2(error_z, error_x));    

    if(error_posisi < 0.1)
        return true;
    else
        return false;
}

#endif