#include "FastLED.h"

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define minDelta 0
#define maxDelta 35

#define firstStrip 6
#define secondStrip 7
#define colorOrder GRB
#define ledType WS2812B
#define numLeds 600

#define maxLED 460

#define pickupPin A0

#define calibrationCycles 5000

#define rComponent 255
#define gComponent 0
#define bComponent 16

#define pickupNumReads 1000

int calibrationDelta = 0;
int pickupValue = 0;
int ledsToLight = 0;

struct CRGB leds[numLeds];

void calibration() {
	int minValue = 1024;
	int maxValue = 0;

	for (int i = 0; i < calibrationCycles; ++i) {
		int val = analogRead(pickupPin);
		minValue = min(minValue, val);
		maxValue = max(maxValue, val);
	}
	calibrationDelta = maxValue - minValue;
}

void pickupRead() {
	int minValue = 1024;
	int maxValue = 0;

	for(int i = 0; i < pickupNumReads; i++){
		int pickupValue = analogRead(pickupPin);
		minValue = min(minValue, pickupValue);
		maxValue = max(maxValue, pickupValue);
	}

	int pickupDelta = maxValue - minValue;
	pickupDelta = pickupDelta - calibrationDelta;

	if(pickupDelta < 0) {
	    pickupDelta = 0;
	}
	else if(pickupDelta > 35) {
		pickupDelta = 35;
	}
/*
	Serial.print("m=");
	Serial.print(minValue);
	Serial.print(" M=");
	Serial.print(maxValue);
	Serial.print(" D=");
	Serial.println(pickupDelta);
*/
	ledsToLight = map(pickupDelta, minDelta, maxDelta, 0, maxLED);
}

void setup() {
	analogReference(EXTERNAL);
	Serial.begin(9600);
	LEDS.addLeds<ledType, firstStrip, colorOrder>(leds, numLeds);
	LEDS.addLeds<ledType, secondStrip, colorOrder>(leds, numLeds);

	FastLED.clear();
	FastLED.show();
	calibration();
}

void loop() {
	FastLED.clear();

	pickupRead();

	for(int i = 0; i < ledsToLight; i++){

		leds[i] = CHSV(255, 255, 255);
	}	

	FastLED.show();
}