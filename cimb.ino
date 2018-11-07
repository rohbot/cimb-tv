#include "bb_Generator.h"
#include "FastLED.h"

#define NUM_LEDS 15


#define BB_5V 2
#define BB_14V 3

#define LED_PIN 4

CRGB leds[NUM_LEDS];


bb_Generator cap(A0);
//bb_Generator battery(A1);
//bb_Generator bike1(A2);
//bb_Generator bike2(A3);

bool outOn = false;

int ledsOn = 0;

int hue = 0;

int MAX_ENERGY = 100;

void switchOnLeds() {
	FastLED.clear();

	for (int i = 0; i < ledsOn; i++) {
		hue = map(i, 0, NUM_LEDS, 0, 100);
		leds[i] = CHSV(hue, 255, 255);
	}
	LEDS.show();

}

void setup() {

	pinMode(BB_5V, OUTPUT);
	pinMode(BB_14V, OUTPUT);

	digitalWrite(BB_5V, LOW);
	digitalWrite(BB_14V, LOW);

	cap.setNumReadings(30);
	cap.setEnergyDecrement(0);
	cap.setDiff(1);
	cap.setLowCutoff(60);
	cap.begin();


	FastLED.addLeds<WS2812, LED_PIN, RGB>(leds, NUM_LEDS);
	FastLED.setBrightness(16);

	Serial.begin(115200);
	Serial.println("s,");



}

bool running = false;
bool maxed_out = false;

unsigned long maxed_time = 0;

bool logo_on = false;

void loop() {

	cap.update();
	//battery.update();

	// if (cap.getVoltage() > 5) {
	// 	digitalWrite(BB_5V, HIGH);
	// 	switchOnLeds();
	// }
	// if (cap.getVoltage() < 4.5) {
	// 	digitalWrite(BB_5V, LOW);
	// }

	if (cap.getRunTime() > 0) {
		running = true;
		Serial.print("b,");
		Serial.println(cap.getEnergy());

		// if (cap.getVoltage() > 9.8 && !outOn) {
		// 	digitalWrite(BB_14V, HIGH);
		// 	outOn = true;
		// }
		// if (cap.getVoltage() < 5 && outOn) {
		// 	digitalWrite(BB_14V, LOW);
		// 	outOn = false;
		// }

		ledsOn = map(cap.getEnergy(), 0, MAX_ENERGY, 0 , NUM_LEDS);
		ledsOn = constrain(ledsOn, 0, NUM_LEDS);
		switchOnLeds();
		if(cap.getEnergy() >  MAX_ENERGY){
			if(!maxed_out){
				Serial.println("x,1");
				maxed_out = true;
				maxed_time = millis();
				logo_on = true;
			}

			if(millis() - maxed_time > 10000){
				if(logo_on){
					Serial.println("x,0");
					logo_on = false;		
				}
			
				//cap.resetEnergy();	
			}
		}

	} else {
		if (running) {
			cap.resetEnergy();
			maxed_out = false;
			ledsOn = 0;
			switchOnLeds();
			running = false;
			Serial.print("b,");
			Serial.println(cap.getEnergy());
			Serial.println("x,0");
			//Serial.println(cap.getEnergy());
				
		}

	}
	delay(30);
}
