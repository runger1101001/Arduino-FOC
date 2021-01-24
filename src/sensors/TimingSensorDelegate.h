/*
 * TimingSensorDelegate.h
 *
 *  Created on: 19 Dec 2020
 *      Author: runger
 */

#ifndef TIMINGSENSORDELEGATE_H_
#define TIMINGSENSORDELEGATE_H_

#include "../common/base_classes/Sensor.h"


/**
 * Wraps another sensor, calling the methods of the wrapped sensor when this sensor is called.
 * It also toggles a configurable pin high during the getAngle() and getVelocity() calls, which
 * means you can attach a logic analyser to that pin and observe the frequency and duration of
 * the sensor calls.
 * This is useful, because the speed at which you can read the sensor will be the determining
 * factor to how quickly you can call loopFOC() and move(), and therefore how fast you can spin
 * the motor, or how smoothly it spins when spinning slowly. It can also have an effect on
 * noise produced by the motor, and obtainable torque.
 *
 * Usage:
 *
 * #include <TimingSensorDelegate.h>
 * #define TIMING_PIN 33
 *
 * ...
 *
 *  MagneticSensorI2C sensor = MagneticSensorI2C(0x40, 14, 0xFE, 8); // your real sensor, this is an example
 *  TimingSensorDelegate timedSensor(sensor, TIMING_PIN);
 *
 * ...
 *
 *	timedSensor.init(); 				// puts timer pin in output mode
 *	sensor.init();      				// don't forget to init the real sensor too
 *	motor.linkSensor(&timedSensor);		// but link the timed sensor
 *
 * ...
 *
 *
 *
 *
 * Now you can sit back and watch the timings in your oscilloscope or logic analyzer.
 *
 *
 */

#include "common/base_classes/Sensor.h"

class TimingSensorDelegate : public Sensor {
public:
	TimingSensorDelegate(Sensor &wrapped, int pin=-1) : delegate(wrapped), timingPin(pin) {};
    virtual float getAngle();
    virtual float getVelocity();
    virtual float initRelativeZero() { return delegate.initRelativeZero(); };
    virtual float initAbsoluteZero() { return delegate.initAbsoluteZero(); };
    virtual int hasAbsoluteZero() { return delegate.hasAbsoluteZero(); };
    virtual int needsAbsoluteZeroSearch() { return delegate.needsAbsoluteZeroSearch(); };

    void init();

	virtual ~TimingSensorDelegate() {};
private:
	Sensor& delegate;
	int timingPin;
};

#endif /* TIMINGSENSORDELEGATE_H_ */
