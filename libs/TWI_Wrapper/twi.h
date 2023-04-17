/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TWI_H__
#define __TWI_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported macro ------------------------------------------------------------*/
#define I2C_ADDMODE_7BIT I2C_AcknowledgedAddress_7bit

/* Exported types ------------------------------------------------------------*/
/* I2C Tx/Rx buffer size */
#define I2C_TX_RX_BUFFER_SIZE 32

typedef struct i2c_s i2c_t;

struct i2c_s {
	I2C_TypeDef *I2Cx;
	uint8_t init_done;
	uint16_t Address;
	uint8_t i2cTxRxBuffer[I2C_TX_RX_BUFFER_SIZE];
	uint8_t i2cTxRxBufferSize;
	uint8_t txCounter;
};

///@brief I2C state
typedef enum {
	I2C_OK = 0, I2C_TIMEOUT = 1, I2C_ERROR = 2, I2C_BUSY = 3
} i2c_status_e;

#define I2C_DEFAULT_SPEED ((uint32_t)100000)

#define I2C_DELAY_MAX ((uint32_t)1)

/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void
i2c_init(i2c_t *obj);
void
i2c_custom_init(i2c_t *obj, uint32_t timing);

void
i2c_deinit(i2c_t *obj);
void
i2c_setTiming(i2c_t *obj, uint32_t frequency);

i2c_status_e
i2c_master_write(i2c_t *obj, uint8_t dev_address, uint8_t *data, uint8_t size);
i2c_status_e
i2c_master_read(i2c_t *obj, uint8_t dev_address, uint8_t *data, uint8_t size);

i2c_status_e
i2c_IsDeviceReady(i2c_t *obj, uint8_t dev_address, uint32_t trials);

#endif /* __TWI_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
