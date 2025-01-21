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

// Sensor Driver

#include "sensor_B-U585I-IOT02A.h"

#include "b_u585i_iot02a_env_sensors.h"
#include "b_u585i_iot02a_motion_sensors.h"

#include "ism330dhcx_fifo.h"

#ifndef SENSOR_NO_LOCK
#include "cmsis_os2.h"


// Compile-time configuration
// Macro for defining driver structures (for instances)
#define SENSOR_DRIVER(n, sensor)        \
DRIVER_SENSOR Driver_Sensor##n = {      \
  sensor##GetConfig,                    \
  sensor##RegisterEvents,               \
  sensor##Enable,                       \
  sensor##Disable,                      \
  sensor##GetStatus,                    \
  sensor##ReadSamples,                  \
  NULL                                  \
};

#if ((ACCELEROMETER_ENABLED != 0U) || (GYROSCOPE_ENABLED != 0U))
static uint8_t ISM330DHCX_ActiveFlags = 0U;

extern ISM330DHCX_Object_t ISM330DHCX_Obj;
#endif

// Mutex lock
static osMutexId_t lock_id  = NULL;
static uint32_t    lock_cnt = 0U;

static inline void sensorLockCreate (void) {
  if (lock_cnt == 0U) {
    lock_id = osMutexNew(NULL);
  }
  lock_cnt++;
}
static inline void sensorLockDelete (void) {
  if (lock_cnt != 0U) {
    lock_cnt--;
    if (lock_cnt == 0U) {
      osMutexDelete(lock_id);
    }
  }
}
static inline void sensorLock (void) {
  osMutexAcquire(lock_id, osWaitForever);
}
static inline void sensorUnLock (void) {
  osMutexRelease(lock_id);
}
#else
static inline void sensorLockCreate (void) {}
static inline void sensorLockDelete (void) {}
static inline void sensorLock       (void) {}
static inline void sensorUnLock     (void) {}
#endif

#if TEMPERATURE_SENSOR_ENABLED
// Sensor0: Temperature Sensor
// Configuration
static sensorConfig_t TemperatureSensorConfig = {
  "Temperature",
  0U,
  4U,
  {
    TEMPERATURE_SENSOR_SAMPLE_INTERVAL,
    4U,
    0U
   }
};

static uint8_t TemperatureSensorActiveFlag = 0U;

// Get sensor configuration
sensorConfig_t * TemperatureSensorGetConfig (void) {
  return &TemperatureSensorConfig;
}

// Register sensor events
static int32_t TemperatureSensorRegisterEvents (sensorEvent_t event_cb, uint32_t event_mask) {
  int32_t ret = SENSOR_ERROR;

  if ((event_cb == NULL) || (event_mask == 0U)) {
    ret = SENSOR_OK;
  }
  return ret;
}

// Enable sensor
static int32_t TemperatureSensorEnable (void) {
  int32_t ret = SENSOR_ERROR;
  float   value;

  if (TemperatureSensorActiveFlag == 0U) {
    sensorLockCreate();
    sensorLock();
    if (BSP_ENV_SENSOR_Enable(0, ENV_TEMPERATURE) == BSP_ERROR_NONE) {
      BSP_ENV_SENSOR_GetValue(0, ENV_TEMPERATURE, &value);
      TemperatureSensorActiveFlag = 1U;
      ret = SENSOR_OK;
    }
    sensorUnLock();
  }
  return ret;
}

// Disable sensor
static int32_t TemperatureSensorDisable (void) {
  int32_t ret = SENSOR_ERROR;

  if (TemperatureSensorActiveFlag == 1U) {
    sensorLock();
    if (BSP_ENV_SENSOR_Disable(0, ENV_TEMPERATURE) == BSP_ERROR_NONE) {
      TemperatureSensorActiveFlag = 0U;
      ret = SENSOR_OK;
    }
    sensorUnLock();
    sensorLockDelete();
  }
  return ret;
}

// Get sensor status
static sensorStatus_t TemperatureSensorGetStatus (void) {
  sensorStatus_t stat = {0U, 0U, 0U};

  stat.active   = TemperatureSensorActiveFlag;
  stat.overflow = 0U;

  return stat;
}

