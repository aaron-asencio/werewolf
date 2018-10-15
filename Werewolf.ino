#include "Arduino.h"
/*
 *
 * If using larger server 20 kg cm, DC adapter must be 6-7.4 V with 1A min
 */

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int servoPin = 9;

// PIR senosr
int ledPin = 10;                // choose the pin for the LED

int inputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;

void setup() {
	Serial.begin(9600);
	pinMode(ledPin, OUTPUT);      // declare LED as output
	pinMode(inputPin, INPUT);     // declare sensor as input
	myservo.attach(servoPin); // attaches the servo on pin 9 to the servo object
}

void chomp() {
	Serial.println("chomp!");
	sweep(0, 18, 1, 50, 250);
}

void sweepLimit() {
	sweep(0, 180, 1, 30, 1000);
}

int fadeRange = 51;

// 30 ms for delay
void sweep(int start, int stop, int step, int delayTime, int pauseInterval) {
	int fadeValue = 35;

	Serial.print("fadeValue: ");
	Serial.println(fadeValue);

	Serial.println("start");
	for (int i = start; i <= stop; i += step) {

		// set the brightness
		analogWrite(ledPin, fadeValue * 5);
		Serial.print("fadeValue: ");
		Serial.println(fadeValue * 5);

		// change the brightness for next time through the loop:
		fadeValue++;
		myservo.write(i);
		//Serial.println(i);
		delay(delayTime);

	}

	//brighten(fadeValue * 5, fadeRange, delayTime);
	analogWrite(ledPin, 255);
	delay(1000);

	fadeValue = 51;
	for (int i = stop; i >= start; i -= step) {
		// set the brightness
		analogWrite(ledPin, fadeValue);
		Serial.print("fadeValue: ");
		Serial.println(fadeValue);
		fadeValue--;
		myservo.write(i);
		//Serial.println(i);
		delay(delayTime);
	}
	dim(fadeValue, 0, delayTime);

	Serial.println("stop");
}

void brighten(int fadeStart, int fadeEnd, int delayTime) {
	for (int fadeValue = fadeStart; fadeValue <= fadeEnd; fadeValue++) {
		// sets the value (range from 0 to 51):
		analogWrite(ledPin, fadeValue *5);
		Serial.print("brighten fadeValue: ");
		Serial.println(fadeValue * 5);

		// wait to see the dimming effect
		delay(delayTime);
	}

}
void dim(int fadeStart, int fadeEnd, int delayTime) {

	for (int fadeValue = fadeStart; fadeValue >= fadeEnd; fadeValue--) {

		analogWrite(ledPin, fadeValue * 5);
		Serial.print("dim fadeValue: ");
		Serial.println(fadeValue);

		delay(delayTime);
	}
}

void handleMotion(int val)
{
	if (val == HIGH) {            // check if the input is HIGH

			if (pirState == LOW) {
				// we have just turned on
				Serial.println("Motion detected!");
				chomp();
				// We only want to print on the output change, not state
				pirState = HIGH;
			}
		} else {
			//
			if (pirState == HIGH) {
				// we have just turned of
				Serial.println("Motion ended!");
				// We only want to print on the output change, not state
				pirState = LOW;
			}
		}
}
void loop() {
	Serial.println("looping");
	//set to zero to prevent any jerking doing looping
	myservo.write(0);
	val = digitalRead(inputPin);  // read input value
	handleMotion(val);
	delay(500);
}

