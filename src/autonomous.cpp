#include "autonomous.hpp"

void auton::example_auton(lemlib::Chassis& chassis) {
//     chassis.setPose(-49, -16, 90);

//     chassis.moveToPoint(-24, -24, 2000);

    // set position to x:0, y:0, heading:0
    chassis.setPose(0, 0, 0);
    // turn to face heading 90 with a very long timeout
    chassis.turnToHeading(90, 100000);
}