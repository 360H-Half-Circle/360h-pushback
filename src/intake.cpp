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
    // bottom_intake.set_reversed(true);
    top_intake.set_reversed(true);

    antijam = true;
    initialized = true;
}

void Intake::update() {
    if (antijam) {
        
    }
}

void Intake::bottom_forwards(int power) {
    bottom_intake.move(power);
}

void Intake::bottom_backwards(int power) {    
    bottom_intake.move(-power);
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