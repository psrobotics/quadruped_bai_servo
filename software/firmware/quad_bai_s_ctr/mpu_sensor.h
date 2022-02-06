//MPU 6050 with kalman filter
//Modified from source - https://github.com/jjundot/MPU6050_Kalman
//Author jjundot

#ifndef _MPU_SENSOR__
#define _MPU_SENSOR__
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Arduino.h"

void mpu_init();
void mpu_update(float *rad_passer);

#endif
