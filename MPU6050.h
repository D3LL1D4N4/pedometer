#ifndef MPU6050_H_
#define MPU6050_H_

#include "stm32f4xx_hal.h"

#define MPU6050_ADDR         0xD0  // AD0 pini GND'ye bağlıysa

// MPU6050 register adresleri
#define WHO_AM_I_REG         0x75
#define PWR_MGMT_1_REG       0x6B
#define ACCEL_XOUT_H_REG     0x3B
#define ACCEL_YOUT_H_REG     0x3D
#define ACCEL_ZOUT_H_REG     0x3F
#define ACCEL_CONFIG_REG     0x1C
#define GYRO_CONFIG_REG      0x1B
#define SMPLRT_DIV_REG       0x19

// Fonksiyon prototipleri
HAL_StatusTypeDef MPU6050_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef MPU6050_Read_Accel(I2C_HandleTypeDef *hi2c, int16_t *ax, int16_t *ay, int16_t *az);

#endif /* MPU6050_H_ */
