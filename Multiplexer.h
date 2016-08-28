// Multiplexer.h

#ifndef _MULTIPLEXER_h
#define _MULTIPLEXER_h
#define BYTES_VAL_T unsigned int

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
class Multiplexer {
public:
	Multiplexer(int ploadPin, int clockEnablePin, int dataPin, int clockPin);
	void display_pin_values();
	int pressedButton();
private:
	int _ploadPin;
	int _clockEnablePin;
	int _dataPin;
	int _clockPin;
	int _bounceValue;
	BYTES_VAL_T _pinValues;
	float log2(float v);
	BYTES_VAL_T read_shift_regs();
};
#endif

