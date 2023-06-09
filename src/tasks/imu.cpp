#include "imu.h"
#include <cstdio>
#include <cmath>

#define IMU_TASK_PRIO     4
#define IMU_STK_SIZE      4*configMINIMAL_STACK_SIZE
static TaskHandle_t ImuTask_Handler;
extern "C" SemaphoreHandle_t i2c_mutex;
extern "C" I2C_HandleTypeDef I2cHandle;

void imu::start()
{
    // Initialize variables
    uint8_t check, select;

    // Set attitude to zero conditions
    attitude.r = 0;
    attitude.p = 0;
    attitude.y = 0;

    // Confirm device
    MPU9250_I2C_Read(WHO_AM_I, 1, &check);

    // TODO: If 9250 or 6050 fails could it trigger the opposite check???
    if ((check == WHO_AM_I_9250_ANS) || (check == WHO_AM_I_6050_ANS))
    {
        // reset the sensor
        select = 0x80;
        MPU9250_I2C_Write(PWR_MGMT_1, 1, &select);
        HAL_Delay(100);

        // Startup / reset the sensor
        select = 0x00;
        MPU9250_I2C_Write(PWR_MGMT_1, 1, &select);
        HAL_Delay(100);

        // get stable time source
        select = 0x01;
        MPU9250_I2C_Write(PWR_MGMT_1, 1, &select);
        HAL_Delay(100);

        // Set the full scale ranges
        writeAccFullScaleRange(_aFSR);
        writeGyroFullScaleRange(_gFSR);
    }
    else
    while(1)
    {

    }

    check = 0x22;

    MPU9250_I2C_Write(INT_PIN_CFG, 1, &check);

    check = 0x01;

    MPU9250_I2C_Write(INT_ENABLE, 1, &check); //INT_ENABLE

    HAL_Delay(100);

    AK8963_I2C_Read(0, 1, &check);

    mutex = xSemaphoreCreateMutex();

    if(check == 72U)
    {
        uint8_t raw_data[3];

        check = 0x00;
        AK8963_I2C_Write(AK8963_CNTL, 1, &check); // Power down magnetometer
        HAL_Delay(10);

        check = 0x0F;
        AK8963_I2C_Write(AK8963_CNTL, 1, &check); // Enter Fuse ROM access mode
        HAL_Delay(10);
        AK8963_I2C_Read(AK8963_ASAX, 3, raw_data);

        mag_bias_factory[0] = ((static_cast<float>(raw_data[0]) - 128.0f)
            / 256.0f + 1.0f) * 4912.0f / 32760.0f;
        mag_bias_factory[1] = ((static_cast<float>(raw_data[1]) - 128.0f)
            / 256.0f + 1.0f) * 4912.0f / 32760.0f;
        mag_bias_factory[2] = ((static_cast<float>(raw_data[2]) - 128.0f)
            / 256.0f + 1.0f) * 4912.0f / 32760.0f;

        check = 0x00;
        AK8963_I2C_Write(AK8963_CNTL, 1, &check); // Power down magnetometer
        HAL_Delay(10);

        check = 0x16;
        AK8963_I2C_Write(AK8963_CNTL, 1, &check);  // Set magnetometer data resolution and sample ODR
        HAL_Delay(10);

    }

    xTaskCreate((TaskFunction_t) imu::task_cb, (const char*) "MPU9250",
        (uint16_t) IMU_STK_SIZE, (void*) NULL,
        (UBaseType_t) IMU_TASK_PRIO,
        (TaskHandle_t*) &ImuTask_Handler);
}

void imu::task_cb(void *)
{
    for(;;)
    {
        xSemaphoreTake(mutex, portMAX_DELAY);
        calcAttitude();
        readMag();
        xSemaphoreGive(mutex);
        vTaskDelay(15);
    }
}

void imu::MPU9250_I2C_Read(uint8_t regAddr, uint8_t bytes, uint8_t *dest)
{  
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);
    HAL_I2C_Mem_Read(&I2cHandle, MPU9250_ADDR, regAddr, I2C_MEMADD_SIZE_8BIT, dest, bytes, HAL_MAX_DELAY);
    xSemaphoreGive(i2c_mutex);  
}

