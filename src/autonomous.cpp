#include "autonomous.hpp"

#include "intake.hpp"
#include "distanceReset.hpp"

auto matchloader = ADIDigitalOut(MATCH_LOADER_PORT);
auto hood = ADIDigitalOut(HOOD_PORT);
auto midgoal = ADIDigitalOut(MIDGOAL_PORT);
auto left_dist = Distance(LEFT_DISTANCE_SENSOR);
auto right_dist = Distance(RIGHT_DISTANCE_SENSOR);

// util
float dist(lemlib::Chassis& chassis, float x, float y) {
    auto pose = chassis.getPose();
    auto poseX = pose.x;
    auto poseY = pose.y;

    return sqrt(pow(poseX - x, 2) + pow(poseY - y, 2));
}

void auton::four_wing_right(lemlib::Chassis& chassis) {
    Intake& intake = Intake::getInstance();
    intake.set_anti_jam(false);

    hood.set_value(true);
    midgoal.set_value(true);

    chassis.setPose(-49, -15.5, 90);

    intake.bottom_forwards();
    intake.top_forwards();

    chassis.moveToPoint(-23, -24, 750, {.forwards = true}, true);
    delay(430);
    matchloader.set_value(true);
    chassis.turnToPoint(-37,-52, 500);
    chassis.moveToPoint(-37,-52, 1000);

    chassis.turnToHeading(270, 250);
    chassis.waitUntilDone();

    // alligned to matchloader

    resetRobotPos(chassis, left_dist, "negative_y");
    
    chassis.moveToPoint(-24.5, -48.5, 1000, {.forwards=false, .maxSpeed=90});
    chassis.waitUntilDone();

    chassis.tank(-50, -50, true);

    hood.set_value(false);
    intake.top_forwards();
    intake.bottom_forwards();

    chassis.setPose(lemlib::Pose(-27.5, chassis.getPose().y, chassis.getPose().theta));
    resetRobotPos(chassis, left_dist, "negative_y");

    delay(750);
    matchloader.set_value(false);
    delay(250);

    chassis.moveToPoint(-36,-40.5, 1000, {.forwards = true}, false);
    intake.stop();
    chassis.turnToHeading(270, 300);

    intake.top_backwards();
    intake.bottom_backwards();

    chassis.moveToPoint(-12,-40.5, 10000, {.forwards = false, .maxSpeed = 60}, false);

    chassis.setBrakeMode(E_MOTOR_BRAKE_HOLD);
}

void auton::seven_wing_right(lemlib::Chassis& chassis) {
    Intake& intake = Intake::getInstance();
    intake.set_anti_jam(false);

    hood.set_value(true);
    midgoal.set_value(true);

    chassis.setPose(-49, -15.5, 90);

    intake.bottom_forwards();
    intake.top_forwards();

    chassis.moveToPoint(-23, -24, 750, {.forwards = true}, true);
    delay(430);
    matchloader.set_value(true);
    chassis.turnToPoint(-37,-52, 500);
    chassis.moveToPoint(-37,-52, 1000);

    chassis.turnToHeading(270, 250);
    chassis.waitUntilDone();

    // alligned to matchloader

    resetRobotPos(chassis, left_dist, "negative_y");
    
    intake.bottom_forwards();
    intake.top_forwards();

    chassis.moveToPoint(-60.5, -48.5, 1300, {.maxSpeed=80});
    chassis.waitUntilDone();

    chassis.tank(40, 40, true);
    delay(200); 

    resetRobotPos(chassis, left_dist, "negative_y");

    intake.stop_top();

    // done matchloading

    chassis.moveToPoint(-24.5, -48.5, 1000, {.forwards=false, .maxSpeed=90});
    chassis.waitUntilDone();

    hood.set_value(false);
    intake.top_forwards();
    intake.bottom_forwards();

    chassis.setPose(lemlib::Pose(-27.5, chassis.getPose().y, chassis.getPose().theta));
    resetRobotPos(chassis, left_dist, "negative_y");

    delay(750);
    matchloader.set_value(false);
    delay(750);

    chassis.moveToPoint(-36,-40.5, 1000, {.forwards = true}, false);
    intake.stop();
    chassis.turnToHeading(270, 700);

    intake.top_backwards();
    intake.bottom_backwards();

    chassis.moveToPoint(-12,-40.5, 10000, {.forwards = false, .maxSpeed = 60}, false);

    chassis.setBrakeMode(E_MOTOR_BRAKE_HOLD);
}