// Read samples from sensor
static uint32_t TemperatureSensorReadSamples (uint32_t num_samples, void *buf, uint32_t buf_size) {
  uint32_t num = 0U;
  int32_t  ret;
  uint8_t  stat;
  float    value;

  if ((num_samples != 0U) && (buf != NULL) &&
      (buf_size >= (num_samples * TemperatureSensorConfig.sample_size))) {
    sensorLock();
    ret = HTS221_TEMP_Get_DRDY_Status(Env_Sensor_CompObj[0], &stat);
    if ((ret == 0) && (stat != 0U)) {
      if (BSP_ENV_SENSOR_GetValue(0, ENV_TEMPERATURE, &value) == BSP_ERROR_NONE) {
        memcpy(buf, &value, sizeof(float));
        num = 1U;
      }
    }
    sensorUnLock();
  }
  return num;
}
#endif

#if HUMIDITY_SENSOR_ENABLED
// Sensor1: Humidity Sensor
// Configuration
static sensorConfig_t HumiditySensorConfig = {
  "Humidity",
  0U,
  4U,
  {
    HUMIDITY_SENSOR_SAMPLE_INTERVAL,
    4U,
    0U
   }
};

static uint8_t HumiditySensorActiveFlag = 0U;

// Get sensor configuration
sensorConfig_t * HumiditySensorGetConfig (void) {
  return &HumiditySensorConfig;
}

// Register sensor events
static int32_t HumiditySensorRegisterEvents (sensorEvent_t event_cb, uint32_t event_mask) {
  int32_t ret = SENSOR_ERROR;

  if ((event_cb == NULL) || (event_mask == 0U)) {
    ret = SENSOR_OK;
  }
  return ret;
}

// Enable sensor
static int32_t HumiditySensorEnable (void) {
  int32_t ret = SENSOR_ERROR;
  float   value;

  if (HumiditySensorActiveFlag == 0U) {
    sensorLockCreate();
    sensorLock();
    if (BSP_ENV_SENSOR_Enable(0, ENV_HUMIDITY) == BSP_ERROR_NONE) {
      BSP_ENV_SENSOR_GetValue(0, ENV_HUMIDITY, &value);
      HumiditySensorActiveFlag = 1U;
      ret = SENSOR_OK;
    }
    sensorUnLock();
  }
  return ret;
}

// Disable sensor
static int32_t HumiditySensorDisable (void) {
  int32_t ret = SENSOR_ERROR;

  if (HumiditySensorActiveFlag == 1U) {
    sensorLock();
    if (BSP_ENV_SENSOR_Disable(0, ENV_HUMIDITY) == BSP_ERROR_NONE) {
      HumiditySensorActiveFlag = 0U;
      ret = SENSOR_OK;
    }
    sensorUnLock();
    sensorLockDelete();
  }
  return ret;
}

// Get sensor status
static sensorStatus_t HumiditySensorGetStatus (void) {
  sensorStatus_t stat = {0U, 0U, 0U};

  stat.active   = HumiditySensorActiveFlag;
  stat.overflow = 0U;

  return stat;
}

// Read samples from sensor
static uint32_t HumiditySensorReadSamples (uint32_t num_samples, void *buf, uint32_t buf_size) {
  uint32_t num = 0U;
  int32_t  ret;
  uint8_t  stat;
  float    value;

  if ((num_samples != 0U) && (buf != NULL) &&
      (buf_size >= (num_samples * HumiditySensorConfig.sample_size))) {
    sensorLock();
    ret = HTS221_HUM_Get_DRDY_Status(Env_Sensor_CompObj[0], &stat);
    if ((ret == 0) && (stat != 0U)) {
      if (BSP_ENV_SENSOR_GetValue(0, ENV_HUMIDITY, &value) == BSP_ERROR_NONE) {
        memcpy(buf, &value, sizeof(float));
        num = 1U;
      }
    }
    sensorUnLock();
  }
  return num;
}
#endif

#if PRESSURE_SENSOR_ENABLED
// Sensor2: Pressure Sensor
// Configuration
static sensorConfig_t PressureSensorConfig = {
  "Pressure",
  0U,
  4U,
  {
    PRESSURE_SENSOR_SAMPLE_INTERVAL,
    4U,
    0U
   }
};

static uint8_t PressureSensorActiveFlag = 0U;

// Get sensor configuration
sensorConfig_t * PressureSensorGetConfig (void) {
  return &PressureSensorConfig;
}