void imu::MPU9250_I2C_Write(uint8_t regAddr, uint8_t bytes, uint8_t *data)
{
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);
    HAL_I2C_Mem_Write(&I2cHandle, MPU9250_ADDR, regAddr, I2C_MEMADD_SIZE_8BIT, data, bytes, HAL_MAX_DELAY);
    xSemaphoreGive(i2c_mutex);  
}

void imu::AK8963_I2C_Read(uint8_t regAddr, uint8_t bytes, uint8_t *dest)
{
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);
    HAL_I2C_Mem_Read(&I2cHandle, AK8963_ADDR, regAddr, I2C_MEMADD_SIZE_8BIT, dest, bytes, HAL_MAX_DELAY);
    xSemaphoreGive(i2c_mutex);  
}

void imu::AK8963_I2C_Write(uint8_t regAddr, uint8_t bytes, uint8_t *data)
{
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);
    HAL_I2C_Mem_Write(&I2cHandle, AK8963_ADDR, regAddr, I2C_MEMADD_SIZE_8BIT, data, bytes, HAL_MAX_DELAY);
    xSemaphoreGive(i2c_mutex);  
}

/// @brief Set the accelerometer full scale range.
/// @param aScale Set 0 for ±2g, 1 for ±4g, 2 for ±8g, and 3 for ±16g.
void imu::writeAccFullScaleRange(uint8_t aFSR)
{
    // Variable init
    uint8_t select;

    // Set the value
    switch (aFSR)
    {
    case AFSR_2G:
        aScaleFactor = 16384.0;
        select = 0x00;
        MPU9250_I2C_Write(ACCEL_CONFIG, 1, &select);
        break;
    case AFSR_4G:
        aScaleFactor = 8192.0;
        select = 0x08;
        MPU9250_I2C_Write(ACCEL_CONFIG, 1, &select);
        break;
    case AFSR_8G:
        aScaleFactor = 4096.0;
        select = 0x10;
        MPU9250_I2C_Write(ACCEL_CONFIG, 1, &select);
        break;
    case AFSR_16G:
        aScaleFactor = 2048.0;
        select = 0x18;
        MPU9250_I2C_Write(ACCEL_CONFIG, 1, &select);
        break;
    default:
        aScaleFactor = 8192.0;
        select = 0x08;
        MPU9250_I2C_Write(ACCEL_CONFIG, 1, &select);
        break;
    }
}

/// @brief Set the gyroscope full scale range.
/// @param gScale Set 0 for ±250°/s, 1 for ±500°/s, 2 for ±1000°/s, and 3 for ±2000°/s.
void imu::writeGyroFullScaleRange(uint8_t gFSR)
{
    // Variable init
    uint8_t select;

    // Set the value
    switch (gFSR)
    {
        case GFSR_250DPS:
            gScaleFactor = 131.0;
            select = 0x00;
            MPU9250_I2C_Write(GYRO_CONFIG, 1, &select);
            break;
        case GFSR_500DPS:
            gScaleFactor = 65.5;
            select = 0x08;
            MPU9250_I2C_Write(GYRO_CONFIG, 1, &select);
            break;
        case GFSR_1000DPS:
            gScaleFactor = 32.8;
            select = 0x10;
            MPU9250_I2C_Write(GYRO_CONFIG, 1, &select);
            break;
        case GFSR_2000DPS:
            gScaleFactor = 16.4;
            select = 0x18;
            MPU9250_I2C_Write(GYRO_CONFIG, 1, &select);
            break;
        default:
            gScaleFactor = 65.5;
            select = 0x08;
            MPU9250_I2C_Write(GYRO_CONFIG, 1, &select);
            break;
    }
}

/// @brief Set the gyroscope full scale range
/// @param gFSR Desired yroscope full scale range
void imu::setGyroFullScaleRange(uint8_t gFSR)
{
    _gFSR = gFSR;
}

/// @brief Set the accelerometer full scale range
/// @param aFSR Desired accelerometer full scale range
void imu::setAccFullScaleRange(uint8_t aFSR)
{
    _aFSR = aFSR;
}

/// @brief Set the sampling duration (delta time) in seconds
/// @param dt Sampling time delta in seconds
void imu::setDeltaTime(float dt)
{
    _dt = dt;
}

/// @brief Time constant of the complementary filter
/// @param tau Time constant
void imu::setTau(float tau)
{
    _tau = tau;
}

