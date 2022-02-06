//Quad controller for atmega328p
//Shuang Peng - 2019-02 ver0.2

#include "matrix.h"
#include "kinematics.h"
#include "gait_generator.h"
#include "mpu_sensor.h"
#include <Servo.h>
#include <FlexiTimer2.h>

#define steady_pos_rpy_control 1
#define mpu_auto_balance 2
#define gait_control 3

Servo *servo_arr = new Servo[12];
int servo_pin[12] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
int offset_arr[12] = {98, 166, 145, 68, 147, 84, 105, 28, 68, 72, -5, 110};//servo offset calibration
int init_arr[12] = {0, 135, 90, 0, 135, 90, 0, 135, 90, 0, 135, 90};//init servo angle
int write_arr[12];
int read_arr[12];

int leg_sensor_arr[4] = {0, 0, 0, 0};

float rpy[3] = {0, 0, 0};
float rpy_target[3] = { -0.02, 0, 0};
float rpy_limit[3] = {0.9, 0.6, 0.5};

float rpy_read[3];
float rpy_read_old[3];
float rpy_delta[3];

float rpy_k[3] = { -1.9, -1.9, 0};
float rpy_d[3] = { -0.055, -0.055, 0};

float rpy_f_k[3] = { 0, 0.4, 0};

float pos[3] = {2.3, 0, 9.5};
float leg_len[3] = {3, 6, 7.7};
//dog.body_ab_len = 3; dog.body_hip_len = 9;
//dog.foot_ab_len = 8; dog.foot_hip_len = 9;
body dog(3, 9, 12, 10, rpy, pos, leg_len);

float temp_v = 0;
int dir = 1;

float data_received[4] = {0}; //received control data

int overall_state;//general state

gait_generator generator(dog);

void setup() {
  Serial.begin(115200);

  generator.trot_delay = 70;
  generator.trot_height = 4.5;//set up generator;

  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);

  for (int s = 0; s < 12; s++)
    servo_arr[s].attach(servo_pin[s]);
  for (int s = 0; s < 12; s++)
    read_arr[s] = init_arr[s];

  mpu_init();
  FlexiTimer2::set(150, 1.0 / 1000, serial_read); // call every 150 1ms "ticks" to the serial reader
  FlexiTimer2::start();
}

void init_pos()
{
  servo_arr[0].write(98);//add clockwise FL
  servo_arr[1].write(170);//add clockwise
  servo_arr[2].write(142);//add clockwise

  servo_arr[3].write(68);//minus clockwise BL
  servo_arr[4].write(150);
  servo_arr[5].write(88);

  servo_arr[6].write(107);//add BR
  servo_arr[7].write(52);//minus
  servo_arr[8].write(68);//minus

  servo_arr[9].write(71);//minus
  servo_arr[10].write(0);//minus
  servo_arr[11].write(111);
}

void angle_trans(int *offset, int *read_arr, int *write_arr)
{
  write_arr[1] = read_arr[1] - 180 + offset[1];
  write_arr[4] = read_arr[4] - 180 + offset[4];

  write_arr[2] = read_arr[2] - 90 + offset[2];
  write_arr[5] = read_arr[5] - 90 + offset[5];

  write_arr[7] = 180 - read_arr[7] + offset[7];
  write_arr[10] = 180 - read_arr[10] + offset[10];

  write_arr[8] = 90 - read_arr[8] + offset[8];
  write_arr[11] = 90 - read_arr[11] + offset[11];

  write_arr[0] = offset[0] + read_arr[0];
  write_arr[3] = offset[3] - read_arr[3];
  write_arr[6] = offset[6] - (read_arr[6] + 180);
  write_arr[9] = offset[9] + read_arr[9] + 180;
}


void write_servo(int *data_write, int *offset)
{
  for (int s = 0; s < 12; s++)
  {
    servo_arr[s].write(data_write[s]);
  }
}

void print_leg_vec(body &in)
{
  Serial.print("fl: ");
  for (int s = 0; s < 3; s++) {
    Serial.print(in.leg_vec_fl.vec[s]);  Serial.print(" ");
  }
  Serial.println(" ");
  Serial.print("fr: ");
  for (int s = 0; s < 3; s++) {
    Serial.print(in.leg_vec_fr.vec[s]);  Serial.print(" ");
  }
  Serial.println(" ");
  Serial.print("bl: ");
  for (int s = 0; s < 3; s++) {
    Serial.print(in.leg_vec_bl.vec[s]);  Serial.print(" ");
  }
  Serial.println(" ");
  Serial.print("br: ");
  for (int s = 0; s < 3; s++) {
    Serial.print(in.leg_vec_br.vec[s]);  Serial.print(" ");
  }
  Serial.println(" ");

}