void auton::sawp(lemlib::Chassis& chassis) {
    Intake& intake = Intake::getInstance();
    intake.set_anti_jam(true);

    Task d([&] {
        delay(14900);
        intake.bottom_forwards();
        intake.top_forwards();
    });

    chassis.setPose(-46.5, -0.25, 0);
    
    intake.bottom_forwards();
    hood.set_value(true);
    midgoal.set_value(true);
    
    chassis.moveToPoint(-46.5, 5, 500, {.minSpeed=50});
    chassis.waitUntilDone();

    chassis.moveToPoint(-46.5, -46.5, 1250, {.forwards=false});
    chassis.waitUntilDone();

    intake.stop_bottom();

    matchloader.set_value(true);
    chassis.turnToHeading(270, 500);
    chassis.waitUntilDone();

    resetRobotPos(chassis, left_dist, "negative_y");
    
    intake.bottom_forwards();
    intake.top_forwards();

    chassis.moveToPoint(-60.5, -48.5, 2000, {.minSpeed=50});
    chassis.waitUntilDone();

    chassis.tank(80, 80, true);
    delay(200); 

    resetRobotPos(chassis, left_dist, "negative_y");

    chassis.turnToPoint(-24.5, -48.5, 300, {.forwards=false}, false);

    intake.stop_top();

    chassis.moveToPoint(-24.5, -48.5, 1000, {.forwards=false, .maxSpeed=90});
    chassis.waitUntilDone();

    hood.set_value(false);
    intake.top_forwards();
    intake.bottom_forwards();

    chassis.setPose(lemlib::Pose(-27.5, chassis.getPose().y, chassis.getPose().theta));
    resetRobotPos(chassis, left_dist, "negative_y");

    delay(750);
    matchloader.set_value(false);
    delay(500);

    chassis.swingToPoint(-15.5, -27.5, DriveSide::RIGHT, 1000);
    chassis.waitUntilDone();
    
    hood.set_value(true);

    Task d1([&] {
        delay(200);
        matchloader.set_value(true);
    });
    chassis.moveToPoint(-15.5, -30.5, 750);
    chassis.waitUntilDone();

    delay(200);
    matchloader.set_value(false);

    chassis.turnToPoint(-16.5, 21.5, 500);
    chassis.waitUntilDone();

    Task d2([&] {
        delay(600);
        matchloader.set_value(true);
    });
    chassis.moveToPoint(-21.5, 18.5, 1000);
    chassis.waitUntilDone();

    matchloader.set_value(false);

    // chassis.turnToPoint(-11, 5, 500, {.forwards=false});
    // chassis.waitUntilDone();

    chassis.moveToPose(-11, 5, 315, 1000, {.forwards=false, .minSpeed=60});
    chassis.waitUntilDone();

    chassis.tank(-30, -30, true);
    delay(500);

    midgoal.set_value(false);
    intake.bottom_forwards();
    intake.top_forwards(70);
    delay(400);

    intake.stop();
    midgoal.set_value(true);
    delay(100);

    chassis.moveToPoint(-43.5, 40.5, 1000);
    chassis.waitUntilDone();

    matchloader.set_value(true);

    chassis.turnToHeading(270, 300);
    chassis.waitUntilDone();

    resetRobotPos(chassis, right_dist, "positive_y");

    intake.bottom_forwards();
    intake.top_forwards();

    chassis.moveToPoint(-60.5, 48.5, 1500, {.minSpeed=50});
    chassis.waitUntilDone();

    chassis.tank(80, 80, true);
    delay(100);

    resetRobotPos(chassis, right_dist, "positive_y");

    intake.stop_top();

    chassis.moveToPoint(-24.5, 48.5, 1000, {.forwards=false, .maxSpeed=90});
    chassis.waitUntilDone();

    hood.set_value(false);
    intake.top_forwards();
    intake.bottom_forwards();

    delay(1000);
}

