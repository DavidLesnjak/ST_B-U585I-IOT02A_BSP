/*
 * Copyright (c) 2025 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SENSOR_B_U585I_IOT02A_H_
#define SENSOR_B_U585I_IOT02A_H_

#include "sensor_drv.h"
#include "sensor_config.h"

#ifdef  __cplusplus
extern  "C"
{
#endif

// Global driver structures

#if TEMPERATURE_SENSOR_ENABLED
extern DRIVER_SENSOR Driver_Sensor0;
#endif
#if HUMIDITY_SENSOR_ENABLED
extern DRIVER_SENSOR Driver_Sensor1;
#endif
#if PRESSURE_SENSOR_ENABLED
extern DRIVER_SENSOR Driver_Sensor2;
#endif
#if ACCELEROMETER_ENABLED
extern DRIVER_SENSOR Driver_Sensor3;
#endif
#if GYROSCOPE_ENABLED
extern DRIVER_SENSOR Driver_Sensor4;
#endif
#if MAGNETOMETER_ENABLED
extern DRIVER_SENSOR Driver_Sensor5;
#endif

#ifdef  __cplusplus
}
#endif

#endif  // SENSOR_B_U585I_IOT02A_H_
