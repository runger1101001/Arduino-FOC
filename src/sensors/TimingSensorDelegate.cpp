/*
 * TimingSensorDelegate.cpp
 *
 *  Created on: 19 Jan 2021
 *      Author: runger
 */

#include "TimingSensorDelegate.h"
#include "Arduino.h"


void TimingSensorDelegate::init() {
	pinMode(timingPin, OUTPUT);
	digitalWrite(timingPin, 0);
}


float TimingSensorDelegate::getAngle() {
	float it = 0;
	digitalWrite(timingPin, 1);
	it = delegate.getAngle();
	digitalWrite(timingPin, 0);
	return it;
}


float TimingSensorDelegate::getVelocity() {
	float it = 0;
	digitalWrite(timingPin, 1);
	it = delegate.getVelocity();
	digitalWrite(timingPin, 0);
	return it;
}
