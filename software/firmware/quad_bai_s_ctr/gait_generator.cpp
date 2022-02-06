//Quad controller for atmega328p
//Shuang Peng - 2019-02 ver0.2

#include "gait_generator.h"
#include "Arduino.h"

gait_generator::gait_generator(body &dog_s)
{
  now_time = 0;
  now_time_old = 0;
  main_state = -1;
  time_interval = 1;
  changing_seed = 0;
  changing_dir = 1;
  dog_s.gen_foot_mat();

  //trot_len = 2;
  turn_angle = 0;
  turn_l = (pow(dog_s.foot_ab_len, 2) + pow(dog_s.foot_hip_len, 2)) / 2;
}

int gait_generator::judge_leg(int *leg_state)
{
  if (leg_state[1] == SKY && leg_state[3] == SKY)
    return FR_BL_SKY;
  //else if (leg_state[0] == GROUND || leg_state[2] == GROUND)
  //return FL_BR_GROUND;
  else if ((leg_state[0] == SKY && leg_state[2] == SKY))
    return FL_BR_SKY;
  //else if (leg_state[3] == GROUND || leg_state[1] == GROUND)
  //return FR_BL_GROUND;
  else
    return -1;
}

void gait_generator::trot_changer(body &dog_s, int *leg_state, float *rpy, float *pos, float tar_vec_x, float tar_vec_y, float turn_vec, float *rpy_read, float *rpy_target, float *rpy_f_k)
{
  now_time = millis();
  int time_d = now_time - now_time_old;

  if (atime_ground_s > 0.7)  atime_ground_s = 0.7;
  if (btime_ground_s > 0.7)  btime_ground_s = 0.7;

  if (pos[0] - 2.3 > 1) pos[0] = 2.3 + 1;
  else if (pos[0] - 2.3 < -1) pos[0] = 2.3 - 1;

  if (pos[1]  > 0.5) pos[0] = 0.5;
  else if (pos[1]  < -0.5) pos[1] = - 0.5;

  for (int s = 0; s < 4; s++)
  {
    if (height_storage[s] < -0.7) height_storage[s] = -0.7;
  }

  //if (time_d > time_interval)
  if (1)
  {
    dog_s.gen_foot_mat();//generate initial foot pos

    for (int s = 0; s < 3; s++)
    {
      dog_s.rad_rpy[s] = rpy[s];
      dog_s.pos_offset[s] = pos[s];
    }

    float turn_l_tx;
    float turn_l_ty;

    switch (main_state) {// state machine
      case LOADING_A:
        //delay(50);

        main_state = COMPRESSION_A;
        break;
      case COMPRESSION_A:

        main_state = THRUST_A;
        break;
      case THRUST_A:
        height_counter = 0;

        main_state = UNLOADING_A;
        break;
      case UNLOADING_A:
        turn_angle = turn_vec * btime_ground_s / 2;
        turn_l_tx = turn_angle * turn_l * sin(turn_angle);
        turn_l_ty = turn_angle * turn_l * cos(turn_angle);

        dog_s.leg_pfr.vec[0] += tar_vec_x * btime_ground_s / 2 - turn_l_tx;
        dog_s.leg_pbl.vec[0] += tar_vec_x * btime_ground_s / 2 + turn_l_tx;

        dog_s.leg_pfr.vec[1] += tar_vec_y * btime_ground_s / 2 + turn_l_ty;
        dog_s.leg_pbl.vec[1] += tar_vec_y * btime_ground_s / 2 - turn_l_ty;
        dog_s.leg_pfr.vec[2] += trot_height;
        dog_s.leg_pbl.vec[2] += trot_height;

        height_storage[FR] = trot_height; height_storage[FL] = 0;
        height_storage[BL] = trot_height; height_storage[BR] = 0;

        height_counter++;

        if (leg_state[FR] == SKY && leg_state[BL] == SKY && height_counter > height_waiter)
        {
          main_state = FLIGHT_A;
          atime_ground = millis();
          atime_ground_s = (atime_ground - atime_ground_old) / 1000.0f;

          rpy_fi = 0;
          rpy_fi_1 = 0;
          rpy_counter = 0;
        }
        break;
      case FLIGHT_A:
        //float turn_l_t1 = (turn_angle * turn_l) / 1.414;
        rpy_fi += (rpy_read[1] - rpy_target[1]);
        rpy_fi_1 += (rpy_read[0] - rpy_target[0]);
        rpy_counter++;
        //delay(10);
        dog_s.leg_pfr.vec[0] += tar_vec_x * btime_ground_s / 2 - turn_l_tx;
        dog_s.leg_pbl.vec[0] += tar_vec_x * btime_ground_s / 2 + turn_l_tx;

        dog_s.leg_pfr.vec[1] += tar_vec_y * btime_ground_s / 2 + turn_l_ty;
        dog_s.leg_pbl.vec[1] += tar_vec_y * btime_ground_s / 2 - turn_l_ty;

        dog_s.leg_pfl.vec[0] -= tar_vec_x * btime_ground_s / 2 ;
        dog_s.leg_pbr.vec[0] -= tar_vec_x * btime_ground_s / 2 ;

        dog_s.leg_pfl.vec[1] -= tar_vec_y * btime_ground_s / 2 ;
        dog_s.leg_pbr.vec[1] -= tar_vec_y * btime_ground_s / 2 ;

        //dog_s.pos_offset[0] = pos[0] + tar_vec_x * atime_ground_s / 2;
        //dog_s.pos_offset[1] = pos[1] + tar_vec_y * atime_ground_s / 2;

        if (leg_state[FR] == SKY)
        {
          //height_storage[FR] -= 0.4;
          height_storage[FR] = 0;
        }
        if (leg_state[BL] == SKY)
        {
          //height_storage[BL] -= 0.4;
          height_storage[BL] = 0;
        }
        dog_s.leg_pfr.vec[2] = height_storage[FR];
        dog_s.leg_pbl.vec[2] = height_storage[BL];
        if (leg_state[FR] == GROUND || leg_state[BL] == GROUND || rpy_counter > flight_killer)
        {
          main_state = THR_A;
          atime_ground_old = millis();

          pos_counter = 0;
        }
        break;

      case THR_A://three legged phrase 1, pose changed
        pos_counter++;
        pos[0] += rpy_f_k[1] * (rpy_fi / rpy_counter);
        rpy[0] += rpy_f_k[0] * (rpy_fi_1 / rpy_counter);
        dog_s.rad_rpy[0] = rpy[0];
        dog_s.pos_offset[0] = pos[0];
        if (leg_state[FR] == SKY) {
          height_storage[BL] -= 0.4;
          height_storage[FR] -= 0.2;
        }
        else if (leg_state[BL] == SKY) {
          height_storage[FR] -= 0.4;
          height_storage[BL] -= 0.2;
        }
        dog_s.leg_pfr.vec[2] = height_storage[FR];
        dog_s.leg_pbl.vec[2] = height_storage[BL];

        if ((leg_state[FR] == GROUND && leg_state[BL] == GROUND) || pos_counter > pose_waiter)
          main_state = LOADING_B;

        break;

      case LOADING_B:
        //delay(50);

        main_state = COMPRESSION_B;
        break;
      case COMPRESSION_B:

        main_state = THRUST_B;
        break;
      case THRUST_B:

        height_counter = 0;

        main_state = UNLOADING_B;
        break;
      case UNLOADING_B:
        turn_angle = turn_vec * btime_ground_s / 2;
        turn_l_tx = turn_angle * turn_l * sin(turn_angle);
        turn_l_ty = turn_angle * turn_l * cos(turn_angle);

        dog_s.leg_pfl.vec[0] += tar_vec_x * atime_ground_s / 2 + turn_l_tx;
        dog_s.leg_pbr.vec[0] += tar_vec_x * atime_ground_s / 2 - turn_l_tx;

        dog_s.leg_pfl.vec[1] += tar_vec_y * atime_ground_s / 2 + turn_l_ty;
        dog_s.leg_pbr.vec[1] += tar_vec_y * atime_ground_s / 2 - turn_l_ty;

        dog_s.leg_pfl.vec[2] += trot_height;
        dog_s.leg_pbr.vec[2] += trot_height;

        height_storage[FL] = trot_height;  height_storage[FR] = 0;
        height_storage[BR] = trot_height;  height_storage[BL] = 0;

        height_counter++;

        if (leg_state[FL] == SKY && leg_state[BR] == SKY && height_counter > height_waiter)
        {
          main_state = FLIGHT_B;
          btime_ground = millis();
          btime_ground_s = (btime_ground - btime_ground_old) / 1000.0f;
          rpy_fi = 0;
          rpy_fi_1 = 0;
          rpy_counter = 0;
        }

        break;
      case FLIGHT_B:
        //float turn_l_t1 = (turn_angle * turn_l) / 1.414;
        rpy_fi += (rpy_read[1] - rpy_target[1]);
        rpy_fi_1 += (rpy_read[0] - rpy_target[0]);
        rpy_counter++;
        //delay(10);
        dog_s.leg_pfl.vec[0] += tar_vec_x * atime_ground_s / 2 + turn_l_tx ;
        dog_s.leg_pbr.vec[0] += tar_vec_x * atime_ground_s / 2 - turn_l_tx;

        dog_s.leg_pfl.vec[1] += tar_vec_y * atime_ground_s / 2 + turn_l_ty;
        dog_s.leg_pbr.vec[1] += tar_vec_y * atime_ground_s / 2 - turn_l_ty;

        dog_s.leg_pfr.vec[0] -= tar_vec_x * btime_ground_s / 2 ;
        dog_s.leg_pbl.vec[0] -= tar_vec_x * btime_ground_s / 2 ;

        dog_s.leg_pfr.vec[1] -= tar_vec_y * btime_ground_s / 2 ;
        dog_s.leg_pbl.vec[1] -= tar_vec_y * btime_ground_s / 2 ;


        if (leg_state[FL] == SKY)
        {
          //height_storage[FL] -= 0.4;
          height_storage[FL] = 0;
        }
        if (leg_state[BR] == SKY)
        {
          //height_storage[BR] -= 0.4;
          height_storage[BR] = 0;
        }
        dog_s.leg_pfl.vec[2] = height_storage[FL];
        dog_s.leg_pbr.vec[2] = height_storage[BR];
        if (leg_state[FL] == GROUND || leg_state[BR] == GROUND || rpy_counter > flight_killer)
        {
          main_state = THR_B;
          btime_ground_old = millis();

          pos_counter = 0;
        }
        break;

      case THR_B:
        pos_counter++;
        pos[0] += rpy_f_k[1] * (rpy_fi / rpy_counter);
        rpy[0] += rpy_f_k[0] * (rpy_fi_1 / rpy_counter);
        dog_s.rad_rpy[0] = rpy[0];
        dog_s.pos_offset[0] = pos[0];

        if (leg_state[FL] == SKY)  {
          height_storage[BR] -= 0.4;
          height_storage[FL] -= 0.2;
        }
        else if (leg_state[BR] == SKY) {
          height_storage[FL] -= 0.4;
          height_storage[BR] -= 0.2;
        }
        dog_s.leg_pfl.vec[2] = height_storage[FL];
        dog_s.leg_pbr.vec[2] = height_storage[BR];
        if ((leg_state[FL] == GROUND && leg_state[BR] == GROUND) || pos_counter > pose_waiter)
          main_state = LOADING_A;

        break;
    }

    dog_s.gen_body_mat();
    dog_s.update_leg_vec();
    now_time_old = now_time;
  }
}
