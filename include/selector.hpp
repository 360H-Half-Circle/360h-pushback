#pragma once


#include <string>

//selector configuration
#define HUE 360
#define DEFAULT 1
#define AUTONS (const char*[]){"POS", "NEG", "nothing"}

namespace sec{

extern std::string auton;
extern int color;
// const char *bee[] = {AUTONS, ""};
void init(int hue = HUE, int default_auton = DEFAULT, const char **autons = AUTONS);
}