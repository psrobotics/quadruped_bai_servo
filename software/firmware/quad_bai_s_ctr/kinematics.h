//Quad controller for atmega328p
//Shuang Peng - 2019-02 ver0.2

#ifndef _KINEMATICS__
#define _KINEMATICS__
#include "matrix.h"
#include "Arduino.h"

class leg;
class body;

class leg
{
  public:
    float hip; float knee; float ab;
    float leg_l1; float leg_l2; float leg_l3;

    void leg_ik(const vec3d &leg_vec);
};

class body
{
  public:
    leg fl; leg fr; leg bl; leg br;

    float body_ab_len;
    float body_hip_len;
    float foot_ab_len;
    float foot_hip_len;//for static standing position

    float rad_rpy[3];
    float pos_offset[3];

    vec3d body_pfl;
    vec3d body_pfr;
    vec3d body_pbl;
    vec3d body_pbr;//body outer point pos

    vec3d leg_pfl;
    vec3d leg_pfr;
    vec3d leg_pbl;
    vec3d leg_pbr;//leg outer point pos

    vec3d leg_vec_fl;
    vec3d leg_vec_fr;
    vec3d leg_vec_bl;
    vec3d leg_vec_br;//leg pointer pos vector

    body(float b_lab, float b_lhip, float f_lab, float f_lhip, float *rpy, float *pos,float *leg_len);
    void gen_body_mat();
    void gen_foot_mat();
    void update_leg_vec();
    void change_dog_pos(float *rpy, float *pos);
};
#endif
