#include "autonomous.hpp"

#include "intake.hpp"
#include "distanceReset.hpp"

auto matchloader = ADIDigitalOut(MATCH_LOADER_PORT);
auto hood = ADIDigitalOut(HOOD_PORT);
auto midgoal = ADIDigitalOut(MIDGOAL_PORT);
auto left_dist = Distance(LEFT_DISTANCE_SENSOR);
auto right_dist = Distance(RIGHT_DISTANCE_SENSOR);
auto front_dist = Distance(FRONT_DISTANCE_SENSOR);
auto back_dist = Distance(BACK_DISTANCE_SENSOR);
auto wing = ADIDigitalOut(WING_PORT);

void auton::auton_skills(lemlib::Chassis& chassis) {
    Intake& intake = Intake::getInstance();
    intake.set_anti_jam(false);

    hood.set_value(true);
    midgoal.set_value(true);
    wing.set_value(true);

    chassis.setPose(-48.8, 15.8, 0);

    chassis.moveToPoint(-48.8, 45, 1000);
    chassis.waitUntilDone();

    matchloader.set_value(true);

    chassis.turnToHeading(270, 700);
    chassis.waitUntilDone();

    delay(100);

    resetRobotPos(chassis, right_dist, "positive_y");

    intake.bottom_forwards();
    intake.top_forwards();

    chassis.moveToPoint(-65.5, 48.5, 1500, {.maxSpeed=70});
    chassis.waitUntilDone();

    chassis.tank(30, 30, true);
    delay(1000); 

    intake.stop_top();

    // done matchloading

    chassis.moveToPoint(-48.8, 45, 1000, {.forwards=false});
    chassis.waitUntilDone();

    chassis.moveToPoint(-40.5, 60, 1000, {.forwards=false});
    chassis.waitUntilDone();

    chassis.turnToPoint(40.5, 60, 500, {.forwards=false});
    chassis.waitUntilDone();

    chassis.moveToPoint(35.5, 60, 2000, {.forwards=false});
    chassis.waitUntilDone();

    matchloader.set_value(false);
    intake.stop();

    chassis.turnToHeading(270, 500);
    chassis.waitUntilDone();

    delay(100);

    resetRobotPos(chassis, right_dist, "positive_y");
    resetRobotPos(chassis, back_dist, "positive_x");

    chassis.turnToPoint(48.8, 48.5, 500, {.forwards=false});
    chassis.waitUntilDone();

    chassis.moveToPoint(48.8, 48.5, 1000, {.forwards=false});
    chassis.waitUntilDone();

    chassis.turnToHeading(90, 500);
    chassis.waitUntilDone();

    // score

    chassis.moveToPoint(24.5, 48.5, 1000, {.forwards=false, .maxSpeed=70});
    chassis.waitUntilDone();

    hood.set_value(false);
    intake.top_forwards();
    intake.bottom_forwards();

    matchloader.set_value(true);

    resetRobotPos(chassis, left_dist, "positive_y");

    chassis.tank(-60, -60, true);
    delay(700);
    chassis.tank(0, 0, true);
    delay(1200);

    // end scoring
    // start matchloading

    Task del1([&] {
        delay(500);
        hood.set_value(true);
    });

    chassis.moveToPoint(65.5, 47.5, 1500, {.maxSpeed=70});
    chassis.waitUntilDone();

    chassis.tank(60, 60, true);
    delay(1800); 

    resetRobotPos(chassis, left_dist, "positive_y");

    intake.stop_top();

    // done matchloading
    // score

    chassis.moveToPoint(24.5, 47.5, 1000, {.forwards=false, .maxSpeed=70});
    chassis.waitUntilDone();

    hood.set_value(false);
    intake.top_forwards();
    intake.bottom_forwards();
    matchloader.set_value(false);

    chassis.tank(-60, -60, true);
    delay(700);
    chassis.tank(0, 0, true);
    delay(1800);

    chassis.moveToPoint(chassis.getPose().x + 5, chassis.getPose().y, 1000);
    chassis.waitUntilDone();

    hood.set_value(true);

    chassis.tank(-30, -30, true);
    delay(750);
    chassis.tank(0, 0, true);

    // done scoring

    // GO ACROSS

    chassis.setPose(lemlib::Pose(27.5, chassis.getPose().y, chassis.getPose().theta));

    chassis.moveToPoint(40.8, 48.5, 1500, {.forwards=true});
    chassis.waitUntilDone();

    hood.set_value(true);

    chassis.turnToPoint(40.8, -48.5, 500, {.forwards=false});
    chassis.waitUntilDone();

    chassis.moveToPoint(40.8, -48.5, 2000, {.forwards=false});
    chassis.waitUntilDone();

    matchloader.set_value(true);
    chassis.turnToHeading(90, 500);
    chassis.waitUntilDone();

    delay(100);

    // start matchloading

    resetRobotPos(chassis, right_dist, "negative_y");

    intake.bottom_forwards();
    intake.top_forwards();

    chassis.moveToPoint(65.5, -48.5, 1500, {.maxSpeed=70});
    chassis.waitUntilDone();

    chassis.tank(30, 30, true);
    delay(1000); 

    intake.stop_top();

    // done matchloading

    chassis.moveToPoint(48.8, -45, 1000, {.forwards=false});
    chassis.waitUntilDone();

    chassis.moveToPoint(40.5, -60, 1000, {.forwards=false});
    chassis.waitUntilDone();

    chassis.turnToPoint(-40.5, -60, 500, {.forwards=false});
    chassis.waitUntilDone();

    chassis.moveToPoint(-35.5, -60, 2000, {.forwards=false});
    chassis.waitUntilDone();

    matchloader.set_value(false);
    intake.stop();

    chassis.turnToHeading(90, 500);
    chassis.waitUntilDone();

    delay(100);

    resetRobotPos(chassis, right_dist, "negative_y");
    resetRobotPos(chassis, back_dist, "negative_x");

    chassis.turnToPoint(-48.8, -48.5, 500, {.forwards=false});
    chassis.waitUntilDone();

    chassis.moveToPoint(-48.8, -48.5, 1000, {.forwards=false});
    chassis.waitUntilDone();

    chassis.turnToHeading(270, 500);
    chassis.waitUntilDone();

    // score

    chassis.moveToPoint(-24.5, -48.5, 1000, {.forwards=false, .maxSpeed=70});
    chassis.waitUntilDone();

    hood.set_value(false);
    intake.top_forwards();
    intake.bottom_forwards();

    matchloader.set_value(true);

    resetRobotPos(chassis, left_dist, "negative_y");

    chassis.tank(-60, -60, true);
    delay(200);
    chassis.tank(0, 0, true);
    delay(1300);

    Task del2([] {
        delay(500);
        hood.set_value(true);
    });

    // end scoring
    // start matchloading

    chassis.moveToPoint(-65.5, -47.5, 1500, {.maxSpeed=60});
    chassis.waitUntilDone();

    chassis.tank(60, 60, true);
    delay(1500); 

    resetRobotPos(chassis, left_dist, "negative_y");

    intake.stop_top();

    // done matchloading
    // score

    chassis.moveToPoint(-24.5, -47.5, 1000, {.forwards=false, .maxSpeed=70});
    chassis.waitUntilDone();

    hood.set_value(false);
    intake.top_forwards();
    intake.bottom_forwards();
    matchloader.set_value(false);

    chassis.tank(-60, -60, true);
    delay(700);
    chassis.tank(0, 0, true);
    delay(1200);

    chassis.moveToPoint(chassis.getPose().x - 5, chassis.getPose().y, 1000);
    chassis.waitUntilDone();

    hood.set_value(true);

    chassis.tank(-30, -30, true);
    delay(750);
    // chassis.tank(0, 0, true);

    // done scoring
    // park

    chassis.setPose(lemlib::Pose(-27.5, chassis.getPose().y, chassis.getPose().theta));

    chassis.moveToPoint(-46.5, -48.5, 1000);
    chassis.waitUntilDone();

    intake.stop();

    chassis.turnToPoint(-64, -18.5, 500);
    chassis.waitUntilDone();

    chassis.moveToPoint(-64, -18.5, 1500);
    chassis.waitUntilDone();

    chassis.swingToHeading(0, DriveSide::RIGHT, 500);
    chassis.waitUntilDone();

    matchloader.set_value(true);
    delay(100);

    intake.bottom_forwards();
    intake.top_forwards();
    hood.set_value(true);

    chassis.tank(80, 80, true);
    delay(800);
    chassis.tank(0, 0, true);

    delay(5000);
}

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

    // done matchloading

    chassis.moveToPoint(-24.5, -46.5, 500, {.forwards=false, .maxSpeed=90});
    chassis.waitUntilDone();

    hood.set_value(false);
    intake.top_forwards();
    intake.bottom_forwards();
    
    chassis.tank(-60, -60, true);
    delay(750);
    
    chassis.tank(0, 0, true);
    matchloader.set_value(false);
    chassis.setPose(lemlib::Pose(-27.5, chassis.getPose().y, chassis.getPose().theta));
    resetRobotPos(chassis, left_dist, "negative_y");
    
    delay(250);

    chassis.moveToPoint(-36,-37, 750, {.forwards = true}, false);
    // intake.stop();
    chassis.turnToHeading(270, 300);

    intake.top_backwards();
    intake.bottom_backwards();

    chassis.moveToPoint(-11,-37, 10000, {.forwards=false, .maxSpeed=127}, false);

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

    chassis.moveToPoint(-60.5, -46.5, 750, {.maxSpeed=60});
    chassis.waitUntilDone();

    chassis.tank(40, 40, true);
    delay(300); 

    resetRobotPos(chassis, left_dist, "negative_y");

    intake.stop_top();

    // done matchloading

    chassis.moveToPoint(-24.5, -46.5, 1000, {.forwards=false, .maxSpeed=90});
    chassis.waitUntilDone();

    hood.set_value(false);
    intake.top_forwards();
    intake.bottom_forwards();

    chassis.tank(-60, -60, true);
    delay(750);

    chassis.tank(0, 0, true);
    matchloader.set_value(false);
    chassis.setPose(lemlib::Pose(-27.5, chassis.getPose().y, chassis.getPose().theta));
    resetRobotPos(chassis, left_dist, "negative_y");
    
    delay(750);

    chassis.moveToPoint(-36,-38, 750, {.forwards = true}, false);
    intake.stop();
    chassis.turnToHeading(270, 300);

    intake.top_backwards();
    intake.bottom_backwards();

    chassis.moveToPoint(-14,-38, 10000, {.forwards=false, .maxSpeed=60}, false);

    chassis.tank(20, 20, true);
    delay(200);
    chassis.tank(0, 0, true);

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

    chassis.moveToPoint(-60.5, -46.5, 500, {.maxSpeed=60});
    chassis.waitUntilDone();

    chassis.tank(30, 30, true);
    delay(200); 

    resetRobotPos(chassis, left_dist, "negative_y");

    chassis.turnToPoint(-24.5, -48.5, 300, {.forwards=false}, false);

    chassis.moveToPoint(-24.5, -48.5, 1000, {.forwards=false, .maxSpeed=70});
    chassis.waitUntilDone();

    hood.set_value(false);
    intake.top_forwards();
    intake.bottom_forwards();

    chassis.tank(-60, -60, true);
    delay(750);

    chassis.setPose(lemlib::Pose(-27.5, chassis.getPose().y, chassis.getPose().theta));
    resetRobotPos(chassis, left_dist, "negative_y");

    matchloader.set_value(false);
    delay(500);

    chassis.swingToPoint(-22.5, -22.5, DriveSide::RIGHT, 1000);
    chassis.waitUntilDone();
    
    hood.set_value(true);

    Task d1([&] {
        delay(200);
        matchloader.set_value(true);
    });
    chassis.moveToPoint(-20.5, -20.5, 500);
    chassis.waitUntilDone();

    // delay(100);
    matchloader.set_value(false);

    chassis.turnToPoint(-24.5, 24.5, 500);
    chassis.waitUntilDone();

    Task d2([&] {
        delay(800);
        matchloader.set_value(true);
    });
    chassis.moveToPoint(-24.5, 24.5, 1000);
    chassis.waitUntilDone();

    matchloader.set_value(false);

    chassis.moveToPose(-14, 14, 315, 1000, {.forwards=false, .minSpeed=60});
    chassis.waitUntilDone();

    chassis.tank(-30, -30, true);
    delay(500);

    midgoal.set_value(false);
    intake.bottom_forwards();
    intake.top_forwards(90);
    delay(500);

    intake.stop();
    midgoal.set_value(true);
    delay(100);

    chassis.moveToPoint(-48.5, 46.5, 1000);
    chassis.waitUntilDone();

    matchloader.set_value(true);

    chassis.turnToHeading(270, 300);
    chassis.waitUntilDone();

    resetRobotPos(chassis, right_dist, "positive_y");

    intake.bottom_forwards();
    intake.top_forwards();

    chassis.moveToPoint(-60.5, 48.5, 500, {.maxSpeed=70});
    chassis.waitUntilDone();

    chassis.tank(30, 30, true);
    delay(200);

    resetRobotPos(chassis, right_dist, "positive_y");

    chassis.moveToPoint(-24.5, 46.5, 1000, {.forwards=false, .maxSpeed=70});
    chassis.waitUntilDone();

    hood.set_value(false);
    intake.top_forwards();
    intake.bottom_forwards();

    chassis.tank(-80, -80, true);
    delay(1000);
}

