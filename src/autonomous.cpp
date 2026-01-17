#include "autonomous.hpp"

#include "intake.hpp"

auto matchloader = ADIDigitalOut(MATCH_LOADER_PORT);

void auton::seven_wing_right(lemlib::Chassis& chassis) {
    Intake& intake = Intake::getInstance();
    intake.set_anti_jam(false);
    chassis.setPose(-49, -16, 90);
    intake.bottom_forwards(127);

    chassis.moveToPoint(-23, -24, 1000, {.forwards = true}, true);
    delay(430);
    matchloader.set_value(true);
    chassis.turnToPoint(-40,-52, 700);
    chassis.moveToPoint(-40,-52, 3000);
    intake.stop();
    chassis.turnToHeading(270, 1500);

    // alligned to matchloader

    chassis.moveToPoint(-60,-52, 3000,  {.maxSpeed = 70});
    intake.set_anti_jam(false);
    chassis.tank(70,70);
    intake.bottom_forwards(127);

    delay(1000);

    // done matchloading

    chassis.moveToPoint(-24,-52, 4000, {.forwards = false}, false);
    intake.stop();
    intake.set_anti_jam(true);
    intake.bottom_forwards(127);
    intake.top_forwards(127);

    // scoring at the goal
    // auto pose = chassis.getPose();
    // std::cout 
    // << "X: " << pose.x
    // << " Y: " << pose.y
    // << " Heading: " << pose.theta
    // << std::endl;
    chassis.tank(-60,-60);
    delay(2000);
    chassis.tank(0,0);
    intake.stop();
    chassis.setPose(-25.1751 ,-51.1889, chassis.getPose().theta);


    chassis.moveToPoint(-36,-41, 1000, {.forwards = true}, false);
    intake.stop();
    chassis.turnToHeading(270, 700);
    chassis.moveToPoint(-6,-41, 5000, {.forwards = false, .maxSpeed = 60}, false);


    

    // // set position to x:0, y:0, heading:0
    // chassis.setPose(0, 0, 0);
    // // turn to face heading 90 with a very long timeout
    // // chassis.turnToHeading(90, 100000);
    // chassis.moveToPoint(0, 24, 10000);
}

void auton::four_wing_right(lemlib::Chassis& chassis) {
    Intake& intake = Intake::getInstance();
    intake.set_anti_jam(false);
    chassis.setPose(-49, -16, 90);
    intake.bottom_forwards(127);

chassis.moveToPoint(-23, -24, 1000, {.forwards = true, .maxSpeed = 110}, true);
    delay(430);
    matchloader.set_value(true);
    chassis.turnToPoint(-40,-52, 700);
    chassis.moveToPoint(-40,-52, 3000);
    intake.stop();
    chassis.turnToHeading(270, 1500);

    chassis.moveToPoint(-24,-52, 4000, {.forwards = false}, false);
    intake.stop();
    intake.set_anti_jam(true);
    intake.bottom_forwards(127);
    intake.top_forwards(127);

    // scoring at the goal
    // auto pose = chassis.getPose();
    // std::cout 
    // << "X: " << pose.x
    // << " Y: " << pose.y
    // << " Heading: " << pose.theta
    // << std::endl;
    chassis.tank(-60,-60);
    delay(2000);
    chassis.tank(0,0);
    intake.stop();
    chassis.setPose(-25.1751 ,-51.1889, chassis.getPose().theta);


    chassis.moveToPoint(-36,-41, 1000, {.forwards = true}, false);
    intake.stop();
    chassis.turnToHeading(270, 700);
    chassis.moveToPoint(-6,-41, 5000, {.forwards = false, .maxSpeed = 60}, false);


    

    // // set position to x:0, y:0, heading:0
    // chassis.setPose(0, 0, 0);
    // // turn to face heading 90 with a very long timeout
    // // chassis.turnToHeading(90, 100000);
    // chassis.moveToPoint(0, 24, 10000);
}

void auton::seven_wing_left(lemlib::Chassis& chassis) {
    Intake& intake = Intake::getInstance();
    intake.set_anti_jam(false);
    chassis.setPose(-49, 16, 90);
    intake.bottom_forwards(127);

    chassis.moveToPoint(-24, 23, 1000, {.forwards = true, .earlyExitRange = 0.1}, true);
    delay(440);
    matchloader.set_value(true);
    chassis.turnToPoint(-40,48, 700);
    chassis.moveToPoint(-40,48, 3000);
    intake.stop();
    chassis.turnToHeading(270, 1500);

    // alligned to matchloader

    chassis.moveToPoint(-60,48, 3000,  {.maxSpeed = 70});
    intake.set_anti_jam(false);
    chassis.tank(70,70);
    intake.bottom_forwards(127);
 
    delay(1000);

    // done matchloading
    chassis.turnToHeading(270, 1500);

    chassis.moveToPoint(-24,48, 4000, {.forwards = false, .maxSpeed = 82}, false);
    intake.stop();
    intake.set_anti_jam(true);
    intake.bottom_forwards(127);
    intake.top_forwards(127);

    // scoring at the goal
    // auto pose = chassis.getPose();
    // std::cout 
    // << "X: " << pose.x
    // << " Y: " << pose.y
    // << " Heading: " << pose.theta
    // << std::endl;
    chassis.tank(-60,-60);
    delay(2000);
    chassis.tank(0,0);
    chassis.setPose(-25.1751 ,51.1889, chassis.getPose().theta);
     intake.stop();

    chassis.moveToPoint(-36,61, 1000, {.forwards = true}, false);
    intake.stop();
    chassis.turnToHeading(270, 700);
    chassis.moveToPoint(-6,61, 5000, {.forwards = false, .maxSpeed = 60}, false);

}