// Register sensor events
static int32_t PressureSensorRegisterEvents (sensorEvent_t event_cb, uint32_t event_mask) {
  int32_t ret = SENSOR_ERROR;

  if ((event_cb == NULL) || (event_mask == 0U)) {
    ret = SENSOR_OK;
  }
  return ret;
}

// Enable sensor
static int32_t PressureSensorEnable (void) {
  int32_t ret = SENSOR_ERROR;

  if (PressureSensorActiveFlag == 0U) {
    sensorLockCreate();
    sensorLock();
    if (BSP_ENV_SENSOR_Enable(1, ENV_PRESSURE) == BSP_ERROR_NONE) {
      PressureSensorActiveFlag = 1U;
      ret = SENSOR_OK;
    }
    sensorUnLock();
  }
  return ret;
}

// Disable sensor
static int32_t PressureSensorDisable (void) {
  int32_t ret = SENSOR_ERROR;

  if (PressureSensorActiveFlag == 1U) {
    sensorLock();
    if (BSP_ENV_SENSOR_Disable(1, ENV_PRESSURE) == BSP_ERROR_NONE) {
      PressureSensorActiveFlag = 0U;
      ret = SENSOR_OK;
    }
    sensorUnLock();
    sensorLockDelete();
  }
  return ret;
}

// Get sensor status
static sensorStatus_t PressureSensorGetStatus (void) {
  sensorStatus_t stat = {0U, 0U, 0U};

  stat.active   = PressureSensorActiveFlag;
  stat.overflow = 0U;

  return stat;
}

// Read samples from sensor
static uint32_t PressureSensorReadSamples (uint32_t num_samples, void *buf, uint32_t buf_size) {
  uint32_t num = 0U;
  int32_t  ret;
  uint8_t  stat;
  float    value;

  if ((num_samples != 0U) && (buf != NULL) &&
      (buf_size >= (num_samples * PressureSensorConfig.sample_size))) {
    sensorLock();
    ret = LPS22HH_PRESS_Get_DRDY_Status(Env_Sensor_CompObj[1], &stat);
    if ((ret == 0) && (stat != 0U)) {
      if (BSP_ENV_SENSOR_GetValue(1, ENV_PRESSURE, &value) == BSP_ERROR_NONE) {
        memcpy(buf, &value, sizeof(float));
        num = 1U;
      }
    }
    sensorUnLock();
  }
  return num;
}
#endif

#if ACCELEROMETER_ENABLED
// Sensor3: ACCELEROMETER
// Configuration
static sensorConfig_t AccelerometerConfig = {
  "Accelerometer",
  0U,
  6U,
  {
    ACCELEROMETER_SAMPLE_INTERVAL,
    ACCELEROMETER_FIFO_SIZE,
    0U
   }
};

// Get sensor configuration
sensorConfig_t * AccelerometerGetConfig (void) {
  return &AccelerometerConfig;
}

// Register sensor events
static int32_t AccelerometerRegisterEvents (sensorEvent_t event_cb, uint32_t event_mask) {
  int32_t ret = SENSOR_ERROR;

  if ((event_cb == NULL) || (event_mask == 0U)) {
    ret = SENSOR_OK;
  }
  return ret;
}

// Enable sensor
static int32_t AccelerometerEnable (void) {
  uint8_t sample[6];
  int32_t ret = SENSOR_ERROR;

  if ((ISM330DHCX_ActiveFlags & (1U << ISM330DHCX_ID_ACCELEROMETER)) == 0U) {
    sensorLockCreate();
    sensorLock();
    if (ISM330DHCX_FIFO_Init(ISM330DHCX_ID_ACCELEROMETER) == 0) {
      if (ISM330DHCX_ActiveFlags == 0U) {
        // Clear ISM330DHCX FIFO
        while (ISM330DHCX_FIFO_Read(ISM330DHCX_ID_ACCELEROMETER, 1, sample) != 0U);
      }
      if (ISM330DHCX_ACC_Enable(&ISM330DHCX_Obj) == 0) {
        ISM330DHCX_ActiveFlags |= (1U << ISM330DHCX_ID_ACCELEROMETER);
        ret = SENSOR_OK;
      }
    }
    sensorUnLock();
  }
  return ret;
}

