#include "autonomous.hpp"

#include "intake.hpp"
#include "distanceReset.hpp"
#include "pros/imu.hpp"

auto matchloader = ADIDigitalOut(MATCH_LOADER_PORT);
auto hood = ADIDigitalOut(HOOD_PORT);
auto midgoal = ADIDigitalOut(MIDGOAL_PORT);
auto left_dist = Distance(LEFT_DISTANCE_SENSOR);
auto right_dist = Distance(RIGHT_DISTANCE_SENSOR);
auto front_dist = Distance(FRONT_DISTANCE_SENSOR);
auto back_dist = Distance(BACK_DISTANCE_SENSOR);
auto wing = ADIDigitalOut(WING_PORT);
auto lift_intake = ADIDigitalOut(LIFT_INTAKE);

int sign(float value) {
    return value > 0 ? 1 : -1;
}

int CROSS_TARGET = 870;

void auton::auton_skills(lemlib::Chassis& chassis) {
    Intake& intake = Intake::getInstance();
    intake.set_anti_jam(false);

    hood.set_value(true);
    midgoal.set_value(true);
    wing.set_value(true);

    chassis.setPose(-62.8, -15.5, 0);

    intake.top_forwards();
    intake.bottom_forwards();

    Task matchloaderDelay([&](){
        delay(650);
        matchloader.set_value(true);
    });

    chassis.moveToPoint(-64.5, 34, 5000, {.maxSpeed=80, .minSpeed=65});
    chassis.waitUntilDone();

    // matchloader.set_value(false);

    int target = CROSS_TARGET;
    float error = front_dist.get_distance() - target;
    while (abs(error) > 5) {
        float power = error * 0.2;
        if (abs(power) < 15) {
            power = sign(power) * 15;
        }
        chassis.tank(power, power, true);
        error = front_dist.get_distance() - target;

        delay(5);
    }
    chassis.tank(0, 0, true);

    chassis.turnToHeading(0, 300);
    chassis.waitUntilDone();

    resetRobotPos(chassis, left_dist, "negative_x");
    resetRobotPos(chassis, front_dist, "positive_y");

    chassis.moveToPoint(-20, 0, 1500, {.forwards=false});
    chassis.waitUntilDone();

    matchloader.set_value(false);

    chassis.turnToPoint(-19, 16, 600, {.forwards=true});
    chassis.waitUntilDone();

    chassis.moveToPoint(-19, 16, 2000, {.forwards=true, .maxSpeed=60});;
    chassis.waitUntilDone();

    // Task mldelay0([&]() {
    //     delay(200);
    //     matchloader.set_value(false);
    // });

    // chassis.moveToPoint(-35, 35, 2000, {.forwards=false, .minSpeed=80});
    // chassis.waitUntilDone();
    
    // chassis.turnToPoint(-20, 20, 1000, {.forwards=true});
    // chassis.waitUntilDone();

    // chassis.moveToPoint(-29, 30, 750, {.forwards=true, .maxSpeed=60});
    // chassis.waitUntilDone();

    // chassis.turnToPoint(-14, 14, 300, {.forwards=false});
    // chassis.waitUntilDone();

    chassis.moveToPose(-6, 4, 315, 750, {.forwards=false, .maxSpeed=80});
    chassis.waitUntilDone();

    midgoal.set_value(false);
    delay(100);
    chassis.tank(-30, -30, true);
    intake.bottom_forwards();
    intake.top_intake.move_velocity(200);
    delay(500);
    chassis.tank(0, 0, true);
    intake.top_intake.move_velocity(275);
    delay(2850);

    intake.stop_top();

    chassis.moveToPoint(-42.8, 45, 1200);
    chassis.waitUntilDone();

    midgoal.set_value(true);
    matchloader.set_value(true);

    chassis.turnToHeading(270, 300);
    chassis.waitUntilDone();

    resetRobotPos(chassis, right_dist, "positive_y");
    // chassis.setPose(lemlib::Pose(-48.8, chassis.getPose().y, chassis.getPose().theta));

    intake.bottom_forwards();
    intake.top_forwards();

    // matchload 
    chassis.moveToPoint(-69.5, 46.5, 1500, {.maxSpeed=50});
    chassis.waitUntilDone();

    chassis.tank(30, 30, true);
    delay(750); 

    intake.stop_top();

    // done matchloading

    // chassis.moveToPoint(-40.5, 60, 1000, {.forwards=false, .minSpeed=80});
    chassis.moveToPose(-40.5, 57, 270, 500, {.forwards=false, .lead=0.1, .minSpeed=80});
    chassis.waitUntilDone();

    chassis.moveToPoint(35.5, 58, 2000, {.forwards=false});
    chassis.waitUntilDone();

    matchloader.set_value(false);
    intake.stop_top();

    chassis.turnToHeading(270, 200);
    chassis.waitUntilDone();

    resetRobotPos(chassis, right_dist, "positive_y");
    resetRobotPos(chassis, back_dist, "positive_x");

    chassis.swingToHeading(90, lemlib::DriveSide::LEFT, 500, {.direction=AngularDirection::CW_CLOCKWISE});
    chassis.waitUntilDone();

    // score

    // chassis.moveToPoint(24.5, 48.5, 1000, {.forwards=false, .maxSpeed=70});
    chassis.moveToPose(28.5, 48.5, 90, 1500, {.forwards=false, .maxSpeed=70, .minSpeed=15});
    chassis.waitUntilDone();

    hood.set_value(false);
    delay(50);
    intake.top_forwards();
    intake.bottom_forwards();

    matchloader.set_value(true);

    resetRobotPos(chassis, left_dist, "positive_y");

    // chassis.moveToPoint(-26.5, 48.5, 1200, {.forwards=false, .maxSpeed=60});
    // chassis.waitUntilDone();
    chassis.tank(-60, -60, true);
    delay(2500);
    chassis.tank(0, 0, true);

    // end scoring
    // start matchloading

    Task del1([&] {
        delay(500);
        hood.set_value(true);
    });

    // matchload
    chassis.moveToPoint(69.5, 45.5, 1500, {.maxSpeed=60});
    chassis.waitUntilDone();

    chassis.tank(30, 30, true);
    delay(1400); 

    resetRobotPos(chassis, left_dist, "positive_y");

    intake.stop_top();

    // done matchloading
    // score

    chassis.moveToPoint(24.5, 47.5, 1000, {.forwards=false, .maxSpeed=70});
    chassis.waitUntilDone();

    hood.set_value(false);
    delay(50);
    intake.top_forwards();
    intake.bottom_forwards();
    matchloader.set_value(false);

    chassis.moveToPoint(-24.5, 48.5, 2000, {.forwards=false, .maxSpeed=60});
    chassis.waitUntilDone();

    // HOOOD
    // chassis.moveToPoint(chassis.getPose().x + 5, chassis.getPose().y, 300, {.minSpeed=100});
    // chassis.waitUntilDone();

    // hood.set_value(true);

    // chassis.tank(-60, -60, true);
    // delay(200);
    // chassis.tank(0, 0, true);

    // done scoring

    intake.stop();

    // SECOND HALF
    resetRobotPos(chassis, left_dist, "positive_y");
    chassis.setPose(lemlib::Pose(27.5, chassis.getPose().y, 90));
    intake.top_forwards();
    intake.bottom_forwards();
    delay(100);

    chassis.moveToPose(62.5, 12.5, 180, 1750, {.lead=0.3});
    chassis.waitUntilDone();

    hood.set_value(true);

    intake.top_forwards();
    intake.bottom_forwards();

    Task matchloaderDelay2([&](){
        delay(1000);
        matchloader.set_value(true);
    });

    chassis.moveToPoint(64.5, -40, 5000, {.maxSpeed=75, .minSpeed=60});
    chassis.waitUntilDone();

    error = front_dist.get_distance() - target;
    while (abs(error) > 5) {
        float power = error * 0.2;
        if (abs(power) < 15) {
            power = sign(power) * 15;
        }
        chassis.tank(power, power, true);
        error = front_dist.get_distance() - target;

        delay(5);
    }
    chassis.tank(0, 0, true);
    
    chassis.turnToHeading(180, 300);
    chassis.waitUntilDone();

    resetRobotPos(chassis, left_dist, "positive_x");
    resetRobotPos(chassis, front_dist, "negative_y");

    // delay(300);

    chassis.moveToPoint(20, 0, 1500, {.forwards=false});
    chassis.waitUntilDone();

    matchloader.set_value(false);

    chassis.turnToPoint(18, 14, 600, {.forwards=true});
    chassis.waitUntilDone();

    chassis.moveToPoint(18, 16, 2000, {.forwards=true, .maxSpeed=80});;
    chassis.waitUntilDone();

    chassis.turnToPoint(0, 0, 500, {.forwards=true});
    chassis.waitUntilDone();

    chassis.moveToPoint(8, 12, 700, {.forwards=true, .maxSpeed=40});;
    chassis.waitUntilDone();

    lift_intake.set_value(true);

    chassis.tank(30, 30, true);
    delay(500);

    intake.top_backwards();
    intake.bottom_intake.move_velocity(-300);
    chassis.tank(0, 30, true);
    delay(500);
    intake.bottom_intake.move_velocity(-360);
    delay(1500);
    intake.bottom_intake.move_velocity(-180);
    delay(1500);

    chassis.tank(-60, -60, true);
    delay(500);
    chassis.tank(0, 0, true);
    // chassis.moveToPoint(17, 17, 500, {.forwards=false});
    // chassis.waitUntilDone();

    intake.top_backwards();
    intake.bottom_backwards();
    lift_intake.set_value(false);

    // GO ACROSS

    hood.set_value(true);

    chassis.turnToPoint(37.8, -53.5, 500, {.forwards=false});
    chassis.waitUntilDone();

    chassis.moveToPoint(37.8, -53.5, 2000, {.forwards=false});
    chassis.waitUntilDone();

    intake.stop();

    chassis.turnToHeading(0, 300);
    chassis.waitUntilDone();

    resetRobotPos(chassis, back_dist, "negative_y");

    matchloader.set_value(true);
    chassis.turnToHeading(90, 500);
    chassis.waitUntilDone();

    // start matchloading

    resetRobotPos(chassis, right_dist, "negative_y");

    intake.bottom_forwards();
    intake.top_forwards();

    // matchload
    chassis.moveToPoint(69.5, -48.5, 1500, {.maxSpeed=50});
    chassis.waitUntilDone();

    chassis.tank(30, 30, true);
    delay(750); 

    intake.stop_top();

    // done matchloading

    chassis.moveToPose(40.5, -56, 0, 2000, {.forwards=false, .lead=0.01, .minSpeed=80});
    chassis.waitUntilDone();

    chassis.moveToPoint(-38.5, -58, 2000, {.forwards=false});
    chassis.waitUntilDone();

    matchloader.set_value(false);
    intake.stop();

    chassis.turnToHeading(90, 300);
    chassis.waitUntilDone();

    resetRobotPos(chassis, right_dist, "negative_y");
    resetRobotPos(chassis, back_dist, "negative_x");

    chassis.swingToHeading(270, lemlib::DriveSide::LEFT, 500, {.direction=AngularDirection::CW_CLOCKWISE});
    chassis.waitUntilDone();

    // score

    chassis.moveToPose(-24.5, -48.5, 270, 1000, {.forwards=false, .maxSpeed=70, .minSpeed=30});
    chassis.waitUntilDone();

    hood.set_value(false);
    delay(50);
    intake.top_forwards();
    intake.bottom_forwards();

    matchloader.set_value(true);

    // resetRobotPos(chassis, left_dist, "negative_y");

    chassis.moveToPoint(24.5, -48.5, 1500, {.forwards=false, .maxSpeed=60});
    chassis.waitUntilDone();

    // delay(700);
    // chassis.tank(0, 0, true);
    // delay(800);

    Task del2([] {
        delay(500);
        hood.set_value(true);
    });

    // end scoring
    // start matchloading

    // matchload
    chassis.moveToPoint(-67.5, -45.5, 1500, {.maxSpeed=60});
    chassis.waitUntilDone();

    chassis.tank(30, 30, true);
    delay(1250); 

    // resetRobotPos(chassis, left_dist, "negative_y");

    intake.stop_top();

    // done matchloading
    // score

    chassis.moveToPoint(-24.5, -47.5, 1000, {.forwards=false, .maxSpeed=70});
    chassis.waitUntilDone();

    hood.set_value(false);
    delay(50);
    intake.top_forwards();
    intake.bottom_forwards();
    matchloader.set_value(false);

    chassis.moveToPoint(24.5, -48.5, 2100, {.forwards=false, .maxSpeed=60});
    chassis.waitUntilDone();

    // HOOD
    // chassis.moveToPoint(chassis.getPose().x - 5, chassis.getPose().y, 300, {.minSpeed=100});
    // chassis.waitUntilDone();

    // hood.set_value(true);

    // chassis.tank(-60, -60, true);
    // delay(300);
    // chassis.tank(0, 0, true);

    // done scoring
    // park

    chassis.setPose(lemlib::Pose(-27.5, chassis.getPose().y, chassis.getPose().theta));

    chassis.moveToPose(-62.5, -12.5, 0, 1750, {.lead=0.3, .minSpeed=100, .earlyExitRange=2});
    chassis.waitUntilDone();

    intake.bottom_forwards();
    intake.top_backwards();
    hood.set_value(true);

    chassis.moveToPoint(-64, 12, 5000, {.minSpeed=80});
    chassis.waitUntilDone();

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