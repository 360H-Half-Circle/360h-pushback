#pragma once

#include "lemlib/api.hpp"
#include "controls.hpp"
#include "pros/distance.hpp"

namespace auton {
    void seven_wing_right(lemlib::Chassis& chassis);
    void four_wing_right(lemlib::Chassis& chassis);

    void seven_wing_left(lemlib::Chassis& chassis);
    void four_wing_left(lemlib::Chassis& chassis);

    void nine_split_right(lemlib::Chassis& chassis);

    void seven_split_left(lemlib::Chassis& chassis);
    void seven_split_right(lemlib::Chassis& chassis);

    void sawp(lemlib::Chassis& chassis);
    void sawp_safe(lemlib::Chassis& chassis);

    void auton_skills(lemlib::Chassis& chassis);
}