// Disable sensor
static int32_t AccelerometerDisable (void) {
  int32_t ret = SENSOR_ERROR;

  if ((ISM330DHCX_ActiveFlags & (1U << ISM330DHCX_ID_ACCELEROMETER)) == 1U) {
    sensorLock();
    if (ISM330DHCX_ACC_Disable(&ISM330DHCX_Obj) == 0) {
      if (ISM330DHCX_FIFO_Uninit(ISM330DHCX_ID_ACCELEROMETER) == 0) {
        ISM330DHCX_ActiveFlags &= ~(1U << ISM330DHCX_ID_ACCELEROMETER);
        ret = SENSOR_OK;
      }
    }
    sensorUnLock();
    sensorLockDelete();
  }
  return ret;
}

// Get sensor status
static sensorStatus_t AccelerometerGetStatus (void) {
  sensorStatus_t stat = {0U, 0U, 0U};

  stat.active   = (ISM330DHCX_ActiveFlags >> ISM330DHCX_ID_ACCELEROMETER) & 1U;
  stat.overflow = 0U;

  return stat;
}

// Read samples from sensor
static uint32_t AccelerometerReadSamples (uint32_t num_samples, void *buf, uint32_t buf_size) {
  uint32_t num = 0U;

  if ((num_samples != 0U) && (buf != NULL) &&
      (buf_size >= (num_samples * AccelerometerConfig.sample_size))) {
    sensorLock();
    num = ISM330DHCX_FIFO_Read(ISM330DHCX_ID_ACCELEROMETER, num_samples, buf);
    sensorUnLock();
  }
  return num;
}
#endif

#if GYROSCOPE_ENABLED
// Sensor4: Gyroscope
// Configuration
static sensorConfig_t GyroscopeConfig = {
  "Gyroscope",
  0U,
  6U,
  {
    GYROSCOPE_SAMPLE_INTERVAL,
    GYROSCOPE_FIFO_SIZE,
    0U
   }
};

// Get sensor configuration
sensorConfig_t * GyroscopeGetConfig (void) {
  return &GyroscopeConfig;
}

// Register sensor events
static int32_t GyroscopeRegisterEvents (sensorEvent_t event_cb, uint32_t event_mask) {
  int32_t ret = SENSOR_ERROR;

  if ((event_cb == NULL) || (event_mask == 0U)) {
    ret = SENSOR_OK;
  }
  return ret;
}

// Enable sensor
static int32_t GyroscopeEnable (void) {
  uint8_t sample[6];
  int32_t ret = SENSOR_ERROR;

  if ((ISM330DHCX_ActiveFlags & (1U << ISM330DHCX_ID_GYROSCOPE)) == 0U) {
    sensorLockCreate();
    sensorLock();
    if (ISM330DHCX_FIFO_Init(ISM330DHCX_ID_GYROSCOPE) == 0) {
      if (ISM330DHCX_ActiveFlags == 0U) {
        // Clear ISM330DHCX FIFO
        while (ISM330DHCX_FIFO_Read(ISM330DHCX_ID_GYROSCOPE, 1, sample) != 0U);
      }
      if (ISM330DHCX_GYRO_Enable(&ISM330DHCX_Obj) == 0) {
        ISM330DHCX_ActiveFlags |= (1U << ISM330DHCX_ID_GYROSCOPE);
        ret = SENSOR_OK;
      }
    }
    sensorUnLock();
  }
  return ret;
}

// Disable sensor
static int32_t GyroscopeDisable (void) {
  int32_t ret = SENSOR_ERROR;

  if ((ISM330DHCX_ActiveFlags & (1U << ISM330DHCX_ID_GYROSCOPE)) == 1U) {
    sensorLock();
    if (BSP_MOTION_SENSOR_Disable(0, MOTION_GYRO) == BSP_ERROR_NONE) {
      if (ISM330DHCX_FIFO_Uninit(ISM330DHCX_ID_GYROSCOPE) == 0) {
        ISM330DHCX_ActiveFlags &= ~(1U << ISM330DHCX_ID_GYROSCOPE);
        ret = SENSOR_OK;
      }
    }
    sensorUnLock();
    sensorLockDelete();
  }
  return ret;
}

// Get sensor status
static sensorStatus_t GyroscopeGetStatus (void) {
  sensorStatus_t stat = {0U, 0U, 0U};

  stat.active   = (ISM330DHCX_ActiveFlags >> ISM330DHCX_ID_GYROSCOPE) & 1U;
  stat.overflow = 0U;

  return stat;
}