void auton::seven_wing_left(lemlib::Chassis& chassis) {
    Intake& intake = Intake::getInstance();
    intake.set_anti_jam(false);

    hood.set_value(true);
    midgoal.set_value(true);

    chassis.setPose(-49, 17.5, 90);

    intake.bottom_forwards();
    intake.top_forwards();

    chassis.moveToPoint(-23, 24, 750, {.forwards = true}, true);
    delay(430);
    matchloader.set_value(true);
    chassis.turnToPoint(-42, 52, 500);
    chassis.moveToPoint(-42, 52, 1000);

    chassis.turnToHeading(270, 250);
    chassis.waitUntilDone();

    // alligned to matchloader

    resetRobotPos(chassis, left_dist, "negative_y");
    
    intake.bottom_forwards();
    intake.top_forwards();

    chassis.moveToPoint(-65.5, 48.5, 1600, {.maxSpeed=100});
    chassis.waitUntilDone();

    chassis.tank(60, 60, true);
    delay(200); 

    resetRobotPos(chassis, right_dist, "positive_y");

    intake.stop_top();

    // done matchloading

    chassis.moveToPoint(-24.5, 48.5, 1000, {.forwards=false, .maxSpeed=90});
    chassis.waitUntilDone();

    hood.set_value(false);
    intake.top_forwards();
    intake.bottom_forwards();

    resetRobotPos(chassis, right_dist, "positive_y");
    chassis.setPose(lemlib::Pose(-27.5, -chassis.getPose().y, chassis.getPose().theta));

    delay(750);
    matchloader.set_value(false);
    delay(750);

    chassis.moveToPoint(-36,-40.5, 750, {.forwards = true}, false);
    intake.stop();
    chassis.turnToHeading(270, 300);

    intake.top_backwards();
    intake.bottom_backwards();

    chassis.moveToPoint(-12,-40.5, 10000, {.forwards = false, .maxSpeed = 60}, false);

    chassis.setBrakeMode(E_MOTOR_BRAKE_HOLD);
}

void auton::four_wing_left(lemlib::Chassis& chassis) {
    Intake& intake = Intake::getInstance();
    intake.set_anti_jam(false);

    hood.set_value(true);
    midgoal.set_value(true);

    chassis.setPose(-49, 17.5, 90);

    intake.bottom_forwards();
    intake.top_forwards();

    chassis.moveToPoint(-23, 24, 750, {.forwards = true}, true);
    delay(430);
    matchloader.set_value(true);
    chassis.turnToPoint(-42, 52, 500);
    chassis.moveToPoint(-42, 52, 1000);

    chassis.turnToHeading(270, 250);
    chassis.waitUntilDone();

    resetRobotPos(chassis, right_dist, "positive_y");

    // done matchloading

    chassis.moveToPoint(-24.5, 48.5, 800, {.forwards=false, .maxSpeed=90});
    chassis.waitUntilDone();

    hood.set_value(false);
    intake.top_forwards();
    intake.bottom_forwards();

    resetRobotPos(chassis, right_dist, "positive_y");
    chassis.setPose(lemlib::Pose(-27.5, -chassis.getPose().y, chassis.getPose().theta));

    delay(750);
    matchloader.set_value(false);
    delay(500);

    chassis.moveToPoint(-36,-40.5, 750, {.forwards = true}, false);
    intake.stop();
    chassis.turnToHeading(270, 300);

    intake.top_backwards();
    intake.bottom_backwards();

    chassis.moveToPoint(-12,-40.5, 10000, {.forwards = false, .maxSpeed = 60}, false);

    chassis.setBrakeMode(E_MOTOR_BRAKE_HOLD);
}