void auton::seven_wing_left(lemlib::Chassis& chassis) {
    Intake& intake = Intake::getInstance();
    intake.set_anti_jam(false);

    hood.set_value(true);
    midgoal.set_value(true);

    chassis.setPose(-49, 15.5, 90);

    intake.bottom_forwards();
    intake.top_forwards();

    chassis.moveToPoint(-23, 24, 750, {.forwards = true}, true);
    delay(430);
    matchloader.set_value(true);
    chassis.turnToPoint(-40,52, 500);
    chassis.moveToPoint(-40,52, 1000);

    chassis.turnToHeading(270, 250);
    chassis.waitUntilDone();

    // alligned to matchloader

    resetRobotPos(chassis, right_dist, "positive_y");
    
    intake.bottom_forwards();
    intake.top_forwards();

    chassis.moveToPoint(-60.5, 46.5, 750, {.maxSpeed=60});
    chassis.waitUntilDone();

    chassis.tank(40, 40, true);
    delay(300); 

    resetRobotPos(chassis, right_dist, "positive_y");

    intake.stop_top();

    // done matchloading

    chassis.moveToPoint(-24.5, 48.5, 1000, {.forwards=false, .maxSpeed=90});
    chassis.waitUntilDone();

    hood.set_value(false);
    intake.top_forwards();
    intake.bottom_forwards();

    chassis.tank(-60, -60, true);
    delay(750);

    chassis.tank(0, 0, true);
    matchloader.set_value(false);
    resetRobotPos(chassis, right_dist, "positive_y");
    chassis.setPose(lemlib::Pose(-27.5, -chassis.getPose().y, chassis.getPose().theta));

    delay(750);

    chassis.moveToPoint(-36,-38, 750, {.forwards = true}, false);
    intake.stop();
    chassis.turnToHeading(270, 300);

    intake.top_backwards();
    intake.bottom_backwards();

    chassis.moveToPoint(-14,-38, 10000, {.forwards=false, .maxSpeed=60}, false);

    chassis.tank(20, 20, true);
    delay(200);
    chassis.tank(0, 0, true);

    chassis.setBrakeMode(E_MOTOR_BRAKE_HOLD);
}

