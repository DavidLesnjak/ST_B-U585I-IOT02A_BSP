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

#ifndef SENSOR_DRV_H
#define SENSOR_DRV_H

#ifdef  __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define _Driver_SENSOR(n)   Driver_SENSOR##n
#define  Driver_SENSOR(n)   _Driver_SENSOR_(n)

// ==== Sensor Driver Interface ====

/// Configuration
typedef const struct {
  char    *name;                            ///< Sensor name
  uint32_t dma_mode;                        ///< DMA mode: 1=DMA, 0=non-DMA (FIFO)
  uint32_t sample_size;                     ///< Sample size in bytes
  union {
    struct {
      uint32_t sample_interval;             ///< Sample interval in microseconds
      uint32_t fifo_size;                   ///< Sample FIFO size in bytes
      uint32_t data_threshold;              ///< Data event threshold in number of samples
    } fifo;
    struct {
      uint32_t block_interval;              ///< Block interval in microseconds
      uint32_t block_size;                  ///< Block size in bytes
      uint32_t block_num;                   ///< Number of blocks
    } dma;
  } u;
} sensorConfig_t;

/// Status
typedef struct {
  uint32_t active   :  1;                   ///< Active state: 1=active(enabled), 0=inactive(disabled)
  uint32_t overflow :  1;                   ///< Overflow flag (cleared on \ref sensorGetStatus)
  uint32_t reserved : 30;
} sensorStatus_t;

/// Function return codes
#define SENSOR_OK               (0)         ///< Operation completed successfully
#define SENSOR_ERROR            (-1)        ///< Operation failed

/// Events
#define SENSOR_EVENT_DATA       (1UL << 0)  ///< Data available
#define SENSOR_EVENT_OVERFLOW   (1UL << 1)  ///< Overflow detected

/// Event callback function
typedef void (*sensorEvent_t) (uint32_t event);

/**
  \fn          sensorConfig_t *sensorGetConfig (void)
  \brief       Get sensor configuration.
  \return      pointer to \ref sensorConfig_t
*/
sensorConfig_t *sensorGetConfig (void);

/**
  \fn          int32_t sensorRegisterEvents (sensorEvent_t event_cb, uint32_t event_mask)
  \brief       Register sensor events.
  \param[in]   event_cb    pointer to \ref sensorEvent_t
  \param[in]   event_mask  event mask
  \return      return code
*/
int32_t sensorRegisterEvents (sensorEvent_t event_cb, uint32_t event_mask);

/**
  \fn          int32_t sensorEnable (void)
  \brief       Enable sensor.
  \return      return code
*/
int32_t sensorEnable (void);

/**
  \fn          int32_t sensorDisable (void)
  \brief       Disable sensor.
  \return      return code
*/
int32_t sensorDisable (void);

/**
  \fn          sensorStatus_t sensorGetStatus (void)
  \brief       Get sensor status.
  \return      \ref sensorStatus_t
*/
sensorStatus_t sensorGetStatus (void);

/**
  \fn          uint32_t sensorReadSamples (uint32_t num_samples, void *buf, uint32_t buf_size)
  \brief       Read samples from sensor.
  \param[in]   num_samples maximum number of samples to read
  \param[out]  buf         pointer to buffer for samples
  \param[in]   buf_size    buffer size in bytes
  \return      number of samples read
*/
uint32_t sensorReadSamples (uint32_t num_samples, void *buf, uint32_t buf_size);

/**
  \fn          void *sensorGetBlockData (void)
  \brief       Get block data.
  \return      pointer to block data
*/
void *sensorGetBlockData (void);

/**
  \brief Access structure of the Sensor Driver.
*/
typedef struct _DRIVER_SENSOR {
  sensorConfig_t *  (*GetConfig)      (void);
  int32_t           (*RegisterEvents) (sensorEvent_t event_cb, uint32_t event_mask);         ///< Pointer to \ref sensorRegisterEvents :  Register sensor events.
  int32_t           (*Enable)         (void);                                                ///< Pointer to \ref sensorEnable :  Enable sensor.
  int32_t           (*Disable)        (void);                                                ///< Pointer to \ref sensorDisable :  Disable sensor.
  sensorStatus_t    (*GetStatus)      (void);                                                ///< Pointer to \ref sensorGetStatus : Disable sensor.
  uint32_t          (*ReadSamples)    (uint32_t num_samples, void *buf, uint32_t buf_size);  ///< Pointer to \ref sensorReadSamples : Read samples from sensor.
  void *            (*GetBlockData)   (void);                                                ///< Pointer to \ref sensorGetBlockData : Get block data.
} const DRIVER_SENSOR;

#ifdef  __cplusplus
}
#endif

#endif  /* SENSOR_DRV_H */
