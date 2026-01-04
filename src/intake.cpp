#include "intake.hpp"
#include "controls.hpp"
#include <iostream>

Controller master(E_CONTROLLER_MASTER);

void Intake::initialize() {
    if (initialized) return;

    bottom_intake.set_gearing(E_MOTOR_GEAR_BLUE);
    top_intake.set_gearing(E_MOTOR_GEAR_BLUE);

    // Make "forwards" in code match physical forwards.
    // If your intake spins the wrong way, flip these booleans.
    bottom_intake.set_reversed(true);
    top_intake.set_reversed(true);

    antijam = true;
    initialized = true;
}

void Intake::update() {
    if (antijam) {
        if (moving && (abs(bottom_intake.get_actual_velocity()) < 15)) {
            counter++;

            if (counter >= 5) {
                antijam = false;
                counter = 0;
                Task t{[&] {
                    bottom_intake.move(-127);
                    delay(70);

                    std::cout << "[Intake] Anti-jam activated\n";

                    if (master.get_digital(E_CONTROLLER_DIGITAL_R1)) {
                        bottom_intake.move(volts);
                    } else {
                        bottom_intake.move(0);
                    }
                    antijam = true;
                }};
            }
        } else {
            counter = 0;
        }
    }
}

void Intake::bottom_forwards(int power) {
    if (moving) return;

    double pct = power / 127.0;
    bottom_intake.move_velocity(600 * pct);

    volts = power;
    moving = true;
}

void Intake::bottom_backwards(int power) {
    if (moving) return;
    
    bottom_intake.move(-power);

    volts = -power;
    moving = true;
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