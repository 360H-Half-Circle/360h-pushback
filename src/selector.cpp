#include "main.h"
#include "selector.hpp"
#include "pros/misc.hpp"
#include "liblvgl/lvgl.h"
#include "intake.hpp"

using namespace pros;

Controller cm(E_CONTROLLER_MASTER);

LV_IMG_DECLARE(field);

/*

Autos: (Red and blue are the same except for the color of the balls on the matchloader)

Right Side SAWP

Left Side 7 Wing
Right Side 7 Wing
Left Side 4 Wing
Right Side 4 Wing
Left Side 9 Split
Right Side 9 Split



*/
const char* autonNames[] = {
    "RSAWP",     // 0
    "BSAWP",     // 1
    "R7WL",    // 2
    "B7WL",    // 3
    "R7WR",    // 4
    "B7WR",    // 5
    "R4WL",       // 6
    "B4WL",       // 7
    "R4WR",    // 8
    "B4WR",    // 9
    "R9SL",    // 10
    "B9SL",    // 11
    "R9SR",       // 12
    "B9SR",       // 13
};

// CORNER BUTTONS
lv_obj_t* redLeftBtn;
lv_obj_t* redRightBtn;
lv_obj_t* blueLeftBtn;
lv_obj_t* blueRightBtn;

// RED LEFT
lv_obj_t* R7WLBtn;
lv_obj_t* R4WLBtn;
lv_obj_t* R9SLBtn;

// BLUE LEFT
lv_obj_t* B7WLBtn;
lv_obj_t* B4WLBtn;
lv_obj_t* B9SLBtn;

// RED RIGHT
lv_obj_t* R7WRBtn;
lv_obj_t* R4WRBtn;
lv_obj_t* R9SRBtn;

// BLUE RIGHT
lv_obj_t* B7WRBtn;
lv_obj_t* B4WRBtn;
lv_obj_t* B9SRBtn;

// Solo Win Point
lv_obj_t* redRight_sawpBtn;
lv_obj_t* blueRight_sawpBtn;

lv_obj_t* titleLabel;

std::vector<lv_obj_t*> redRightButtons;
std::vector<lv_obj_t*> redLeftButtons;
std::vector<lv_obj_t*> blueLeftButtons;
std::vector<lv_obj_t*> blueRightButtons;
std::vector<lv_obj_t*> sawpButtons;

#define RED_LEFT 0
#define RED_RIGHT 1
#define BLUE_LEFT 2
#define BLUE_RIGHT 3

namespace sec {
    std::string auton = "none";

    static void set_auto_text(lv_obj_t* button, std::string text) {
        lv_obj_t* label = lv_obj_get_child(button, 0);
        if (label == nullptr) {
            label = lv_label_create(button);
        }

        lv_label_set_text(label, text.c_str());
    }

    static void showButtons(std::vector<lv_obj_t*> buttons) {
        for (auto btn : buttons) {
            lv_obj_clear_flag(btn, LV_OBJ_FLAG_HIDDEN);
        }
    }
    static void hideButtons(std::vector<lv_obj_t*> buttons) {
        for (auto btn : buttons) {
            lv_obj_add_flag(btn, LV_OBJ_FLAG_HIDDEN);
        }
    }
    static void set_state(std::vector<lv_obj_t*> buttons, lv_state_t state) {
        for (auto btn : buttons) {
            lv_obj_add_state(btn, state);
        }
    }
    static void remove_state(std::vector<lv_obj_t*> buttons, lv_state_t state) {
        for (auto btn : buttons) {
            lv_obj_clear_state(btn, state);
        }
    }
    static void showAndHideOtherButtons(int show) {
        switch (show)
        {
        case RED_LEFT:
            showButtons(redLeftButtons);
            hideButtons(redRightButtons);
            hideButtons(blueLeftButtons);
            hideButtons(blueRightButtons);
            break;
        case RED_RIGHT:
            hideButtons(redLeftButtons);
            showButtons(redRightButtons);
            hideButtons(blueLeftButtons);
            hideButtons(blueRightButtons);
            break;
        case BLUE_LEFT:
            hideButtons(redLeftButtons);
            hideButtons(redRightButtons);
            showButtons(blueLeftButtons);
            hideButtons(blueRightButtons);
            break;
        case BLUE_RIGHT:
            hideButtons(redLeftButtons);
            hideButtons(redRightButtons);
            hideButtons(blueLeftButtons);
            showButtons(blueRightButtons);
            break;
        }
    }

