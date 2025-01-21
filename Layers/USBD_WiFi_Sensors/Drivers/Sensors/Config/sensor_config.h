/*
 * Copyright (c) 2022-2025 Arm Limited. All rights reserved.
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

#ifndef SENSOR_CONFIG_H
#define SENSOR_CONFIG_H

// Sensor Configuration

// Temperature Sensor
#define TEMPERATURE_SENSOR_ENABLED              1U
#define TEMPERATURE_SENSOR_SAMPLE_INTERVAL      1000000U

// Humidity
#define HUMIDITY_SENSOR_ENABLED                 1U
#define HUMIDITY_SENSOR_SAMPLE_INTERVAL         1000000U

// Pressure
#define PRESSURE_SENSOR_ENABLED                 1U
#define PRESSURE_SENSOR_SAMPLE_INTERVAL         25000U

// Accelerometer
#define ACCELEROMETER_ENABLED                   1U
#define ACCELEROMETER_SAMPLE_INTERVAL           600U
#define ACCELEROMETER_FIFO_SIZE                 1536U

// Gyroscope
#define GYROSCOPE_ENABLED                       1U
#define GYROSCOPE_SAMPLE_INTERVAL               600U
#define GYROSCOPE_FIFO_SIZE                     1536U

// Magnetometer
#define MAGNETOMETER_ENABLED                    1U
#define MAGNETOMETER_SAMPLE_INTERVAL            10000U

#endif /* SENSOR_CONFIG_H */
