#include "main.h"
#include "controls.hpp"
#include "lemlib/api.hpp"
#include "selector.hpp"
#include <unordered_set>

#define TRACK_WIDTH 10.0

using namespace pros::c;

static Controller master(E_CONTROLLER_MASTER);

static MotorGroup left_motor_group({-L_DRIVE_FRONT, L_DRIVE_MID, -L_DRIVE_BACK}, MotorGears::blue, MotorUnits::rotations);
static MotorGroup right_motor_group({R_DRIVE_FRONT, -R_DRIVE_MID, R_DRIVE_BACK}, MotorGears::blue, MotorUnits::rotations);
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
    60, // derivative gain (kD)
    3, // anti windup
    1, // small error range, in inches
    100, // small error range timeout, in millisecond
    3, // large error range, in inches
    500, // large error range timeout, in milliseconds
    0 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(
    2, // proportional gain (kP)
    0, // integral gain (kI)
    10, // derivative gain (kD)
    3, // anti windup
    1, // small error range, in degrees
    100, // small error range timeout, in milliseconds
    3, // large error range, in degrees
    500, // large error range timeout, in milliseconds
    0 // maximum acceleration (slew)
);

lemlib::OdomSensors sensors(
    nullptr, // vertical tracking wheel 1, set to null
    nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
    nullptr, // horizontal tracking wheel 1
    nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
    &imu // inertial sensor
);

lemlib::Chassis chassis(drivetrain, // drivetrain settings
    lateral_controller, // lateral PID settings
    angular_controller, // angular PID settings
    sensors // odometry sensors
);

void initialize() {
    lcd::initialize();

    chassis.calibrate(true);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

// b is wing down is matchloader 
// tank drive
// top left back spin bvottom stage forward
//bottom left back outake bottom intake
// top right back is intake both
// bottom right back is top outake bottom intake and midgoal piston retract

// top right back

void opcontrol() {
	Motor bottom_intake = Motor(BOTTOM_INTAKE);
    Motor top_intake = Motor(TOP_INTAKE);

    auto wing = ADIDigitalOut(WING_PORT);

    std::unordered_map<controller_digital_e_t, std::function<void()>> toggle_controls;
    std::unordered_map<controller_digital_e_t, std::pair<std::function<void(bool)>, std::function<void()>>> hold_controls;
    std::unordered_set<controller_digital_e_t> held;

    hold_controls.emplace(E_CONTROLLER_DIGITAL_B, std::make_pair(
        [&](bool firstPress) {
            wing.set_value(true);
        },
        [&]() {
            wing.set_value(false);
        }
    ));

    hold_controls.emplace(E_CONTROLLER_DIGITAL_L1, std::make_pair(
        [&](bool firstPress) {
            bottom_intake.move(127);
        },
        [&]() {
            bottom_intake.move(0);
        }
    ));

    hold_controls.emplace(E_CONTROLLER_DIGITAL_L2, std::make_pair(
        [&](bool firstPress) {
            bottom_intake.move(-127);
        },
        [&]() {
            bottom_intake.move(0);
        }
    ));

    hold_controls.emplace(E_CONTROLLER_DIGITAL_R1, std::make_pair(
        [&](bool firstPress) {
            top_intake.move(127);
            bottom_intake.move(127);
        },
        [&]() {
            top_intake.move(0);
            bottom_intake.move(0);
        }
    ));

    hold_controls.emplace(E_CONTROLLER_DIGITAL_R2, std::make_pair(
        [&](bool firstPress) {
            top_intake.move(-127);
            bottom_intake.move(-127);
        },
        [&]() {
            top_intake.move(0);
            bottom_intake.move(0);
        }
    ));

    while (true) {
        float rightX = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        float leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);

        left_motor_group.move(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));
        right_motor_group.move(master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));
        
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

        master.print(0, 0, "Top Intake: %.2f°C", top_intake.get_temperature());
        pros::delay(1);
        master.print(1, 0, "Bottom Intake: %.2f°C", bottom_intake.get_temperature());
        pros::delay(1);
        master.print(2, 0, "Drive: %.2f°C", averageTemperature);
        pros::delay(1);
    }
}