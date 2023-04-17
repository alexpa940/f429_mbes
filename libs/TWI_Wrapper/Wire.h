#ifndef TwoWire_h
#define TwoWire_h

#include <inttypes.h>
#include "twi.h"

#define BUFFER_LENGTH 64

class TwoWire {
private:
	uint8_t rxBuffer[BUFFER_LENGTH];
	uint8_t rxBufferIndex;
	uint8_t rxBufferLength;

	uint8_t txAddress;
	uint8_t txBuffer[BUFFER_LENGTH];
	uint8_t txBufferIndex;
	uint8_t txBufferLength;

	uint8_t transmitting;

	uint8_t ownAddress;

public:
	i2c_t _i2c;
	TwoWire() = default;
	void begin(I2C_TypeDef*);
	void end();
	void setClock(uint32_t);
	void beginTransmission(uint8_t);
	void beginTransmission(int);
	uint8_t endTransmission(void);
	uint8_t endTransmission(uint8_t);
	uint8_t requestFrom(uint8_t, uint8_t);
	uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
	uint8_t requestFrom(uint8_t, uint8_t, uint32_t, uint8_t, uint8_t);
	uint8_t requestFrom(int, int);
	uint8_t requestFrom(int, int, int);
	virtual size_t write(uint8_t);
	virtual size_t write(const uint8_t*, size_t);
	virtual int available(void);
	virtual int read(void);
	virtual int peek(void);
	virtual void flush(void);
	void onReceive(void (*)(int));
	void onRequest(void (*)(void));

	inline size_t write(unsigned long n) {
		return write((uint8_t) n);
	}
	inline size_t write(long n) {
		return write((uint8_t) n);
	}
	inline size_t write(unsigned int n) {
		return write((uint8_t) n);
	}
	inline size_t write(int n) {
		return write((uint8_t) n);
	}
};

extern TwoWire Wire1;
extern TwoWire Wire2;
extern TwoWire Wire3;

#endif
