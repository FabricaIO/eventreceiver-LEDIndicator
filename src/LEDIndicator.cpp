#include "LEDIndicator.h"

/// @brief true when start up succeeds
extern bool POSTSuccess;

/// @brief Creates and LED indicator
/// @param LEDPin The LED pin
/// @param LEDCount The number of LEDs
/// @param startupOnly If set to true, will only show signals during that startup process
/// @param RGB True to use an RGB LED, False to use a single color LED
/// @param LED_Type If RGB, the type of RGB LED to use
LEDIndicator::LEDIndicator(uint8_t LEDPin, int LEDCount, bool startupOnly, bool RGB, neoPixelType LED_Type) : leds(LEDCount, LEDPin, LED_Type) {
	rgb = RGB;
	led_pin = LEDPin;
	startup = startupOnly;
}

/// @brief Initializes the LEDs
/// @return True on success
bool LEDIndicator::begin() {
	Description.name = "LED Indicator";
	Description.version = "0.8";
	// Start LEDs
	if (rgb) {
		leds.begin();
		leds.fill(); // Clear LEDs
		leds.show();
	} else {
		pinMode(led_pin, OUTPUT);
		digitalWrite(led_pin, LOW);
	}
	return true;
}

/// @brief Shows a color on the LED indicator, or blinks the LED
/// @param color The color to show
/// @return True on success
bool LEDIndicator::receiveEvent(int event) {
	if (!POSTSuccess || !startup) {
		if (rgb) {
			leds.fill(color_map[event]);
			leds.show();
		} else {
			for (int i = 0; i < event; i++) {
				digitalWrite(led_pin, HIGH);
				delay (150);
				digitalWrite(led_pin, LOW);
				delay (150);
			}
		}
	} else {
		leds.fill();
		leds.show();
	}
	return true;
}