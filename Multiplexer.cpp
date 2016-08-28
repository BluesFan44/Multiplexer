#include "Multiplexer.h"
#include "arduino.h"

#define NUMBER_OF_SHIFT_CHIPS   1
#define DATA_WIDTH   NUMBER_OF_SHIFT_CHIPS * 8
#define PULSE_WIDTH_USEC   5
#define POLL_DELAY_MSEC   1
#define BYTES_VAL_T unsigned int

int _ploadPin;  // Connects to Parallel load pin the 165
int _clockEnablePin;  // Connects to Clock Enable pin the 165
int _dataPin;
int _clockPin;
int _bounceValue;

BYTES_VAL_T _pinValues;

Multiplexer::Multiplexer(int ploadPin, int clockEnablePin, int dataPin, int clockPin)
{
	_ploadPin = ploadPin;  // Connects to Parallel load pin the 165
	_clockEnablePin = clockEnablePin;  // Connects to Clock Enable pin the 165
	_dataPin = dataPin; // Connects to the Q7 pin the 165
	_clockPin = clockPin; // Connects to the Clock pin the 165

	pinMode(_ploadPin, OUTPUT);
	pinMode(_clockEnablePin, OUTPUT);
	pinMode(_clockPin, OUTPUT);
	pinMode(_dataPin, INPUT);

	digitalWrite(_clockPin, LOW);
	digitalWrite(_ploadPin, HIGH);
}

BYTES_VAL_T Multiplexer::read_shift_regs()
{
	long bitVal;
	BYTES_VAL_T bytesVal = 0;

	digitalWrite(_clockEnablePin, HIGH);
	digitalWrite(_ploadPin, LOW);
	delayMicroseconds(PULSE_WIDTH_USEC);
	digitalWrite(_ploadPin, HIGH);
	digitalWrite(_clockEnablePin, LOW);

	for (int i = 0; i < DATA_WIDTH; i++)
	{
		bitVal = digitalRead(_dataPin);
		bytesVal |= (bitVal << ((DATA_WIDTH - 1) - i));
		digitalWrite(_clockPin, HIGH);
		delayMicroseconds(PULSE_WIDTH_USEC);
		digitalWrite(_clockPin, LOW);
	}
	_pinValues = bytesVal;
	return(bytesVal);
}

void Multiplexer::display_pin_values()
{
	Serial.print("Pin States:" + (String)_pinValues + "\r\n");

	for (int i = 0; i < DATA_WIDTH; i++)
	{
		Serial.print("  Pin-");
		Serial.print(i);
		Serial.print(": ");

		if ((_pinValues >> i) & 1)
			Serial.print("HIGH");
		else
			Serial.print("LOW");

		Serial.print("\r\n");
	}
	Serial.print("\r\n");
}

int Multiplexer::pressedButton() {
	float _value = read_shift_regs();
	if (_value == 0) {
		_bounceValue = 0;
		return (0); // no buttons pressed
	}
	float _floatButtonPressed = log2(_value)+1;
	int _intButtonPressed = (int)_floatButtonPressed;
	if (_floatButtonPressed ==_intButtonPressed) { // even power of two means only one button pressed
		if (_bounceValue == _intButtonPressed) return (0); // debounce
		//Serial.println("BounceValue: " + (String)_bounceValue);
		_bounceValue = _intButtonPressed;
	}
	return (_intButtonPressed); // add one to prevent 0-based button numbers
}

float Multiplexer::log2(float v) {
	// from http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogFloat
	// find int(log2(v)), where v > 0.0 && finite(v) && isnormal(v)
	float c;         // 32-bit int c gets the result;

	c = (log(v) / log(2));
	return (c);
}