    static bool equals(lv_obj_t *btn1, lv_obj_t *btn2) {
        return btn1->coords.x1 == btn2->coords.x1 && btn1->coords.y1 == btn2->coords.y1;
    }

    static void secondButtonsEventHandler(lv_event_t * e) {
        lv_obj_t *btn = lv_event_get_target(e);
        lv_obj_t* label = lv_obj_get_child(btn, 0);

        if (label == nullptr) {
            lv_label_set_text_fmt(titleLabel, "Selected Auton: none");   
            return;
        }

        auton = std::string(lv_label_get_text(label));
        
        lv_label_set_text_fmt(titleLabel, "Selected Auton: %s", lv_label_get_text(label));
    }

    static void makeButtons(int x, std::vector<lv_obj_t*> &buttons, int type) {
        int i = 0;
        int realIndex = 0;
        int j = 0;
        for (auto button : buttons) {
            lv_obj_set_size(button, 70, 40);
            lv_obj_set_style_pad_left(button, 2, LV_PART_MAIN);  // Or 0 for no padding
            lv_obj_set_style_pad_right(button, 2, LV_PART_MAIN);  // Or 0 for no padding

            // lv_obj_t *label = lv_label_create(button);
            // lv_label_set_text(label, "no auto");
            auto align = LV_ALIGN_LEFT_MID;
            if (type == BLUE_RIGHT || type == BLUE_LEFT) {
                align = LV_ALIGN_RIGHT_MID;
            }
            lv_obj_align(button, align, x + 72 * i, -40 + 42 * j);
            i++;
            if (i % 2 == 0) {
                i = 0;
                j++;
            }

            realIndex++;

            if (type == RED_RIGHT || type == RED_LEFT) {
                lv_obj_set_style_bg_color(button, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);
            } else {
                lv_obj_set_style_bg_color(button, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);
            }

            lv_obj_add_event_cb(button, secondButtonsEventHandler, LV_EVENT_CLICKED, NULL);
        }
    }

    static void mainButtonsEventHandler(lv_event_t * e){
        lv_obj_t *btn = lv_event_get_target(e);
        
        if (equals(btn, redLeftBtn)) {
            std::cout << "Red Neg" << std::endl;
            showAndHideOtherButtons(RED_LEFT);

            lv_obj_add_state(redLeftBtn, LV_STATE_CHECKED);
            lv_obj_clear_state(redRightBtn, LV_STATE_CHECKED);
            lv_obj_clear_state(blueRightBtn, LV_STATE_CHECKED);
            lv_obj_clear_state(blueLeftBtn, LV_STATE_CHECKED);

            makeButtons(8, redLeftButtons, RED_LEFT);
        } else if (btn == redRightBtn) {
            std::cout << "Red Pos" << std::endl;
            showAndHideOtherButtons(RED_RIGHT);

            lv_obj_add_state(redRightBtn, LV_STATE_CHECKED);
            lv_obj_clear_state(redLeftBtn, LV_STATE_CHECKED);
            lv_obj_clear_state(blueRightBtn, LV_STATE_CHECKED);
            lv_obj_clear_state(blueLeftBtn, LV_STATE_CHECKED);

            makeButtons(8, redRightButtons, RED_RIGHT);
        } else if (btn == blueRightBtn) {
            std::cout << "Blue Right" << std::endl;
            showAndHideOtherButtons(BLUE_RIGHT);

            lv_obj_add_state(blueRightBtn, LV_STATE_CHECKED);
            lv_obj_clear_state(redLeftBtn, LV_STATE_CHECKED);
            lv_obj_clear_state(redRightBtn, LV_STATE_CHECKED);
            lv_obj_clear_state(blueLeftBtn, LV_STATE_CHECKED);

            makeButtons(-80, blueRightButtons, BLUE_RIGHT);
        } else if (btn == blueLeftBtn) {
            std::cout << "Blue Left" << std::endl;
            showAndHideOtherButtons(BLUE_LEFT);

            lv_obj_add_state(blueLeftBtn, LV_STATE_CHECKED);
            lv_obj_clear_state(redLeftBtn, LV_STATE_CHECKED);
            lv_obj_clear_state(redRightBtn, LV_STATE_CHECKED);
            lv_obj_clear_state(blueRightBtn, LV_STATE_CHECKED);

            makeButtons(-80, blueLeftButtons, BLUE_LEFT);
        } else {
            std::cout << "Unknown Button Pressed" << std::endl;
        }
    }

