#include <stddef.h>
#include "twi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32_i2c.h"

static i2c_t *I2C_LocalObject1 = NULL;
static i2c_t *I2C_LocalObject2 = NULL;
static i2c_t *I2C_LocalObject3 = NULL;

static SimpleI2c<I2C_1> I2C_First;
static SimpleI2c<I2C_2> I2C_Second;
static SimpleI2c<I2C_3> I2C_Third;

/****************I2C Master Events (Events grouped in order of communication)********************/

#define I2C_EVENT_MASTER_MODE_SELECT                      ((uint32_t)0x00030001)  /* BUSY, MSL and SB flag */
#define I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED        ((uint32_t)0x00070082)  /* BUSY, MSL, ADDR, TXE and TRA flags */
#define I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED           ((uint32_t)0x00030002)  /* BUSY, MSL and ADDR flags */
#define I2C_EVENT_MASTER_MODE_ADDRESS10                   ((uint32_t)0x00030008)  /* BUSY, MSL and ADD10 flags */
#define I2C_EVENT_MASTER_BYTE_RECEIVED                    ((uint32_t)0x00030040)  /* BUSY, MSL and RXNE flags */
#define I2C_EVENT_MASTER_BYTE_TRANSMITTING                ((uint32_t)0x00070080)  /* TRA, BUSY, MSL, TXE flags */
#define I2C_EVENT_MASTER_BYTE_TRANSMITTED                 ((uint32_t)0x00070084)  /* TRA, BUSY, MSL, TXE and BTF flags */

void i2c_init(i2c_t *obj) {
	i2c_custom_init(obj, I2C_DEFAULT_SPEED);
}

void i2c_custom_init(i2c_t *obj, uint32_t timing) {
	(void) timing;

	if (obj->I2Cx == I2C1) {
		I2C_LocalObject1 = obj;
		I2C_First.initPins();
		I2C_First.init();

	} else if (obj->I2Cx == I2C2) {
		I2C_LocalObject2 = obj;
		I2C_Second.initPins();
		I2C_Second.init();
	} else if (obj->I2Cx == I2C3) {
		I2C_LocalObject3 = obj;
		I2C_Third.initPins();
		I2C_Third.init();
	}

	obj->init_done = 1;
}

void i2c_deinit(i2c_t *obj) {
	obj->init_done = 0;
}

void i2c_setTiming(i2c_t *obj, uint32_t frequency) {
	if (obj->init_done == 1) {
		i2c_custom_init(obj, frequency);
	}
}

bool I2C_CheckEvent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT) {
	uint32_t lastevent = 0;
	uint32_t flag1 = 0, flag2 = 0;
	bool status = false;

	flag1 = I2Cx->SR1;
	flag2 = I2Cx->SR2;
	flag2 = flag2 << 16;

	lastevent = (flag1 | flag2) & 0x00FFFFFF;

	if ((lastevent & I2C_EVENT) == I2C_EVENT) {
		status = true;
	} else {
		status = false;
	}

	return status;
}

static void I2C_ClearFlag(I2C_TypeDef *I2Cx, uint32_t I2C_FLAG) {
	uint32_t flagpos = 0;

	flagpos = I2C_FLAG & 0x00FFFFFF;
	I2Cx->SR1 = (uint16_t) ~flagpos;
}

static
bool i2c_wait_for_event(I2C_TypeDef *I2Cx, uint32_t I2C_Event) {
	__IO uint32_t timeout = xTaskGetTickCount();
	bool ret;

	ret = I2C_CheckEvent(I2Cx, I2C_Event);
	while (ret == false) {
		if ((xTaskGetTickCount() - timeout > I2C_DELAY_MAX)) {
			/*!< Clear AF flag */
			I2C_ClearFlag(I2Cx, I2C_FLAG_AF);

			/*!< STOP condition */
			I2Cx->CR1 |= I2C_CR1_STOP;

			return ret;
		}
		ret = I2C_CheckEvent(I2Cx, I2C_Event);
	}
	return ret;
}

