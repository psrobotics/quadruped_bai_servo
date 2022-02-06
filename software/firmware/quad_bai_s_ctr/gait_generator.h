//Quad controller for atmega328p
//Shuang Peng - 2019-02 ver0.2

#ifndef _GAIT_GENERATOR__
#define _GAIT_GENERATOR__
#include "Arduino.h"
#include "matrix.h"
#include "kinematics.h"

#define LOADING_A      1
#define COMPRESSION_A  2
#define THRUST_A       3
#define UNLOADING_A    4
#define FLIGHT_A       5

#define LOADING_B      6
#define COMPRESSION_B  7
#define THRUST_B       8
#define UNLOADING_B    9
#define FLIGHT_B       10

#define THR_A 11
#define THR_B 12

#define GROUND    0
#define SKY       1

#define FL_BR_SKY     20
#define FL_BR_GROUND  21
#define FR_BL_SKY     22
#define FR_BL_GROUND  23

#define ALL_GROUND   24
#define ALL_SKY      25

#define FL 0
#define BL 1
#define BR 2
#define FR 3

#define height_waiter 4
#define flight_killer 7
#define pose_waiter 4
#define pose_delta 0.2

class gait_generator
{
  public:
    float trot_height;
    float trot_delay;

    float trot_len;

    int main_state;
    int time_interval;

    unsigned long now_time;
    unsigned long now_time_old;

    unsigned long atime_ground;
    unsigned long atime_ground_old;
    unsigned long btime_ground;
    unsigned long btime_ground_old;

    float atime_ground_s;
    float btime_ground_s;

    float height_storage[4];

    float changing_seed;
    int changing_dir;

    int height_counter;

    float rpy_fi;
    float rpy_fi_1;
    int rpy_counter;

    int pos_counter;

    float turn_angle;
    float turn_l;

    gait_generator(body &dog_s);
    int judge_leg(int *leg_state);
    void trot_changer(body &dog_s, int *leg_state, float *rpy, float *pos, float tar_vec_x, float tar_vec_y, float turn_vec, float *rpy_read, float *rpy_target, float *rpy_f_k);
};
#endif