    void init(int hue, int default_auton, const char **autons){
        auton = 9;

        lv_scr_act();

        lv_obj_t* background = lv_obj_create(lv_scr_act());

        // 2. Set it to full screen
        lv_obj_set_size(background, LV_PCT(100), LV_PCT(100));

        lv_obj_t* center_img = lv_img_create(lv_scr_act());
        lv_img_set_src(center_img, &field);
        lv_obj_align(center_img, LV_ALIGN_TOP_MID, 0, 25);

        titleLabel = lv_label_create(lv_scr_act());

        redLeftBtn = lv_btn_create(lv_scr_act());
        //--
        //fix this my god
        R4WLBtn = lv_btn_create(lv_scr_act());
        R7WLBtn = lv_btn_create(lv_scr_act());
        R9SLBtn = lv_btn_create(lv_scr_act());
        redLeftButtons.push_back(R4WLBtn);
        redLeftButtons.push_back(R7WLBtn);
        redLeftButtons.push_back(R9SLBtn);

        redRightBtn = lv_btn_create(lv_scr_act());
        //--
        R4WRBtn = lv_btn_create(lv_scr_act());
        R7WRBtn = lv_btn_create(lv_scr_act());
        R9SRBtn = lv_btn_create(lv_scr_act());
        redRightButtons.push_back(R4WRBtn);
        redRightButtons.push_back(R7WRBtn);
        redRightButtons.push_back(R9SRBtn);

        blueLeftBtn = lv_btn_create(lv_scr_act());
        //--
        B4WLBtn = lv_btn_create(lv_scr_act());
        B7WLBtn = lv_btn_create(lv_scr_act());
        B9SLBtn = lv_btn_create(lv_scr_act());
        blueLeftButtons.push_back(B4WLBtn);
        blueLeftButtons.push_back(B7WLBtn);
        blueLeftButtons.push_back(B9SLBtn);

        blueRightBtn = lv_btn_create(lv_scr_act());
        //--
        B4WRBtn = lv_btn_create(lv_scr_act());
        B7WRBtn = lv_btn_create(lv_scr_act());
        B9SRBtn = lv_btn_create(lv_scr_act());
        blueRightButtons.push_back(B4WRBtn);
        blueRightButtons.push_back(B7WRBtn);
        blueRightButtons.push_back(B9SRBtn);

        // Solo Win Point
        redRight_sawpBtn = lv_btn_create(lv_scr_act());
        blueRight_sawpBtn = lv_btn_create(lv_scr_act());
        sawpButtons.push_back(redRight_sawpBtn);
        sawpButtons.push_back(blueRight_sawpBtn);
        
        hideButtons(redLeftButtons);
        hideButtons(redRightButtons);
        hideButtons(blueLeftButtons);
        hideButtons(blueRightButtons);
        hideButtons(sawpButtons);
    
        lv_label_set_text(titleLabel, "Select Auton:");
        lv_obj_align(titleLabel, LV_ALIGN_TOP_MID, 0, 5);
    
        lv_obj_set_width(redLeftBtn, 100);
        lv_obj_set_height(redLeftBtn, LV_SIZE_CONTENT);
        lv_obj_t *redLeftBtnLabel = lv_label_create(redLeftBtn);
        lv_label_set_text(redLeftBtnLabel, "Red Left");
        lv_obj_align(redLeftBtn, LV_ALIGN_TOP_LEFT, 8, 15);
        lv_obj_add_event_cb(redLeftBtn, mainButtonsEventHandler, LV_EVENT_CLICKED, NULL);
    
        lv_obj_set_width(redRightBtn, 100);
        lv_obj_set_height(redRightBtn, LV_SIZE_CONTENT);
        lv_obj_t *redRightBtnLabel = lv_label_create(redRightBtn);
        lv_label_set_text(redRightBtnLabel, "Red Right");
        lv_obj_align(redRightBtn, LV_ALIGN_BOTTOM_LEFT, 8, -15);
        lv_obj_add_event_cb(redRightBtn, mainButtonsEventHandler, LV_EVENT_CLICKED, NULL);
    
        lv_obj_set_width(blueLeftBtn, 100);
        lv_obj_set_height(blueLeftBtn, LV_SIZE_CONTENT);
        lv_obj_t *blueLeftBtnLabel = lv_label_create(blueLeftBtn);
        lv_label_set_text(blueLeftBtnLabel, "Blue Left");
        lv_obj_align(blueLeftBtn, LV_ALIGN_BOTTOM_RIGHT, -8, -15);
        lv_obj_add_event_cb(blueLeftBtn, mainButtonsEventHandler, LV_EVENT_CLICKED, NULL);

        lv_obj_set_width(blueRightBtn, 100);
        lv_obj_set_height(blueRightBtn, LV_SIZE_CONTENT);
        lv_obj_t *blueRightBtnLabel = lv_label_create(blueRightBtn);
        lv_label_set_text(blueRightBtnLabel, "Blue Right");
        lv_obj_align(blueRightBtn, LV_ALIGN_TOP_RIGHT, -8, 15);
        lv_obj_add_event_cb(blueRightBtn, mainButtonsEventHandler, LV_EVENT_CLICKED, NULL);

        int i = 0;
        for (auto btn : sawpButtons) {
            lv_obj_set_size(btn, 70, 40);
            lv_obj_set_style_pad_left(btn, 2, LV_PART_MAIN);  // Or 0 for no padding
            lv_obj_set_style_pad_right(btn, 2, LV_PART_MAIN);  // Or 0 for no padding

            auto name = "R SAWP";
            if (i == 1) {
                name = "B SAWP";
            }

            int mult = i == 0 ? 1 : -1;
            lv_obj_t *label = lv_label_create(btn);
            lv_label_set_text(label, name);
            lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, -30 + i * 72, 0);

            lv_obj_clear_flag(btn, LV_OBJ_FLAG_HIDDEN);

            if (i == 0) {
                lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);
            } else {
                lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);
            }

            lv_obj_add_event_cb(btn, secondButtonsEventHandler, LV_EVENT_CLICKED, NULL);

            i++;
        }

        redLeftBtn = redLeftBtn;
        redRightBtn = redRightBtn;
        blueLeftBtn = blueLeftBtn;
        blueRightBtn = blueRightBtn;

        // FINISHED AUTONS:
        set_auto_text(R7WLBtn, "R7WL");
        set_auto_text(R7WRBtn, "R7WR");
        set_auto_text(B7WLBtn, "B7WL");
        set_auto_text(B7WRBtn, "B7WR");
        
        set_auto_text(R4WLBtn, "R4WL");
        set_auto_text(R4WRBtn, "R4WR");
        set_auto_text(B4WLBtn, "B4WL");
        set_auto_text(B4WRBtn, "B4WR");
    }
}
