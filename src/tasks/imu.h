#pragma once

#include "stm32f4xx.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "filters.h"

// Defines
#define WHO_AM_I_6050_ANS 0x68
#define WHO_AM_I_9250_ANS 0x71
#define WHO_AM_I          0x75
#define AD0_LOW           0x68
#define AD0_HIGH          0x69
#define GYRO_CONFIG       0x1B
#define ACCEL_CONFIG      0x1C
#define PWR_MGMT_1        0x6B
#define ACCEL_XOUT_H      0x3B
#define I2C_TIMOUT_MS     1000
#define USER_CTRL         0x6A
#define INT_PIN_CFG       0x37
#define INT_ENABLE        0x38

#define AK8963_WHO_AM_I  0x00 // should return 0x48
#define AK8963_INFO      0x01
#define AK8963_ST1       0x02  // data ready status bit 0
#define AK8963_XOUT_L	 0x03  // data
#define AK8963_XOUT_H	 0x04
#define AK8963_YOUT_L	 0x05
#define AK8963_YOUT_H	 0x06
#define AK8963_ZOUT_L	 0x07
#define AK8963_ZOUT_H	 0x08
#define AK8963_ST2       0x09  // Data overflow bit 3 and data read error status bit 2
#define AK8963_CNTL      0x0A  // Power down (0000), single-measurement (0001), self-test (1000) and Fuse ROM (1111) modes on bits 3:0
#define AK8963_ASTC      0x0C  // Self test control
#define AK8963_I2CDIS    0x0F  // I2C disable
#define AK8963_ASAX      0x10  // Fuse ROM x-axis sensitivity adjustment value
#define AK8963_ASAY      0x11  // Fuse ROM y-axis sensitivity adjustment value
#define AK8963_ASAZ      0x12  // Fuse ROM z-axis sensitivity adjustment value

struct imu_data_s
{
    float roll, pitch, yaw;
    int heading;
};

// Structs
struct RawData
{
    int16_t ax, ay, az, gx, gy, gz;
};

struct ProcessedData
{
    float ax, ay, az, gx, gy, gz;
};

struct GyroCal
{
    float x, y, z;
};

struct Attitude
{
    float r, p, y;
};

// Full scale ranges
enum gyroscopeFullScaleRange
{
    GFSR_250DPS,
    GFSR_500DPS,
    GFSR_1000DPS,
    GFSR_2000DPS
};

enum accelerometerFullScaleRange
{
    AFSR_2G,
    AFSR_4G,
    AFSR_8G,
    AFSR_16G
};

struct imu
{
	static void start();
	static void task_cb(void*);

    private:
        static constexpr double RAD2DEG = 57.2957795131;

        static constexpr uint8_t MPU9250_ADDR = (0x68 << 1);
        static constexpr uint8_t AK8963_ADDR = (0x0C << 1);
        static constexpr uint8_t WHOAMI_MPU9250 = 0x71;
        static constexpr uint8_t WHOAMI_MPU9255 = 0x73;
        static constexpr uint8_t WHOAMI_AK8963 = 0x48;

        static inline SemaphoreHandle_t mutex;

        static inline float mag_bias_factory[3] {0., 0., 0.};
        static inline float mag_bias[3] {0., 0., 0.};  // mag calibration value in MAG_OUTPUT_BITS: 16BITS
        static inline float mag_scale[3] {1., 1., 1.};
        static inline float m[3] {0.f, 0.f, 0.f};

        static void MPU9250_I2C_Read(uint8_t regAddr, uint8_t bytes, uint8_t *dest);
        static void MPU9250_I2C_Write(uint8_t regAddr, uint8_t bytes, uint8_t *data);
        static void AK8963_I2C_Read(uint8_t regAddr, uint8_t bytes, uint8_t *dest);
        static void AK8963_I2C_Write(uint8_t regAddr, uint8_t bytes, uint8_t *data);

        // Functions
        static void writeGyroFullScaleRange(uint8_t gFSR);
        static void writeAccFullScaleRange(uint8_t aFSR);

        // Variables
        static inline float aScaleFactor, gScaleFactor;
        static inline uint8_t _addr;

        // Default values
        static inline uint8_t _gFSR = GFSR_500DPS;
        static inline uint8_t _aFSR = AFSR_4G;
        static inline float _tau = 0.98;
        static inline float _dt = 0.015;

        // Structs
        static inline GyroCal gyroCal;
        static inline Attitude attitude;

        static inline ExponentialFilter2<float> g_x;
        static inline ExponentialFilter2<float> g_y;
        static inline ExponentialFilter2<float> g_z;

        static inline ExponentialFilter2<float> a_x;
        static inline ExponentialFilter2<float> a_y;
        static inline ExponentialFilter2<float> a_z;

        static inline ExponentialFilter2<float> m_x;
        static inline ExponentialFilter2<float> m_y;
        static inline ExponentialFilter2<float> m_z;

        static void calibrateGyro(uint16_t numCalPoints);
        static void setGyroFullScaleRange(uint8_t gFSR);
        static void setAccFullScaleRange(uint8_t aFSR);
        static void setDeltaTime(float dt);
        static RawData readRawData();
        static ProcessedData processData();
        static Attitude calcAttitude();
        static void setTau(float tau);

        static void readMag();

        public:
        static void getDataSafe(imu_data_s* data);
};

