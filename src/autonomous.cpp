#include "autonomous.hpp"

#include "intake.hpp"

auto matchloader = ADIDigitalOut(MATCH_LOADER_PORT);

void auton::seven_wing_right(lemlib::Chassis& chassis) {
    Intake& intake = Intake::getInstance();
    intake.set_anti_jam(false);
    chassis.setPose(-49, -16, 90);
    intake.bottom_forwards(127);

    chassis.moveToPoint(-24, -24, 1000, {.forwards = true}, true);
    delay(430);
    matchloader.set_value(true);
    chassis.turnToPoint(-40,-52, 700);
    chassis.moveToPoint(-40,-52, 3000);
    intake.stop();
    chassis.turnToHeading(270, 1000);

    // alligned to matchloader

    chassis.moveToPoint(-60,-52, 3000,  {.maxSpeed = 60});
    intake.set_anti_jam(false);
    intake.bottom_forwards(127);
    left_motor_group.move(70);
    right_motor_group.move(70);

    delay(1000);

    // done matchloading

    chassis.moveToPoint(-24,-52, 4000, {.forwards = false}, false);
    intake.stop();
    intake.set_anti_jam(true);
    intake.bottom_forwards(127);
    intake.top_forwards(127);

    // scoring at the goal
    auto pose = chassis.getPose();
    std::cout 
    << "X: " << pose.x
    << " Y: " << pose.y
    << " Heading: " << pose.theta
    << std::endl;

    chassis.setPose(-25.1751 ,-51.1889, chassis.getPose().theta);

    delay(2000);
    chassis.moveToPoint(-36,-41, 3000, {.forwards = true}, false);
    intake.stop();
    chassis.turnToHeading(270, 1000);
    chassis.moveToPoint(-5,-41, 5000, {.forwards = false, .maxSpeed = 100}, false);


    

    // // set position to x:0, y:0, heading:0
    // chassis.setPose(0, 0, 0);
    // // turn to face heading 90 with a very long timeout
    // // chassis.turnToHeading(90, 100000);
    // chassis.moveToPoint(0, 24, 10000);
}