void auton::four_wing_left(lemlib::Chassis& chassis) {
 Intake& intake = Intake::getInstance();
    intake.set_anti_jam(false);
    chassis.setPose(-49, 16, 90);
    intake.bottom_forwards(127);

    chassis.moveToPoint(-24, 23, 1000, {.forwards = true, .earlyExitRange = 0.1}, true);

    delay(440);
    matchloader.set_value(true);

    chassis.turnToPoint(-40, 48, 700, {.earlyExitRange = 0.1});
    chassis.moveToPoint(-40, 48, 3000, {.earlyExitRange = 0.1});

    intake.stop();
    chassis.turnToHeading(270, 1500, {.earlyExitRange = 0.1});

    chassis.moveToPoint(-24, 48, 4000,
        {.forwards = false, .maxSpeed = 82, .earlyExitRange = 0.1},
        false
    );
    intake.stop();
    intake.set_anti_jam(true);
    intake.bottom_forwards(127);
    intake.top_forwards(127);

    // scoring at the goal
    // auto pose = chassis.getPose();
    // std::cout 
    // << "X: " << pose.x
    // << " Y: " << pose.y
    // << " Heading: " << pose.theta
    // << std::endl;
    chassis.tank(-60,-60);
    delay(2000);
    chassis.tank(0,0);
    chassis.setPose(-25.1751 ,51.1889, chassis.getPose().theta);
     intake.stop();

    chassis.moveToPoint(-36,61, 1000, {.forwards = true}, false);
    intake.stop();
    chassis.turnToHeading(270, 700);
    chassis.moveToPoint(-6,61, 5000, {.forwards = false, .maxSpeed = 60}, false);

}

void auton::sawp(lemlib::Chassis& chassis) {
    Intake& intake = Intake::getInstance();
    intake.set_anti_jam(false);
    chassis.setPose(-50, -16, 0);
    matchloader.set_value(true);
    chassis.moveToPoint(-50,-50, 1000, {.forwards = false}, false);
    chassis.turnToHeading(270, 300);

    // alligned to matchloader
    intake.bottom_forwards(127);
    chassis.moveToPoint(-63,-52, 700,  {.maxSpeed = 70});
    chassis.tank(100,100);

    intake.set_anti_jam(false);
    chassis.turnToPoint(-24,-52, 400, {.forwards = false, .earlyExitRange = 0.1}, false);
    

    chassis.moveToPoint(-24,-52, 4000, {.forwards = false, .maxSpeed = 80}, true);
    delay(1200);
    intake.stop();
    intake.set_anti_jam(true);
    intake.bottom_forwards(127);    
    intake.top_forwards(127);
    chassis.tank(-60,-60);
    delay(1500);
    matchloader.set_value(false);
    chassis.tank(0,0);
    intake.stop();
    chassis.setPose(-25.1751 ,-51.1889, chassis.getPose().theta);

    //done scoring

    chassis.swingToPoint(-22,-30, DriveSide::RIGHT, 1000, {.maxSpeed = 80});
    intake.bottom_forwards(127);
    chassis.moveToPoint(-20,-28, 1000, {.forwards = true}, true);
    delay(300);
    matchloader.set_value(true);

    //intaking first stack

    chassis.turnToHeading(0, 500);
    matchloader.set_value(false);
    chassis.moveToPoint(-20,22, 2000, {.forwards = true, .maxSpeed = 80}, true);
    delay(800);
    matchloader.set_value(true);

    chassis.turnToPoint(-40, 40, 700, {.earlyExitRange = 0.1});

        //intaking second stack

    intake.stop();
    chassis.moveToPoint(-40, 40, 800, {.earlyExitRange = 0.1});
    chassis.turnToHeading(270, 300, {.earlyExitRange = 0.1});



    chassis.moveToPoint(-20,40, 1000, {.forwards = false, .maxSpeed = 80}, true);
    delay(1200);
    intake.set_anti_jam(true);
    intake.bottom_forwards(127);    
    intake.top_forwards(127);
    chassis.tank(-60,-60);
    delay(2000);
    matchloader.set_value(false);
    chassis.tank(0,0);
    intake.stop();
    chassis.setPose(-25.1751 ,47, chassis.getPose().theta);

    chassis.moveToPoint(-25.1751,25.1751,1000);
    chassis.turnToPoint(0,0,700, {.forwards = false});
    chassis.moveToPoint(14,14,3000, {.forwards = false});

} 