// Read samples from sensor
static uint32_t GyroscopeReadSamples (uint32_t num_samples, void *buf, uint32_t buf_size) {
  uint32_t num = 0U;

  if ((num_samples != 0U) && (buf != NULL) &&
      (buf_size >= (num_samples * GyroscopeConfig.sample_size))) {
    sensorLock();
    num = ISM330DHCX_FIFO_Read(ISM330DHCX_ID_GYROSCOPE, num_samples, buf);
    sensorUnLock();
  }
  return num;
}
#endif

#if MAGNETOMETER_ENABLED
// Sensor5: Magnetometer
// Configuration
static sensorConfig_t MagnetometerConfig = {
  "Magnetometer",
  0U,
  6U,
  {
    MAGNETOMETER_SAMPLE_INTERVAL,
    6U,
    0U
   }
};

static uint8_t MagnetometerActiveFlag = 0U;

// Get sensor configuration
sensorConfig_t * MagnetometerGetConfig (void) {
  return &MagnetometerConfig;
}

// Register sensor events
static int32_t MagnetometerRegisterEvents (sensorEvent_t event_cb, uint32_t event_mask) {
  int32_t ret = SENSOR_ERROR;

  if ((event_cb == NULL) || (event_mask == 0U)) {
    ret = SENSOR_OK;
  }
  return ret;
}

// Enable sensor
static int32_t MagnetometerEnable (void) {
  int32_t ret = SENSOR_ERROR;

  if (MagnetometerActiveFlag == 0U) {
    sensorLockCreate();
    sensorLock();
    if (BSP_MOTION_SENSOR_Enable(1, MOTION_MAGNETO) == BSP_ERROR_NONE) {
      MagnetometerActiveFlag = 1U;
      ret = SENSOR_OK;
    }
    sensorUnLock();
  }
  return ret;
}

// Disable sensor
static int32_t MagnetometerDisable (void) {
  int32_t ret = SENSOR_ERROR;

  if (MagnetometerActiveFlag == 1U) {
    sensorLock();
    if (BSP_ENV_SENSOR_Disable(0, ENV_TEMPERATURE) == BSP_ERROR_NONE) {
      MagnetometerActiveFlag = 0U;
      ret = SENSOR_OK;
    }
    sensorUnLock();
    sensorLockDelete();
  }
  return ret;
}

// Get sensor status
static sensorStatus_t MagnetometerGetStatus (void) {
  sensorStatus_t stat = {0U, 0U, 0U};

  stat.active   = MagnetometerActiveFlag;
  stat.overflow = 0U;

  return stat;
}

// Read samples from sensor
static uint32_t MagnetometerReadSamples (uint32_t num_samples, void *buf, uint32_t buf_size) {
  uint32_t num = 0U;
  int32_t  ret;
  uint8_t  stat;
  BSP_MOTION_SENSOR_AxesRaw_t axes;

  if ((num_samples != 0U) && (buf != NULL) &&
      (buf_size >= (num_samples * MagnetometerConfig.sample_size))) {
    sensorLock();
    ret = IIS2MDC_MAG_Get_DRDY_Status(Motion_Sensor_CompObj[1], &stat);
    if ((ret == 0) && (stat != 0U)) {
      if (BSP_MOTION_SENSOR_GetAxesRaw(1, MOTION_MAGNETO, &axes) == BSP_ERROR_NONE) {
        memcpy(buf, &axes, sizeof(BSP_MOTION_SENSOR_AxesRaw_t));
        num = 1U;
      }
    }
    sensorUnLock();
  }
  return num;
}
#endif

// Global driver structures
#if TEMPERATURE_SENSOR_ENABLED
SENSOR_DRIVER(0, TemperatureSensor)
#endif
#if HUMIDITY_SENSOR_ENABLED
SENSOR_DRIVER(1, HumiditySensor)
#endif
#if PRESSURE_SENSOR_ENABLED
SENSOR_DRIVER(2, PressureSensor)
#endif
#if ACCELEROMETER_ENABLED
SENSOR_DRIVER(3, Accelerometer)
#endif
#if GYROSCOPE_ENABLED
SENSOR_DRIVER(4, Gyroscope)
#endif
#if MAGNETOMETER_ENABLED
SENSOR_DRIVER(5, Magnetometer)
#endif