/// @brief Find offsets for each axis of gyroscope
void imu::calibrateGyro(uint16_t numCalPoints)
{
    // Init
    RawData rawData;
    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;

    // Zero guard
    if (numCalPoints == 0)
    {
        numCalPoints = 1;
    }

    // Save specified number of points
    for (uint16_t ii = 0; ii < numCalPoints; ii++)
    {
        rawData = readRawData();
        x += rawData.gx;
        y += rawData.gy;
        z += rawData.gz;
        HAL_Delay(3);
    }

    // Average the saved data points to find the gyroscope offset
    gyroCal.x = (float)x / (float)numCalPoints;
    gyroCal.y = (float)y / (float)numCalPoints;
    gyroCal.z = (float)z / (float)numCalPoints;
}

/// @brief Read raw data from IMU
/// @return Structure containing raw accelerometer and gyroscope data
RawData imu::readRawData()
{
    // Data out and buffer init
    RawData rawData;
    uint8_t buf[14];

    // Subroutine for reading the raw data
    MPU9250_I2C_Read(ACCEL_XOUT_H, 14, buf);

    // Bit shift the data
    rawData.ax = buf[0] << 8 | buf[1];
    rawData.ay = buf[2] << 8 | buf[3];
    rawData.az = buf[4] << 8 | buf[5];
    // temperature = buf[6] << 8 | buf[7];
    rawData.gx = buf[8] << 8 | buf[9];
    rawData.gy = buf[10] << 8 | buf[11];
    rawData.gz = buf[12] << 8 | buf[13];

    // Result
    return rawData;
}

ProcessedData imu::processData()
{
    // Data out structure
    ProcessedData processedData;

    // Get raw values from the IMU
    RawData rawData = readRawData();

    // Convert accelerometer values to g's
    processedData.ax = rawData.ax / aScaleFactor;
    processedData.ay = rawData.ay / aScaleFactor;
    processedData.az = rawData.az / aScaleFactor;

    // Compensate for gyro offset
    processedData.gx = rawData.gx - gyroCal.x;
    processedData.gy = rawData.gy - gyroCal.y;
    processedData.gz = rawData.gz - gyroCal.z;

    // Convert gyro values to deg/s
    processedData.gx /= gScaleFactor;
    processedData.gy /= gScaleFactor;
    processedData.gz /= gScaleFactor;

    // Return structure
    return processedData;
}

/// @brief Calculate the attitude of the sensor in degrees
/// @return Structure containing sensor attitude data
Attitude imu::calcAttitude()
{
    // Read processed data
    ProcessedData sensorData = processData();

    attitude.r = atan2(sensorData.ay, sensorData.az) * RAD2DEG;
    attitude.p = atan2(-1 * sensorData.ax, sqrt(pow(sensorData.ay, 2) + pow(sensorData.az, 2))) * RAD2DEG;
    attitude.y += sensorData.gz * _dt;

    // Return
    return attitude;
}

void imu::readMag()
{
    static volatile uint8_t raw_data[6]; 
    static volatile int16_t destination[3] = {0, 0, 0};

    AK8963_I2C_Read(AK8963_XOUT_L, 7, const_cast<uint8_t*>(raw_data));

    destination[0] = ((int16_t)raw_data[1] << 8) | raw_data[0];  // Turn the MSB and LSB into a signed 16-bit value
    destination[1] = ((int16_t)raw_data[3] << 8) | raw_data[2];  // Data stored as little Endian
    destination[2] = ((int16_t)raw_data[5] << 8) | raw_data[4];

    m[0] =   static_cast<float>(destination[0]) * mag_bias_factory[0];
    m[1] =   static_cast<float>(destination[1]) * mag_bias_factory[1];
    m[2] =   static_cast<float>(destination[2]) * mag_bias_factory[2];
}

void imu::getDataSafe(imu_data_s *data)
{
    xSemaphoreTake(mutex, portMAX_DELAY);
    data->yaw = attitude.y;
    data->roll = attitude.r;
    data->pitch = attitude.p;
    static float declination = 11.0f / 180.0f * M_PI;

    float heading = atan2(m[1], m[0]);
    if (heading < 0) {
        heading += 2 * M_PI;
    }

    heading += declination;
    if (heading > 2 * M_PI) {
        heading -= 2 * M_PI;
    }
    float headingDegrees = heading * 180 / M_PI;
    data->heading = static_cast<int>(headingDegrees);
    xSemaphoreGive(mutex);
}
