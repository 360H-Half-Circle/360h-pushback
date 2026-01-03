#include "autonomous.hpp"

void auton::example_auton(lemlib::Chassis& chassis) {
    chassis.setPose(0, 0, 0);

    chassis.moveToPoint(40, 0, 2000);
}