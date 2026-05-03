#include "LEDIndicator.h"

/// @brief Creates and LED indicator
/// @param LEDPin The LED pin
/// @param LEDCount The number of LEDs
/// @param ignoreRunning Set true to disable the LED when running normally (i.e. only show error or configuration events)
/// @param RGB True to use an RGB LED, False to use a single color LED
/// @param LED_Type If RGB, the type of RGB LED to use
LEDIndicator::LEDIndicator(uint8_t LEDPin, int LEDCount, bool ignoreRunning, bool RGB, neoPixelType LED_Type) : leds(LEDCount, LEDPin, LED_Type) {
	rgb = RGB;
	led_pin = LEDPin;
	running = ignoreRunning;
}

/// @brief Initializes the LEDs
/// @return True on success
bool LEDIndicator::begin() {
	Description.name = "LED Indicator";
	Description.version = "0.8";
	if (rgb) {
		if (NeoPixelControl::neoMutex == NULL) {
			if (!NeoPixelControl::createMutex()) {
				return false;
			}
		}
	}
	// Start LEDs
	if (rgb) {
		if (xSemaphoreTake(NeoPixelControl::neoMutex, pdMS_TO_TICKS(1000)) == pdFALSE) {
			Logger.println("LEDIndicator: timeout waiting for mutex");
			return false;
		}
		leds.begin();
		leds.fill(); // Clear LEDs
		leds.show();
		xSemaphoreGive(NeoPixelControl::neoMutex);
	} else {
		pinMode(led_pin, OUTPUT);
		digitalWrite(led_pin, LOW);
	}
	return true;
}

/// @brief Shows a color on the LED indicator or blinks the LED
/// @param color The color to show
/// @return True on success
bool LEDIndicator::receiveEvent(int event) {
	if (!running || (event != 1 && event != 2)) {
		if (rgb) {
			if (xSemaphoreTake(NeoPixelControl::neoMutex, pdMS_TO_TICKS(1000)) == pdFALSE) {
				Logger.println("LEDIndicator: timeout waiting for mutex");
				return false;
			}
			leds.fill(color_map[event]);
			leds.show();
			xSemaphoreGive(NeoPixelControl::neoMutex);
		} else {
			for (int i = 0; i < event; i++) {
				digitalWrite(led_pin, HIGH);
				delay (150);
				digitalWrite(led_pin, LOW);
				delay (150);
			}
		}
	} else if (rgb) {
		if (xSemaphoreTake(NeoPixelControl::neoMutex, pdMS_TO_TICKS(1000)) == pdFALSE) {
			Logger.println("LEDIndicator: timeout waiting for mutex");
			return false;
		}
		leds.fill();
		leds.show();
		xSemaphoreGive(NeoPixelControl::neoMutex);
	}
	return true;
}