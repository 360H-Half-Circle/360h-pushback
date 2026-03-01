#include "main.h"
#include "controls.hpp"
#include "lemlib/api.hpp"
#include "selector.hpp"
#include <unordered_set>
#include "intake.hpp"
#include "autonomous.hpp"
#include "distanceReset.hpp"
#include "miku/mcl.hpp"

#define TRACK_WIDTH 11.125
#define IS_DRIVER_SKILLS true

using namespace pros::c;

static Controller master(E_CONTROLLER_MASTER);

static MotorGroup left_motor_group({L_DRIVE_FRONT, -L_DRIVE_MID, -L_DRIVE_BACK}, MotorGears::blue, MotorUnits::degrees);
static MotorGroup right_motor_group({-R_DRIVE_FRONT, R_DRIVE_MID, R_DRIVE_BACK}, MotorGears::blue, MotorUnits::degrees);
static auto imu = Imu(INERTIAL_PORT);

lemlib::Drivetrain drivetrain(
    &left_motor_group, // left motor group
    &right_motor_group,// right motor group
    TRACK_WIDTH, // 10 inch track width
    lemlib::Omniwheel::NEW_325, // using new 4" omnis
    450, // drivetrain rpm is 360
    2 // horizontal drift is 2 (for now)
);

lemlib::ControllerSettings lateral_controller(
    10, // proportional gain (kP)
    0, // integral gain (kI)
    40, // derivative gain (kD)
    3, // anti windup
    0.25, // small error range, in inches
    100, // small error range timeout, in millisecond
    3, // large error range, in inches
    500, // large error range timeout, in milliseconds
    50 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(
    3, // proportional gain (kP)
    0.1, // integral gain (kI)
    21, // derivative gain (kD)
    3, // anti windup
    1, // small error range, in degrees
    100, // small error range timeout, in milliseconds
    3, // large error range, in degrees
    500, // large error range timeout, in milliseconds
    0 // maximum acceleration (slew)
);

pros::Rotation perp_sens(-PERP_TRACKING_WHEEL);
lemlib::TrackingWheel horizontal_tracking_wheel(&perp_sens, lemlib::Omniwheel::NEW_2, -1.5);

lemlib::OdomSensors sensors(
    nullptr, // vertical tracking wheel 1, set to null
    nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
    &horizontal_tracking_wheel, // horizontal tracking wheel 1
    nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
    &imu // inertial sensor
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttle_curve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steer_curve(0, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

lemlib::Chassis chassis(drivetrain, // drivetrain settings
    lateral_controller, // lateral PID settings
    angular_controller, // angular PID settings
    sensors, // odometry sensors
    &throttle_curve, // throttle input curve
    &steer_curve // steer input curve
);

static lemlib::Chassis& getChassis() {
    return chassis;
}

Intake& intake = Intake::getInstance();

miku::Distance front_distance(FRONT_DISTANCE_SENSOR, 0, 0, 0.0f);                // front (0)
miku::Distance back_distance(BACK_DISTANCE_SENSOR, 0, 0, M_PI);                  // back 
miku::Distance left_distance(LEFT_DISTANCE_SENSOR, 0, 0, -M_PI_2);                 // left (-90deg)
miku::Distance right_distance(RIGHT_DISTANCE_SENSOR, 0, 0, M_PI_2);                 // right (90deg)

ParticleFilter mcl({
    std::make_shared<miku::Distance>(left_distance),
    std::make_shared<miku::Distance>(right_distance),
    std::make_shared<miku::Distance>(front_distance),
    std::make_shared<miku::Distance>(back_distance)
}, chassis);

float prev_left_raw = 0.0f;
float prev_right_raw = 0.0f;
compass_degrees prev_theta_raw = 0.0f;
#define WHEEL_DIAMETER 3.25f
#define GEAR_RATIO (36.0f / 48.0f)

float average_position(MotorGroup& group) {
    std::vector<pros::MotorGears> gearsets = group.get_gearing_all();
    std::vector<double> positions = group.get_position_all();
    std::vector<float> distances;
    for (int i = 0; i < group.size(); i++) {
        float in;
        switch (gearsets[i]) {
            case pros::MotorGears::red: in = 100; break;
            case pros::MotorGears::green: in = 200; break;
            case pros::MotorGears::blue: in = 600; break;
            default: in = 200; break;
        }
        distances.push_back(positions[i] * (WHEEL_DIAMETER * M_PI) * (450 / in));
    }

    float sum = 0;
    for (auto d : distances) {
        sum += d;
    }

    return sum/distances.size();
}

void initialize() {
    chassis.calibrate(true);
    // delay(1000);

    intake.initialize();
    lcd::initialize();
    // sec::init();
    
    pros::Task screen_task([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            pros::lcd::print(3, "Heading %f", imu.get_heading()); // heading
            pros::lcd::print(4, "Pitch %f", imu.get_pitch()); // heading
            pros::lcd::print(5, "Roll %f", imu.get_roll()); // heading
            // delay to save resources
            pros::delay(20);
        }
    });
}

void disabled() { }

void competition_initialize() {}

/*

1: sawp
2: skills
3: RIGHT 4W
4: RIGHT 7W
5: LEFT 4W
6: LEFT 7W

*/

void autonomous() {
    // auton::sawp(chassis);
    // auton::seven_wing_right(chassis);
    // auton::seven_wing_left(chassis);
    // auton::four_wing_left(chassis);
    // auton::four_wing_right(chassis);
    // auton::sawp_safe(chassis);
    // auton::nine_split_right(chassis);
    auton::auton_skills(chassis);
    // auton::seven_split_left(chassis);
    // auton::seven_split_right(chassis);
    return;

    // LEFT AUTOS
    if (sec::auton == "R4WL" || sec::auton == "B4WL") {
        auton::four_wing_left(chassis);
    } 
    if (sec::auton == "R7WL" || sec::auton == "B7WL") {
        auton::seven_wing_left(chassis);
    } 

    // RIGHT AUTOS
    if (sec::auton == "R4WR" || sec::auton == "B4WR") {
        auton::four_wing_right(chassis);
    } 
    if (sec::auton == "R7WR" || sec::auton == "B7WR") {
        auton::seven_wing_right(chassis);
    } 

    // SOLO AWP
    if (sec::auton == "R SAWP" || sec::auton == "B SAWP") {
        auton::sawp(chassis);
    } 
}

bool killcode = false;

void opcontrol() {
    // return;
    // autonomous();

    Motor bottom_intake = Motor(-BOTTOM_INTAKE);
    Motor top_intake = Motor(-TOP_INTAKE);
    static uint32_t l2_press_ms = 0;

    auto wing = ADIDigitalOut(WING_PORT);
    auto matchloader = ADIDigitalOut(MATCH_LOADER_PORT);
    auto midgoal = ADIDigitalOut(MIDGOAL_PORT);
    auto intakelift = ADIDigitalOut(LIFT_INTAKE);
    auto hood = ADIDigitalOut(HOOD_PORT);
    auto hoodBottom = ADIDigitalOut(HOOD_BOTTOM_PORT);
    auto odomLift = ADIDigitalOut(ODOM_LIFT);

    std::unordered_map<controller_digital_e_t, std::function<void()>> toggle_controls;
    std::unordered_map<controller_digital_e_t, std::pair<std::function<void(bool)>, std::function<void()>>> hold_controls;
    std::unordered_set<controller_digital_e_t> held;

    chassis.setBrakeMode(E_MOTOR_BRAKE_COAST);

    intake.set_anti_jam(false);

    midgoal.set_value(true);
    hood.set_value(true);
    wing.set_value(true);
    hoodBottom.set_value(false);
    odomLift.set_value(true);

    hold_controls.emplace(E_CONTROLLER_DIGITAL_B, std::make_pair(
        [&](bool firstPress) {
            matchloader.set_value(true);
        },
        [&]() {
            matchloader.set_value(false);
        }
    ));

    hold_controls.emplace(E_CONTROLLER_DIGITAL_RIGHT, std::make_pair(
        [&](bool firstPress) {
            chassis.tank(-80, -80, true);
        },
        [&]() {
            chassis.tank(0, 0, true);
        }
    ));

    int skills_down_held = 0;
    int skills_low_held = 0;

    hold_controls.emplace(E_CONTROLLER_DIGITAL_DOWN, std::make_pair(
        [&](bool firstPress) {
            if (IS_DRIVER_SKILLS) {
                intakelift.set_value(true);
                if (firstPress) {
                    Task e([&]() {
                        intake.bottom_intake.move_velocity(-200);
                        delay(500);
                        if (!master.get_digital(E_CONTROLLER_DIGITAL_DOWN)) return;
                        intake.top_backwards();
                        intake.bottom_intake.move_velocity(-200);
                        delay(1650);
                        if (!master.get_digital(E_CONTROLLER_DIGITAL_DOWN)) return;
                        intake.bottom_intake.move_velocity(-120);
                    });
                }
            } else {
                intake.bottom_backwards();
                intake.top_backwards();
            }
        },
        [&]() {
            intake.stop();
            intakelift.set_value(false);
        }
    ));

    hold_controls.emplace(E_CONTROLLER_DIGITAL_R1, std::make_pair(
        [&](bool firstPress) {
            if (master.get_digital(E_CONTROLLER_DIGITAL_L1)) return;
            intake.top_forwards();
            intake.bottom_forwards();
            midgoal.set_value(true);
            hood.set_value(true);
        },
        [&]() {
            intake.stop();   
            hood.set_value(true);
        }
    ));

    hold_controls.emplace(E_CONTROLLER_DIGITAL_R2, std::make_pair(
        [&](bool firstPress) {
            wing.set_value(false);
        },
        [&]() {
            wing.set_value(true);
        }
    ));

    hold_controls.emplace(E_CONTROLLER_DIGITAL_L1, std::make_pair(
        [&](bool firstPress) {
            intake.top_forwards();
            intake.bottom_forwards();
            midgoal.set_value(true);
            hood.set_value(false);
        },
        [&]() {
            intake.stop();
        }
    ));

    bool no_move = false;
    hold_controls.emplace(E_CONTROLLER_DIGITAL_L2, std::make_pair(
        [&](bool firstPress) {
            midgoal.set_value(false);
            hoodBottom.set_value(true);
            delay(200);
            if (IS_DRIVER_SKILLS) {
                if (firstPress) {
                    Task e([&]() {
                        intake.top_intake.move_velocity(200);
                        chassis.tank(-25, -25, true);
                        no_move = true;
                        delay(1750);
                        no_move = false;
                        if (!master.get_digital(E_CONTROLLER_DIGITAL_DOWN)) return;
                        intake.top_intake.move_velocity(150);
                    });
                }
            } else intake.top_forwards();
            intake.bottom_forwards();
            hood.set_value(true);

            std::cout << "Skills down held: " << skills_down_held << std::endl;
        },
        [&]() {
            intake.stop();
            midgoal.set_value(true);
            hood.set_value(true);
            hoodBottom.set_value(false);
            chassis.tank(0, 0, true);
            no_move = false;

            skills_down_held = 0;
        }
    ));

    hold_controls.emplace(E_CONTROLLER_DIGITAL_X, std::make_pair(
        [&](bool firstPress) {
            chassis.tank(60, 60, true);
        },
        [&]() {
            chassis.tank(0, 0, true);
        }
    ));

    Distance right_dist(RIGHT_DISTANCE_SENSOR);
    Distance front_dist(FRONT_DISTANCE_SENSOR);

    bool limit_drive = false;

    hold_controls.emplace(E_CONTROLLER_DIGITAL_Y, std::make_pair(
        [&](bool firstPress) {
            limit_drive = true;
        },
        [&]() {
            limit_drive = false;
        }
    ));

    toggle_controls.emplace(E_CONTROLLER_DIGITAL_LEFT, [&]() {
        double battery = battery_get_capacity();

        auto drivetrainMotors = {L_DRIVE_FRONT, L_DRIVE_MID, L_DRIVE_BACK, R_DRIVE_FRONT, R_DRIVE_MID, R_DRIVE_BACK};

        double temperatureSum = 0.0;
        double hotspot = 0.0;
        int hotspotPort = 0;

        for (int port : drivetrainMotors) {
            double currentTemp = motor_get_temperature(port);

            temperatureSum += currentTemp;
            
            if (hotspot < currentTemp) {
                hotspot = currentTemp;
                hotspotPort = port;
            }
        }

        double averageTemperature = temperatureSum / drivetrainMotors.size();

        temperatureSum = 0.0;

        master.print(0, 0, "T%.0f B%.0f D: %.0f", top_intake.get_temperature(), bottom_intake.get_temperature(), averageTemperature);
    });

    while (true) {
        float rightY = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
        float leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);

        if (abs(rightY) > 3 || abs(leftY) > 3) no_move = false;
        if (limit_drive) {
            rightY = std::clamp(rightY, -40.0f, 40.0f);
            leftY = std::clamp(leftY, -40.0f, 40.0f);
            chassis.tank(leftY, rightY, true);
        } else if (!no_move) {
            chassis.tank(leftY, rightY, false);
        }
                                    
        if (master.get_digital(E_CONTROLLER_DIGITAL_L2)) {
            skills_down_held += 1;
        } else {
            skills_down_held = 0;
        }

        if (master.get_digital(E_CONTROLLER_DIGITAL_DOWN)) {
            skills_low_held += 1;
        } else {
            skills_low_held = 0;
        }

        for (auto control : toggle_controls) {
            if (master.get_digital_new_press(control.first) && !held.contains(control.first)) {
                control.second();
            }
        }

        for (auto control : hold_controls) {
            if (master.get_digital(control.first)) {
                control.second.first(!held.contains(control.first));
                held.insert(control.first);
            } else if (held.contains(control.first)) {
                control.second.second();
                held.erase(control.first);
            }
        }

        intake.update();
    }
}