void auton::four_wing_left(lemlib::Chassis& chassis) {
    Intake& intake = Intake::getInstance();
    intake.set_anti_jam(false);

    hood.set_value(true);
    midgoal.set_value(true);

    chassis.setPose(-49, 15.5, 90);

    intake.bottom_forwards();
    intake.top_forwards();

    chassis.moveToPoint(-23, 24, 750, {.forwards = true}, true);
    delay(430);
    matchloader.set_value(true);
    chassis.turnToPoint(-40,48, 500);
    chassis.moveToPoint(-40,48, 1000);

    chassis.turnToHeading(270, 250);
    chassis.waitUntilDone();

    // alligned to matchloader

    resetRobotPos(chassis, right_dist, "positive_y");
    
    // done matchloading

    chassis.moveToPoint(-24.5, 48.5, 500, {.forwards=false, .maxSpeed=90});
    chassis.waitUntilDone();

    hood.set_value(false);
    intake.top_forwards();
    intake.bottom_forwards();

    chassis.tank(-60, -60, true);
    delay(750);

    chassis.tank(0, 0, true);
    matchloader.set_value(false);
    resetRobotPos(chassis, right_dist, "positive_y");
    chassis.setPose(lemlib::Pose(-27.5, -chassis.getPose().y, chassis.getPose().theta));

    delay(250);

    chassis.moveToPoint(-36,-37, 750, {.forwards = true}, false);
    intake.stop();
    chassis.turnToHeading(270, 300);

    intake.top_backwards();
    intake.bottom_backwards();

    chassis.moveToPoint(-11,-37, 10000, {.forwards=false, .maxSpeed=60}, false);

    chassis.setBrakeMode(E_MOTOR_BRAKE_HOLD);
}