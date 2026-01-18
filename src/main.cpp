#include "main.h"
#include "controls.hpp"
#include "lemlib/api.hpp"
#include "selector.hpp"
#include <unordered_set>
#include "intake.hpp"
#include "autonomous.hpp"
#include "distanceReset.hpp"

#define TRACK_WIDTH 11.125

using namespace pros::c;

static Controller master(E_CONTROLLER_MASTER);

static MotorGroup left_motor_group({L_DRIVE_FRONT, -L_DRIVE_MID, -L_DRIVE_BACK}, MotorGears::blue, MotorUnits::rotations);
static MotorGroup right_motor_group({-R_DRIVE_FRONT, R_DRIVE_MID, R_DRIVE_BACK}, MotorGears::blue, MotorUnits::rotations);
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

static lemlib::Chassis& getChassis() {
    return chassis;
}

Intake& intake = Intake::getInstance();

void initialize() {
    lcd::initialize();
    // sec::init();

    intake.initialize();

    chassis.calibrate(true);

    Task lemlib_print_task{[] {
        // print pose
        while (true) {
            auto pose = chassis.getPose();
            lcd::print(0, "X: %.2f in", pose.x);
            pros::delay(1);
            lcd::print(1, "Y: %.2f in", pose.y);
            pros::delay(1);
            lcd::print(2, "Theta: %.2f deg", pose.theta);
            pros::delay(100);
        }
    }};
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
    auton::four_wing_left(chassis);

}

void opcontrol() {
    Motor bottom_intake = Motor(-BOTTOM_INTAKE);
    Motor top_intake = Motor(-TOP_INTAKE);
    static uint32_t l2_press_ms = 0;

    auto wing = ADIDigitalOut(WING_PORT);
    auto matchloader = ADIDigitalOut(MATCH_LOADER_PORT);
    auto midgoal = ADIDigitalOut(MIDGOAL_PORT);
    auto intakelift = ADIDigitalOut(LIFT_INTAKE);
    auto hood = ADIDigitalOut(HOOD_PORT);

    std::unordered_map<controller_digital_e_t, std::function<void()>> toggle_controls;
    std::unordered_map<controller_digital_e_t, std::pair<std::function<void(bool)>, std::function<void()>>> hold_controls;
    std::unordered_set<controller_digital_e_t> held;

    chassis.setBrakeMode(E_MOTOR_BRAKE_COAST);

    intake.set_anti_jam(false);

    midgoal.set_value(true);
    hood.set_value(false);
    wing.set_value(true);


    hold_controls.emplace(E_CONTROLLER_DIGITAL_B, std::make_pair(
        [&](bool firstPress) {
            matchloader.set_value(true);
        },
        [&]() {
            matchloader.set_value(false);
        }
    ));

    hold_controls.emplace(E_CONTROLLER_DIGITAL_DOWN, std::make_pair(
        [&](bool firstPress) {
            intake.bottom_backwards();
            intake.top_backwards();
            intakelift.set_value(true);
        },
        [&]() {
            intake.stop();
            intakelift.set_value(false);
        }
    ));

    hold_controls.emplace(E_CONTROLLER_DIGITAL_R1, std::make_pair(
        [&](bool firstPress) {
            // intake.top_forwards();
            intake.top_intake.move_velocity(400);
            intake.bottom_forwards();
            midgoal.set_value(true);
            hood.set_value(true);
        },
        [&]() {
            intake.stop();   
            hood.set_value(false);
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

    hold_controls.emplace(E_CONTROLLER_DIGITAL_L2, std::make_pair(
        [&](bool firstPress) {
            intake.top_forwards(80);
            intake.bottom_forwards();
            midgoal.set_value(false);
            hood.set_value(true);
        },
        [&]() {
            intake.stop();
            midgoal.set_value(true);
            hood.set_value(false);
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

        intake.update();

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

        master.print(0, 0, "Top: %.2f°C", top_intake.get_temperature());
        pros::delay(1);
        master.print(1, 0, "Bottom: %.2f°C", bottom_intake.get_temperature());
        pros::delay(1);
        master.print(2, 0, "Drive: %.2f°C", averageTemperature);
        pros::delay(1);
        // master.print(2,0,"Heading: %.2f", imu.get_heading());
        // pros::delay(1);
    }
}