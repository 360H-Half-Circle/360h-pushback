#include "intake.hpp"
#include "controls.hpp"
#include <iostream>

void Intake::initialize() {
    if (initialized) return;

    bottom_intake.set_gearing(E_MOTOR_GEAR_BLUE);
    top_intake.set_gearing(E_MOTOR_GEAR_BLUE);

    antijam = true;
    initialized = true;
}

void Intake::update() {
    if (antijam) {
        movingMutex.take();
        bool b = moving;
        movingMutex.give();
        if (b && (abs(bottom_intake.get_actual_velocity()) < 10)) {
            counter++;

            if (counter >= 4) {
                antijam = false;
                counter = 0;
                Task t{[&] {
                    bottom_intake.move(-40);
                    delay(150);
                    voltsMutex.take();
                    const auto _volts = volts;
                    voltsMutex.give();
                    bottom_intake.move(_volts);
                    antijam = true;
                }};
            }
        } else {
            counter = 0;
        }
    }
}

void Intake::bottom_forwards(int power) {
    double pct = power / 127.0;
    bottom_intake.move_velocity(600 * pct);

    voltsMutex.take();
    volts = power;
    voltsMutex.give();
    if (!antijam) {
        movingMutex.take();
        moving = true;
        movingMutex.give();
        return;
    }
    Task t{[&] {
        delay(100);
        movingMutex.take();
        moving = true;
        movingMutex.give();
    }};
}

void Intake::bottom_backwards(int power) {
    bottom_intake.move(-power);
    voltsMutex.take();
    volts = -power;
    voltsMutex.give();
    if (!antijam) {
        movingMutex.take();
        moving = true;
        movingMutex.give();
        return;
    }
    Task t{[&] {
        delay(100);
        movingMutex.take();
        moving = true;
        movingMutex.give();
    }};
}

void Intake::top_forwards(int power) {
    top_intake.move(power);
}

void Intake::top_backwards(int power) {
    top_intake.move(-power);
}

void Intake::stop() {
    bottom_intake.move(0);
    top_intake.move(0);
    moving = false;
}

void Intake::set_anti_jam(bool antijam) {
    this->antijam = antijam;
}