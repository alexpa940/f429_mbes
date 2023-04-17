extern "C" {
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
}

#include "Wire.h"

// Preinstantiate Objects //////////////////////////////////////////////////////

TwoWire Wire1 = TwoWire();
TwoWire Wire2 = TwoWire();
TwoWire Wire3 = TwoWire();

// Public Methods //////////////////////////////////////////////////////////////

void TwoWire::begin(I2C_TypeDef *ptr) {

	_i2c.I2Cx = ptr;
	rxBufferIndex = 0;
	rxBufferLength = 0;
	txBufferIndex = 0;
	txBufferLength = 0;

	transmitting = 0;

	ownAddress = 0;

	i2c_custom_init(&_i2c, I2C_DEFAULT_SPEED);

}

void TwoWire::end(void) {
	i2c_deinit(&_i2c);
}

void TwoWire::setClock(uint32_t frequency) {
	i2c_setTiming(&_i2c, frequency);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity,
		uint32_t iaddress, uint8_t isize, uint8_t sendStop) {
	(void) sendStop;
	if (isize > 0) {
		// send internal address; this mode allows sending a repeated start to access
		// some devices' internal registers. This function is executed by the hardware
		// TWI module on other processors (for example Due's TWI_IADR and TWI_MMR registers)

		beginTransmission(address);

		// the maximum size of internal address is 3 bytes
		if (isize > 3) {
			isize = 3;
		}

		// write internal register address - most significant byte first
		while (isize-- > 0)
			write((uint8_t) (iaddress >> (isize * 8)));

		endTransmission(false);
	}

	// clamp to buffer length
	if (quantity > BUFFER_LENGTH) {
		quantity = BUFFER_LENGTH;
	}
	// perform blocking read into buffer
	uint8_t read = 0;
	if (I2C_OK == i2c_master_read(&_i2c, address << 1, rxBuffer, quantity))
		read = quantity;

	// set rx buffer iterator vars
	rxBufferIndex = 0;
	rxBufferLength = read;

	return read;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity,
		uint8_t sendStop) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint32_t) 0,
			(uint8_t) 0, (uint8_t) sendStop);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) true);
}

uint8_t TwoWire::requestFrom(int address, int quantity) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop) {
	return requestFrom((uint8_t) address, (uint8_t) quantity,
			(uint8_t) sendStop);
}

void TwoWire::beginTransmission(uint8_t address) {
	// indicate that we are transmitting
	transmitting = 1;
	// set address of targeted slave
	txAddress = address << 1;
	// reset tx buffer iterator vars
	txBufferIndex = 0;
	txBufferLength = 0;
}

void TwoWire::beginTransmission(int address) {
	beginTransmission((uint8_t) address);
}

uint8_t TwoWire::endTransmission(uint8_t sendStop) {
	(void) sendStop;
	int8_t ret = 4;

	// transmit buffer (blocking)
	switch (i2c_master_write(&_i2c, txAddress, txBuffer, txBufferLength)) {
	case I2C_OK:
		ret = 0;
		break;
	case I2C_TIMEOUT:
		ret = 1;
		break;
	default:
		ret = 4;
		break;
	}

	// reset tx buffer iterator vars
	txBufferIndex = 0;
	txBufferLength = 0;
	// indicate that we are done transmitting
	transmitting = 0;

	return ret;
}

//	This provides backwards compatibility with the original
//	definition, and expected behaviour, of endTransmission
//
uint8_t TwoWire::endTransmission(void) {
	return endTransmission(true);
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(uint8_t data) {
	// in master transmitter mode
	// don't bother if buffer is full
	if (txBufferLength >= BUFFER_LENGTH) {
		return 0;
	}
	// put byte in tx buffer
	txBuffer[txBufferIndex] = data;
	++txBufferIndex;
	// update amount in buffer
	txBufferLength = txBufferIndex;
	return 1;
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(const uint8_t *data, size_t quantity) {
	// in master transmitter mode
	for (size_t i = 0; i < quantity; ++i) {
		write(data[i]);
	}
	return quantity;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::available(void) {
	return rxBufferLength - rxBufferIndex;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::read(void) {
	int value = -1;

	// get each successive byte on each call
	if (rxBufferIndex < rxBufferLength) {
		value = rxBuffer[rxBufferIndex];
		++rxBufferIndex;

		/* Commented as not I think it is not useful
		 * but kept to show that it is possible to
		 * reset rx buffer when no more data available */
		/*if(rxBufferIndex == rxBufferLength) {
		 resetRxBuffer();
		 }*/
	}

	return value;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::peek(void) {
	int value = -1;

	if (rxBufferIndex < rxBufferLength) {
		value = rxBuffer[rxBufferIndex];
	}

	return value;
}

void TwoWire::flush(void) {
	rxBufferIndex = 0;
	rxBufferLength = 0;
	txBufferIndex = 0;
	txBufferLength = 0;
}