void print_angle(body &in)
{
  Serial.print("fl: ");
  Serial.print(in.fl.ab);  Serial.print(" ");
  Serial.print(in.fl.hip);  Serial.print(" ");
  Serial.print(in.fl.knee);  Serial.println(" ");
  Serial.print("fr: ");
  Serial.print(in.fr.ab);  Serial.print(" ");
  Serial.print(in.fr.hip);  Serial.print(" ");
  Serial.print(in.fr.knee);  Serial.println(" ");
  Serial.print("bl: ");
  Serial.print(in.bl.ab);  Serial.print(" ");
  Serial.print(in.bl.hip);  Serial.print(" ");
  Serial.print(in.bl.knee);  Serial.println(" ");
  Serial.print("br: ");
  Serial.print(in.br.ab);  Serial.print(" ");
  Serial.print(in.br.hip);  Serial.print(" ");
  Serial.print(in.br.knee);  Serial.println(" ");
}

void print_angle_write(int *arr)
{
  Serial.print("set: ");
  for (int s = 0; s < 12; s++)
  {
    Serial.print(arr[s]);
    Serial.print(" ");
  }
  Serial.println(" ");
}

void read_leg_sensor(int *arr)
{
  arr[0] = digitalRead(A0);
  arr[1] = digitalRead(A1);
  arr[2] = digitalRead(A2);
  arr[3] = digitalRead(A3);
}

void print_mpu_data(float *rpy_read)
{
  Serial.print(rpy_read[0]); Serial.print(",");
  Serial.print(rpy_read[1]); Serial.print(",");
  Serial.print(rpy_read[2]); Serial.println();
}

void serial_read()
{
  if (Serial.available())
  {
    if (Serial.read() == 'h')
      for (int i = 0; i < 4; i++)
        *(data_received + i) = Serial.parseFloat();
  }
  if (int(data_received[0]) != overall_state)
    overall_state = data_received[0];//change main state
}

void loop() {

  read_leg_sensor(leg_sensor_arr);
  mpu_update(rpy_read);
  //serial_read();

  //if (!(leg_sensor_arr[0] == SKY && leg_sensor_arr[1] == SKY && leg_sensor_arr[2] == SKY && leg_sensor_arr[3] == SKY))
  /*for (int s = 0; s < 2; s++)
    {
    rpy_delta[s] = rpy_read[s] - rpy_target[s];
    rpy[s] += (rpy_k[s] * rpy_delta[s] * abs(rpy_delta[s]) + rpy_d[s] * (rpy_read[s] - rpy_read_old[s]));

    rpy_read_old[s] = rpy_read[s];

    if (rpy[s] > rpy_limit[s]) rpy[s] = rpy_limit[s];
    else if (rpy[s] < -1 * rpy_limit[s]) rpy[s] = -1 * rpy_limit[s];
    }*/

  //dog.change_dog_pos(rpy, pos);
  //Serial.print("time interval a  ");
  //Serial.print(generator.atime_ground_s);
  //Serial.print("   time interval b  ");
  //Serial.println(generator.btime_ground_s);

  if (generator.main_state == -1)
  {
    if (!(leg_sensor_arr[0] == SKY && leg_sensor_arr[1] == SKY && leg_sensor_arr[2] == SKY && leg_sensor_arr[3] == SKY))
      generator.main_state = LOADING_A;
  }

  float vec_x = data_received[1];
  float vec_y = data_received[2];
  float vec_turn = data_received[3];
  generator.trot_changer(dog, leg_sensor_arr, rpy, pos, vec_x, vec_y, vec_turn, rpy_read, rpy_target, rpy_f_k);

  read_arr[0] = dog.fl.ab;   read_arr[1] = dog.fl.hip;    read_arr[2] = dog.fl.knee;
  read_arr[3] = dog.bl.ab;   read_arr[4] = dog.bl.hip;    read_arr[5] = dog.bl.knee;
  read_arr[6] = dog.br.ab;   read_arr[7] = dog.br.hip;    read_arr[8] = dog.br.knee;
  read_arr[9] = dog.fr.ab;   read_arr[10] = dog.fr.hip;    read_arr[11] = dog.fr.knee;

  angle_trans(offset_arr, read_arr, write_arr);

  write_servo(write_arr, offset_arr);

  //Serial.print("state ");
  //Serial.println(generator.main_state);

  //fl,bl,br,fr cycle
}
