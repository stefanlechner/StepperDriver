/*
 * Synchronous Multi-motor group driver
 * All motors reach their target at the same time.
 *
 * Copyright (C)2017 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#include "SyncDriver.h"


#define FOREACH_MOTOR(action) for (short i=count-1; i >= 0; i--){action;}

/*
 * Initialize motor parameters
 */
void SyncDriver::startMove(long steps1, long steps2, long steps3) {
    long steps[3] = {steps1, steps2, steps3};
    long times[3] = {0,0,0};
    short gaps[3] = {0,0,0};
    long timing[MAX_MOTORS];
    /*
     * find which motor would take the longest to finish,
     */
    long move_time = 0;
    for (short i=count-1;i>=0;i--) {
        rpms[i] = motors[i]->getRPM();
        motors[i]->setRPM(preferredRPM);
        long m = motors[i]->getTimeForMove(abs(steps[i]));
        timing[i] = m;
        if (m > move_time) {
            move_time = m;
        }
    };

    /*
     * Initialize state for all active motors
     */
    for (short i=count-1;i>=0;i--) {
        if (steps[i]) {
            motors[i]->startMove(steps[i], move_time);
        };
        event_timers[i] = 0;
    };
    ready = false;
}

long SyncDriver::nextAction(void) {
    long next_event = MultiDriver::nextAction();
    if (!next_event) {
        /*
         * Restore original rpm settings
         */
        FOREACH_MOTOR(
                if (rpms[i]) {
                    motors[i]->setRPM(rpms[i]);
                    rpms[i] = 0;
                }
        );
    }
    return next_event;
}

void SyncDriver::setRPM(short rpm) {
    preferredRPM = rpm;
}
