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
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

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
	sweep(0, 18, 1, 20, 250);
}

void sweepLimit() {
	sweep(0, 180, 1, 30, 1000);
}

// 30 ms for delay
void sweep(int start, int stop, int step, int delayTime, int pauseInterval) {

	int totalMovements = abs(stop - start) / step;
	fadeAmount = 255 / totalMovements;
	Serial.print("fadeAmount: ");
	Serial.println(fadeAmount);

	Serial.println("start");
	for (int i = start; i <= stop; i += step) {

		// set the brightness
		analogWrite(ledPin, brightness);
		Serial.print("brightness: ");
		Serial.println(brightness);

		// change the brightness for next time through the loop:
		brightness = brightness + fadeAmount;

		// reverse the direction of the fading at the ends of the fade:
		if (brightness <= 0 || brightness >= 255) {
			fadeAmount = -fadeAmount;
		}
		myservo.write(i);
		Serial.println(i);
		delay(delayTime);
	}
	delay(pauseInterval);
	for (int i = stop; i >= start; i -= step) {

		// set the brightness of pin 9:
		analogWrite(ledPin, brightness);

		brightness = brightness + fadeAmount;

		// reverse the direction of the fading at the ends of the fade:
		if (brightness <= 0 || brightness >= 255) {
			fadeAmount = -fadeAmount;
		}

		myservo.write(i);
		Serial.println(i);
		delay(delayTime);
	}
	Serial.println("stop");
}
// 30 ms for delay
void sweepOpen(int start, int stop, int step, int delayTime) {
//TODO
}

void sweepClose(int start, int stop, int step, int delayTime,
		int pauseInterval) {
// TODO
}

void fade(int delayTime) {

	boolean running = true;
	int cyleNumber = 0;
	do {
		// set the brightness of pin 9:
		analogWrite(ledPin, brightness);

		// change the brightness for next time through the loop:
		brightness = brightness + fadeAmount;

		// reverse the direction of the fading at the ends of the fade:
		if (brightness <= 0 || brightness >= 255) {
			++cyleNumber;
			fadeAmount = -fadeAmount;
		}
		// wait for  milliseconds to see the dimming effect
		delay(delayTime);
		if (cyleNumber == 2) {
			running = false;
		}
	} while (running);
}

void loop() {
	Serial.println("looping");
}

void loop1() {
	Serial.println("looping");

	val = digitalRead(inputPin);  // read input value

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
	delay(500);
}

