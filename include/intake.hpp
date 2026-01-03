#pragma once

#include "subsystem.hpp"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "pros/optical.h"
#include "controls.hpp"

using namespace pros;
using namespace lib;

class Intake : public Subsystem {
public:
    Motor bottom_intake{BOTTOM_INTAKE};
    Motor top_intake{TOP_INTAKE};

    void initialize() override;
    void update() override;
    void stop() override;

    void set_anti_jam(bool antijam);
    void bottom_forwards(int power = 127);
    void bottom_backwards(int power = 127);
    void top_forwards(int power = 127);
    void top_backwards(int power = 127);

    bool get_antijam() {
        return antijam;
    }

    void stop_bottom() {
        bottom_intake.move(0);
        moving = false;
    }
    void stop_top() {
        top_intake.move(0);
    }

    static Intake& getInstance() {
        static Intake instance; 
        return instance;
    }
private:
    bool antijam = false;
    int counter = 0;
    int volts = 0;
    bool moving = false;
};