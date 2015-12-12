/*
 * Simple demo, should work with any driver board
 *
 * Connect STEP, DIR, ~ENBL as indicated
 *
 * Copyright (C)2015 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#include <Arduino.h>
#include "BasicStepperDriver.h"

// All the wires needed for full functionality
#define DIR 8
#define STEP 9
#define ENBL 7

// Since microstepping is set externally, make sure this matches the selected mode
// 1=full step, 2=half step etc.
#define MICROSTEPS 1

// 3-wire basic config, microstepping is hardwired on the driver
BasicStepperDriver stepper(DIR, STEP, ENBL);

void setup() {
    /*
     * Set target motor RPM. 
     * These motors can do up to about 200rpm.
     * Too high will result in a high pitched whine and the motor does not move.
     */
    stepper.setRPM(120);
}

void loop() {
    // energize coils - the motor will hold position
    stepper.enable();

    /*
     * Tell the driver the microstep level we selected.
     * If mismatched, the motor will move at a different RPM than chosen.
     */ 
    stepper.setMicrostep(MICROSTEPS);

    /*
     * Moving motor one full revolution
     */
    stepper.move(200 * MICROSTEPS);

    /*
     * Moving motor one full reverse revolution
     */
    stepper.move(-200 * MICROSTEPS);

    // pause and allow the motor to be moved by hand
    stepper.disable();
    
    delay(5000);
}