i2c_status_e i2c_IsDeviceReady(i2c_t *obj, uint8_t dev_address,
		uint32_t trials) {
	(void) trials;

	if (obj->init_done == 1) {
		/* Send START condition */
		obj->I2Cx->CR1 |= I2C_CR1_START;

		/* Test on EV5 and clear it */
		if (i2c_wait_for_event(obj->I2Cx, I2C_EVENT_MASTER_MODE_SELECT) != true)
			return I2C_BUSY;

		/* Send slave address + write condition */
		obj->I2Cx->DR = dev_address & 0xFFFE;

		if (i2c_wait_for_event(obj->I2Cx,
		I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != true) {
			return I2C_TIMEOUT;
		} else {
			/*!< Clear AF flag */
			I2C_ClearFlag(obj->I2Cx, I2C_FLAG_AF);

			/*!< STOP condition */
			obj->I2Cx->CR1 |= I2C_CR1_STOP;

			return I2C_OK;
		}
	} else {
		return I2C_ERROR;
	}

}

i2c_status_e i2c_master_write(i2c_t *obj, uint8_t dev_address, uint8_t *data,
		uint8_t size) {
	if (size == 0) {
		return i2c_IsDeviceReady(obj, dev_address, 1);
	}

	if (obj->init_done == 1) {
		/* Send START condition */
		obj->I2Cx->CR1 |= I2C_CR1_START;

		/* Test on EV5 and clear it */
		if (i2c_wait_for_event(obj->I2Cx, I2C_EVENT_MASTER_MODE_SELECT) != true)
			return I2C_BUSY;

		/* Send slave address + write condition */
		obj->I2Cx->DR = dev_address & 0xFFFE;

		if (i2c_wait_for_event(obj->I2Cx,
		I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != true)
			return I2C_BUSY;

		/* Send data */
		while (size != 0) {
			obj->I2Cx->DR = *data++;
			size--;

			if (size > 0) {
				if (i2c_wait_for_event(obj->I2Cx,
				I2C_EVENT_MASTER_BYTE_TRANSMITTING) != true)
					return I2C_BUSY;
			}
		}

		/* Send STOP condition */
		if (i2c_wait_for_event(obj->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)
				!= true)
			return I2C_BUSY;

		obj->I2Cx->CR1 |= I2C_CR1_STOP;

		return I2C_OK;
	} else {
		return I2C_ERROR;
	}

}

static FlagStatus I2C_GetFlagStatus(I2C_TypeDef *I2Cx, uint32_t I2C_FLAG) {
	FlagStatus bitstatus = RESET;
	__IO uint32_t i2creg = 0, i2cxbase = 0;

	i2cxbase = (uint32_t) I2Cx;
	i2creg = I2C_FLAG >> 28;
	I2C_FLAG &= 0x00FFFFFF;

	if (i2creg != 0) {
		i2cxbase += 0x14;
	} else {
		I2C_FLAG = (uint32_t) (I2C_FLAG >> 16);
		i2cxbase += 0x18;
	}

	if (((*(__IO uint32_t*) i2cxbase) & I2C_FLAG) != (uint32_t) RESET) {
		bitstatus = SET;
	} else {
		bitstatus = RESET;
	}

	return bitstatus;
}

i2c_status_e i2c_master_read(i2c_t *obj, uint8_t dev_address, uint8_t *data,
		uint8_t size) {
	uint8_t tmpSize = size;
	uint8_t idx = 0;
	if (obj->init_done == 1) {
		/* Enable ACK */
		obj->I2Cx->CR1 |= I2C_CR1_ACK;

		/* Send START condition */
		obj->I2Cx->CR1 |= I2C_CR1_START;

		if (i2c_wait_for_event(obj->I2Cx, I2C_EVENT_MASTER_MODE_SELECT) != true)
			return I2C_BUSY;

		/* Send slave address + read condition */
		obj->I2Cx->DR = dev_address | 1;

		if (i2c_wait_for_event(obj->I2Cx,
		I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != true)
			return I2C_BUSY;

		/* Send data */
		while (size != 0) {
			if (tmpSize == 1) {
				/* Disable ACK */
				obj->I2Cx->CR1 &= ~I2C_CR1_ACK;

				/* Send STOP condition */
				obj->I2Cx->CR1 |= I2C_CR1_STOP;

				tmpSize = 0;
			} else if (tmpSize == 2) {
				if (i2c_wait_for_event(obj->I2Cx,
						I2C_EVENT_MASTER_BYTE_RECEIVED) != true)
					return I2C_BUSY;

				/* Disable ACK */
				obj->I2Cx->CR1 &= ~I2C_CR1_ACK;

				while (I2C_GetFlagStatus(obj->I2Cx, I2C_FLAG_BTF) == RESET)
					;

				/* Send STOP condition */
				obj->I2Cx->CR1 |= I2C_CR1_STOP;

				while (size != 0) {
					data[idx++] = obj->I2Cx->DR;
					size--;
				}

				tmpSize = 0;
			} else if ((tmpSize > 2) && (size == 3)) {
				while (I2C_GetFlagStatus(obj->I2Cx, I2C_FLAG_BTF) == RESET)
					;

				/* Disable ACK */
				obj->I2Cx->CR1 &= ~I2C_CR1_ACK;

				data[idx++] = obj->I2Cx->DR;
				size--;

				/* Send STOP condition */
				obj->I2Cx->CR1 |= I2C_CR1_STOP;

				data[idx++] = obj->I2Cx->DR;
				size--;
				tmpSize = 0;
			}

			/* Read data received */
			if (I2C_GetFlagStatus(obj->I2Cx, I2C_FLAG_RXNE) == SET) {
				data[idx++] = obj->I2Cx->DR;
				size--;
			}
		}
	} else {
		return I2C_ERROR;
	}

	return I2C_OK;

}
