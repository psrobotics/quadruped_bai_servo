//Quad controller for atmega328p
//Shuang Peng - 2019-02 ver0.2

#include "kinematics.h"

void leg::leg_ik(const vec3d &leg_vec)
{
  float x, y, z, l1, l2, l3;
  static float pi = 3.1415926f;
  float fo1, fo2, fo3;
  x = leg_vec.vec[0]; y = leg_vec.vec[1]; z = leg_vec.vec[2];
  l1 = leg_l1; l2 = leg_l2; l3 = leg_l3;

  float ls = sqrt(pow(y, 2) + pow(z, 2));
  float o1 = acos(l1 / ls);
  float o2 = atan(z / y);

  if (o2 < 0) {
    o2 = o2; fo1 = o2 + o1;
  }
  else {
    o2 = o2 - pi; fo1 = o2 - o1;
  }

  float lm = sqrt(pow(ls, 2) - pow(l1, 2));
  float lm2 = sqrt(pow(x, 2) + pow(lm, 2));
  float o3 = atan(abs(x) / lm);

  if (x > 0)  o3 = pi / 2 - o3;
  else  o3 = pi / 2 + o3;

  float css1 = (pow(lm2, 2) + pow(l2, 2) - pow(l3, 2)) / (2 * lm2 * l2);
  float o4 = acos(css1);
  fo2 = o3 + o4;

  float css2 = (pow(l2, 2) + pow(l3, 2) - pow(lm2, 2)) / (2 * l2 * l3);
  float o5 = acos(css2);
  fo3 = o5;

  ab = fo1 / pi * 180;
  hip = fo2 / pi * 180;
  knee = fo3 / pi * 180;
}

body::body(float b_lab, float b_lhip, float f_lab, float f_lhip, float *rpy, float *pos, float *leg_len)
{
  body_ab_len = b_lab;
  body_hip_len = b_lhip;
  foot_ab_len = f_lab;
  foot_hip_len = f_lhip;

  for (int s = 0; s < 3; s++)
  {
    rad_rpy[s] = rpy[s];
    pos_offset[s] = pos[s];
  }
  fl.leg_l1 = leg_len[0]; fl.leg_l2 = leg_len[1]; fl.leg_l3 = leg_len[2];
  fr.leg_l1 = leg_len[0]; fr.leg_l2 = leg_len[1]; fr.leg_l3 = leg_len[2];
  bl.leg_l1 = leg_len[0]; bl.leg_l2 = leg_len[1]; bl.leg_l3 = leg_len[2];
  br.leg_l1 = leg_len[0]; br.leg_l2 = leg_len[1]; br.leg_l3 = leg_len[2];
}

void body::gen_body_mat()
{
  body_pfl.vec[0] = body_hip_len / 2;        body_pfl.vec[1] = body_ab_len / 2;        body_pfl.vec[2] = 0;
  body_pfr.vec[0] = body_hip_len / 2;        body_pfr.vec[1] = -1 * body_ab_len / 2;   body_pfr.vec[2] = 0;
  body_pbl.vec[0] = -1 * body_hip_len / 2;   body_pbl.vec[1] = body_ab_len / 2;        body_pbl.vec[2] = 0;
  body_pbr.vec[0] = -1 * body_hip_len / 2;   body_pbr.vec[1] = -1 * body_ab_len / 2;    body_pbr.vec[2] = 0;
}

void body::gen_foot_mat()
{
  leg_pfl.vec[0] = foot_hip_len / 2;        leg_pfl.vec[1] = foot_ab_len / 2;         leg_pfl.vec[2] = 0;
  leg_pfr.vec[0] = foot_hip_len / 2;        leg_pfr.vec[1] = -1 * foot_ab_len / 2;    leg_pfr.vec[2] = 0;
  leg_pbl.vec[0] = -1 * foot_hip_len / 2;   leg_pbl.vec[1] = foot_ab_len / 2;         leg_pbl.vec[2] = 0;
  leg_pbr.vec[0] = -1 * foot_hip_len / 2;   leg_pbr.vec[1] = -1 * foot_ab_len / 2;    leg_pbr.vec[2] = 0;
}

void body::update_leg_vec()
{
  matrix3d rot_mat;
  rot_mat.gen_rotation(rad_rpy);
  vec3d vec_offset;
  vec_offset.gen_trans(pos_offset);

  body_pfl = rot_mat * body_pfl; 
  body_pfl = body_pfl + vec_offset;
  body_pfr = rot_mat * body_pfr; 
  body_pfr = body_pfr + vec_offset;
  body_pbl = rot_mat * body_pbl; 
  body_pbl = body_pbl + vec_offset;
  body_pbr = rot_mat * body_pbr; 
  body_pbr = body_pbr + vec_offset;

  leg_vec_fl = leg_pfl - body_pfl;
  leg_vec_fr = leg_pfr - body_pfr;
  leg_vec_bl = leg_pbl - body_pbl;
  leg_vec_br = leg_pbr - body_pbr;

  fl.leg_ik(leg_vec_fl);
  fr.leg_ik(leg_vec_fr);
  bl.leg_ik(leg_vec_bl);
  br.leg_ik(leg_vec_br);
}

void body::change_dog_pos(float *rpy, float *pos)
{
  gen_body_mat();
  gen_foot_mat();
  for(int s=0;s<3;s++)
  {
    rad_rpy[s]=rpy[s];
    pos_offset[s]=pos[s];
  }
  update_leg_vec();
}
