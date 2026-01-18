// #include "main.h"
// #include "selector.hpp"
// #include "pros/misc.hpp"
// #include "liblvgl/lvgl.h"
// #include "intake.hpp"

// using namespace pros;

// Controller cm(E_CONTROLLER_MASTER);

// LV_IMG_DECLARE(field);
// /*

// Autos: (Red and blue are the same except for the color of the balls on the matchloader)

// Right Side SAWP

// Left Side 7 Wing
// Right Side 7 Wing
// Left Side 4 Wing
// Right Side 4 Wing
// Left Side 9 Split
// Right Side 9 Split



// */
// const char* autonNames[] = {
//     "RSAWP",     // 0
//     "BSAWP",     // 1
//     "R7WL",    // 2
//     "B7WL",    // 3
//     "R7WR",    // 4
//     "B7WR",    // 5
//     "R4WL",       // 6
//     "B4WL",       // 7
//     "R4WR",    // 8
//     "B4WR",    // 9
//     "R9SL",    // 10
//     "B9SL",    // 11
//     "R9SR",       // 12
//     "B9SR",       // 13
// };

// // RED LEFT
// lv_obj_t* R7WLBtn;
// lv_obj_t* R4WLBtn;
// lv_obj_t* R9SLBtn;

// // BLUE LEFT
// lv_obj_t* B7WLBtn;
// lv_obj_t* B4WLBtn;
// lv_obj_t* B9SLBtn;

// // RED RIGHT
// lv_obj_t* R7WRBtn;
// lv_obj_t* R4WRBtn;
// lv_obj_t* R9SRBtn;

// // BLUE RIGHT
// lv_obj_t* B7WRBtn;
// lv_obj_t* B4WRBtn;
// lv_obj_t* B9SRBtn;

// // Solo Win Point
// lv_obj_t* redRight_sawpBtn;
// lv_obj_t* blueRight_sawpBtn;

// lv_obj_t* titleLabel;

// std::vector<lv_obj_t*> redRightButtons;
// std::vector<lv_obj_t*> redLeftButtons;
// std::vector<lv_obj_t*> blueLeftButtons;
// std::vector<lv_obj_t*> blueRightButtons;
// std::vector<lv_obj_t*> sawpButtons;

// #define RED_LEFT 0
// #define RED_RIGHT 1
// #define BLUE_LEFT 2
// #define BLUE_RIGHT 3

// namespace sec {
//     int auton;

//     static void set_auto_text(lv_obj_t* button, std::string text) {
//         lv_obj_t* label = lv_obj_get_child(button, 0);
//         if (label != nullptr) {
//             lv_label_set_text(label, text.c_str());
//         } else {
//             std::cout << "Label not found" << std::endl;
//         }
//     }

//     static void showButtons(std::vector<lv_obj_t*> buttons) {
//         for (auto btn : buttons) {
//             lv_obj_clear_flag(btn, LV_OBJ_FLAG_HIDDEN);
//         }
//     }
//     static void hideButtons(std::vector<lv_obj_t*> buttons) {
//         for (auto btn : buttons) {
//             lv_obj_add_flag(btn, LV_OBJ_FLAG_HIDDEN);
//         }
//     }
//     static void set_state(std::vector<lv_obj_t*> buttons, lv_state_t state) {
//         for (auto btn : buttons) {
//             lv_obj_add_state(btn, state);
//         }
//     }
//     static void remove_state(std::vector<lv_obj_t*> buttons, lv_state_t state) {
//         for (auto btn : buttons) {
//             lv_obj_clear_state(btn, state);
//         }
//     }
//     static void showAndHideOtherButtons(int show) {
//         switch (show)
//         {
//         case RED_LEFT:
//             showButtons(redLeftButtons);
//             hideButtons(redRightButtons);
//             hideButtons(blueLeftButtons);
//             hideButtons(blueRightButtons);
//             break;
//         case RED_RIGHT:
//             hideButtons(redLeftButtons);
//             showButtons(redRightButtons);
//             hideButtons(blueLeftButtons);
//             hideButtons(blueRightButtons);
//             break;
//         case BLUE_LEFT:
//             hideButtons(redLeftButtons);
//             hideButtons(redRightButtons);
//             showButtons(blueLeftButtons);
//             hideButtons(blueRightButtons);
//             break;
//         case BLUE_RIGHT:
//             hideButtons(redLeftButtons);
//             hideButtons(redRightButtons);
//             hideButtons(blueLeftButtons);
//             showButtons(blueRightButtons);
//             break;
//         }
//     }

//     static bool equals(lv_obj_t *btn1, lv_obj_t *btn2) {
//         return btn1->coords.x1 == btn2->coords.x1 && btn1->coords.y1 == btn2->coords.y1;
//     }

//     static void secondButtonsEventHandler(lv_event_t * e) {
//         lv_obj_t *btn = lv_event_get_target(e);

//         if (equals(btn, redRight_sawpBtn)) {
//             auton = 0;
//         } else if (equals(btn, blueRight_sawpBtn)) {
//             auton = 1;
//         } else if (equals(btn, redRight_sawpBtn)) {
//             auton = 2;
//         } else if (equals(btn, R7WLBtn)) {
//             auton = 3;
//         } else if (equals(btn, B7WLBtn)) {
//             auton = 4;
//         } else if (equals(btn, R7WRBtn)) {
//             auton = 5;
//         } else if (equals(btn, B7WRBtn)) {
//             auton = 6;
//         } else if (equals(btn, R4WLBtn)) {
//             auton = 7;
//         } else if (equals(btn, B4WLBtn)) {
//             auton = 8;
//         } else if (equals(btn, R4WRBtn)) {
//             auton = 9;
//         } else if (equals(btn, B4WRBtn)) {
//             auton = 10;
//         } else if (equals(btn, R9SLBtn)) {
//             auton = 11;
//         } else if (equals(btn, B9SLBtn)) {
//             auton = 12;
//         } else if (equals(btn, R9SRBtn)) {
//             auton = 13;
//         } else if (equals(btn, B9SRBtn)) {
//             auton = 14;
//         }
        
//         lv_label_set_text_fmt(titleLabel, "Selected Auton: %s", autonNames[auton]);
//     }

//     static void makeButtons(int x, std::vector<lv_obj_t*> &buttons, int type) {
//         int i = 0;
//         int realIndex = 0;
//         int j = 0;
//         for (auto button : buttons) {
//             lv_obj_set_size(button, 70, 40);
//             lv_obj_set_style_pad_left(button, 2, LV_PART_MAIN);  // Or 0 for no padding
//             lv_obj_set_style_pad_right(button, 2, LV_PART_MAIN);  // Or 0 for no padding

//             lv_obj_t *label = lv_label_create(button);
//             lv_label_set_text(label, "no auto");
//             auto align = LV_ALIGN_LEFT_MID;
//             if (type == BLUE_RIGHT || type == BLUE_LEFT) {
//                 align = LV_ALIGN_RIGHT_MID;
//             }
//             lv_obj_align(button, align, x + 72 * i, -40 + 42 * j);
//             i++;
//             if (i % 2 == 0) {
//                 i = 0;
//                 j++;
//             }

//             realIndex++;

//             if (type == RED_RIGHT || type == RED_LEFT) {
//                 lv_obj_set_style_bg_color(button, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);
//             } else {
//                 lv_obj_set_style_bg_color(button, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);
//             }

//             lv_obj_add_event_cb(button, secondButtonsEventHandler, LV_EVENT_CLICKED, NULL);
//         }
//     }

//     static void mainButtonsEventHandler(lv_event_t * e){
//         lv_obj_t *btn = lv_event_get_target(e);
        
//         if (equals(btn, redLEFTBtn)) {
//             std::cout << "Red Neg" << std::endl;
//             showAndHideOtherButtons(RED_LEFT);

//             lv_obj_add_state(redLEFTBtn, LV_STATE_CHECKED);
//             lv_obj_clear_state(redRIGHTBtn, LV_STATE_CHECKED);
//             lv_obj_clear_state(blueRIGHTBtn, LV_STATE_CHECKED);
//             lv_obj_clear_state(blueLEFTBtn, LV_STATE_CHECKED);

//             makeButtons(8, redLEFTButtons, RED_LEFT);
//         } else if (btn == redRIGHTBtn) {
//             std::cout << "Red Pos" << std::endl;
//             showAndHideOtherButtons(RED_RIGHT);

//             lv_obj_add_state(redRIGHTBtn, LV_STATE_CHECKED);
//             lv_obj_clear_state(redLEFTBtn, LV_STATE_CHECKED);
//             lv_obj_clear_state(blueRIGHTBtn, LV_STATE_CHECKED);
//             lv_obj_clear_state(blueLEFTBtn, LV_STATE_CHECKED);

//             makeButtons(8, redRIGHTButtons, RED_RIGHT);
//         } else if (btn == blueRIGHTBtn) {
//             std::cout << "Blue Right" << std::endl;
//             showAndHideOtherButtons(BLUE_RIGHT);

//             lv_obj_add_state(blueRIGHTBtn, LV_STATE_CHECKED);
//             lv_obj_clear_state(redLEFTBtn, LV_STATE_CHECKED);
//             lv_obj_clear_state(redRIGHTBtn, LV_STATE_CHECKED);
//             lv_obj_clear_state(blueLEFTBtn, LV_STATE_CHECKED);

//             makeButtons(-80, blueRIGHTButtons, BLUE_RIGHT);
//         } else if (btn == blueLEFTBtn) {
//             std::cout << "Blue Left" << std::endl;
//             showAndHideOtherButtons(BLUE_LEFT);

//             lv_obj_add_state(blueLEFTBtn, LV_STATE_CHECKED);
//             lv_obj_clear_state(redLEFTBtn, LV_STATE_CHECKED);
//             lv_obj_clear_state(redRIGHTBtn, LV_STATE_CHECKED);
//             lv_obj_clear_state(blueRIGHTBtn, LV_STATE_CHECKED);

//             makeButtons(-80, blueLEFTButtons, BLUE_LEFT);
//         } else {
//             std::cout << "Unknown Button Pressed" << std::endl;
//         }
//     }

//     void init(int hue, int default_auton, const char **autons){
//         auton = 9;

//         lv_scr_act();

//         lv_obj_t* background = lv_obj_create(lv_scr_act());

//         // 2. Set it to full screen
//         lv_obj_set_size(background, LV_PCT(100), LV_PCT(100));

//         lv_obj_t* center_img = lv_img_create(lv_scr_act());
//         lv_img_set_src(center_img, &field);
//         lv_obj_align(center_img, LV_ALIGN_TOP_MID, 0, 25);

//         titleLabel = lv_label_create(lv_scr_act());

//         redLEFTBtn = lv_btn_create(lv_scr_act());
//         //--
//         //fix this my god
//         redLEFT_6p1CornerBtn = lv_btn_create(lv_scr_act());
//         redLEFT_6p1LadderBtn = lv_btn_create(lv_scr_act());
//         redLEFT_6p1CornerNoSweepBtn = lv_btn_create(lv_scr_act());
//         redLEFT_6CornerBtn = lv_btn_create(lv_scr_act());
//         redLEFT_6LadderBtn = lv_btn_create(lv_scr_act());
//         redLEFT_6CornerNoSweepBtn = lv_btn_create(lv_scr_act());
//         redLEFTButtons.push_back(redLEFT_6p1CornerBtn);
//         redLEFTButtons.push_back(redLEFT_6p1LadderBtn);
//         redLEFTButtons.push_back(redLEFT_6p1CornerNoSweepBtn);
//         redLEFTButtons.push_back(redLEFT_6CornerBtn);
//         redLEFTButtons.push_back(redLEFT_6LadderBtn);
//         redLEFTButtons.push_back(redLEFT_6CornerNoSweepBtn);

//         redRIGHTBtn = lv_btn_create(lv_scr_act());
//         //--
//         redPos_5p1w_BakerBtn = lv_btn_create(lv_scr_act());
//         redPos_5p1w_LadderBtn = lv_btn_create(lv_scr_act());
//         redPos_5p1a_BakerBtn = lv_btn_create(lv_scr_act());
//         redPos_5p1a_LadderBtn = lv_btn_create(lv_scr_act());
//         redPos_6_BakerBtn = lv_btn_create(lv_scr_act());
//         redPos_6_LadderBtn = lv_btn_create(lv_scr_act());
//         redPosButtons.push_back(redPos_5p1w_BakerBtn);
//         redPosButtons.push_back(redPos_5p1w_LadderBtn);
//         redPosButtons.push_back(redPos_5p1a_BakerBtn);
//         redPosButtons.push_back(redPos_5p1a_LadderBtn);
//         redPosButtons.push_back(redPos_6_BakerBtn);
//         redPosButtons.push_back(redPos_6_LadderBtn);

//         blueNegBtn = lv_btn_create(lv_scr_act());
//         //--
//         blueNeg_6p1CornerBtn = lv_btn_create(lv_scr_act());
//         blueNeg_6p1LadderBtn = lv_btn_create(lv_scr_act());
//         blueNeg_6p1CornerNoSweepBtn = lv_btn_create(lv_scr_act());
//         blueNeg_6CornerBtn = lv_btn_create(lv_scr_act());
//         blueNeg_6LadderBtn = lv_btn_create(lv_scr_act());
//         blueNeg_6CornerNoSweepBtn = lv_btn_create(lv_scr_act());
//         blueNegButtons.push_back(blueNeg_6p1CornerBtn);
//         blueNegButtons.push_back(blueNeg_6p1LadderBtn);
//         blueNegButtons.push_back(blueNeg_6p1CornerNoSweepBtn);
//         blueNegButtons.push_back(blueNeg_6CornerBtn);
//         blueNegButtons.push_back(blueNeg_6LadderBtn);
//         blueNegButtons.push_back(blueNeg_6CornerNoSweepBtn);

//         bluePosBtn = lv_btn_create(lv_scr_act());
//         //--
//         bluePos_5p1w_BakerBtn = lv_btn_create(lv_scr_act());
//         bluePos_5p1w_LadderBtn = lv_btn_create(lv_scr_act());
//         bluePos_5p1a_BakerBtn = lv_btn_create(lv_scr_act());
//         bluePos_5p1a_LadderBtn = lv_btn_create(lv_scr_act());
//         bluePos_6_BakerBtn = lv_btn_create(lv_scr_act());
//         bluePos_6_LadderBtn = lv_btn_create(lv_scr_act());
//         bluePosButtons.push_back(bluePos_5p1w_BakerBtn);
//         bluePosButtons.push_back(bluePos_5p1w_LadderBtn);
//         bluePosButtons.push_back(bluePos_5p1a_BakerBtn);
//         bluePosButtons.push_back(bluePos_5p1a_LadderBtn);
//         bluePosButtons.push_back(bluePos_6_BakerBtn);
//         bluePosButtons.push_back(bluePos_6_LadderBtn);

//         // Solo Win Point
//         redNeg_sawpBtn = lv_btn_create(lv_scr_act());
//         blueNeg_sawpBtn = lv_btn_create(lv_scr_act());
//         sawpButtons.push_back(redNeg_sawpBtn);
//         sawpButtons.push_back(blueNeg_sawpBtn);
        
//         hideButtons(redNegButtons);
//         hideButtons(redPosButtons);
//         hideButtons(blueNegButtons);
//         hideButtons(bluePosButtons);
//         hideButtons(sawpButtons);
    
//         lv_label_set_text(titleLabel, "Select Auton:");
//         lv_obj_align(titleLabel, LV_ALIGN_TOP_MID, 0, 5);
    
//         lv_obj_set_width(redNegBtn, 100);
//         lv_obj_set_height(redNegBtn, LV_SIZE_CONTENT);
//         lv_obj_t *redNegBtnLabel = lv_label_create(redNegBtn);
//         lv_label_set_text(redNegBtnLabel, "Red Left");
//         lv_obj_align(redNegBtn, LV_ALIGN_TOP_LEFT, 8, 15);
//         lv_obj_add_event_cb(redNegBtn, mainButtonsEventHandler, LV_EVENT_CLICKED, NULL);
    
//         lv_obj_set_width(redPosBtn, 100);
//         lv_obj_set_height(redPosBtn, LV_SIZE_CONTENT);
//         lv_obj_t *redPosBtnLabel = lv_label_create(redPosBtn);
//         lv_label_set_text(redPosBtnLabel, "Red Right");
//         lv_obj_align(redPosBtn, LV_ALIGN_BOTTOM_LEFT, 8, -15);
//         lv_obj_add_event_cb(redPosBtn, mainButtonsEventHandler, LV_EVENT_CLICKED, NULL);
    
//         lv_obj_set_width(blueNegBtn, 100);
//         lv_obj_set_height(blueNegBtn, LV_SIZE_CONTENT);
//         lv_obj_t *blueNegBtnLabel = lv_label_create(blueNegBtn);
//         lv_label_set_text(blueNegBtnLabel, "Blue Right");
//         lv_obj_align(blueNegBtn, LV_ALIGN_TOP_RIGHT, -8, 15);
//         lv_obj_add_event_cb(blueNegBtn, mainButtonsEventHandler, LV_EVENT_CLICKED, NULL);
    
//         lv_obj_set_width(bluePosBtn, 100);
//         lv_obj_set_height(bluePosBtn, LV_SIZE_CONTENT);
//         lv_obj_t *bluePosBtnLabel = lv_label_create(bluePosBtn);
//         lv_label_set_text(bluePosBtnLabel, "Blue Left");
//         lv_obj_align(bluePosBtn, LV_ALIGN_BOTTOM_RIGHT, -8, -15);
//         lv_obj_add_event_cb(bluePosBtn, mainButtonsEventHandler, LV_EVENT_CLICKED, NULL);

//         int i = 0;
//         for (auto btn : sawpButtons) {
//             lv_obj_set_size(btn, 70, 40);
//             lv_obj_set_style_pad_left(btn, 2, LV_PART_MAIN);  // Or 0 for no padding
//             lv_obj_set_style_pad_right(btn, 2, LV_PART_MAIN);  // Or 0 for no padding

//             auto name = "R SAWP";
//             if (i == 1) {
//                 name = "B SAWP";
//             }

//             int mult = i == 0 ? 1 : -1;
//             lv_obj_t *label = lv_label_create(btn);
//             lv_label_set_text(label, name);
//             lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, -30 + i * 72, 0);

//             lv_obj_clear_flag(btn, LV_OBJ_FLAG_HIDDEN);

//             if (i == 0) {
//                 lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);
//             } else {
//                 lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);
//             }

//             lv_obj_add_event_cb(btn, secondButtonsEventHandler, LV_EVENT_CLICKED, NULL);

//             i++;
//         }

//         redNegBtn = redNegBtn;
//         redPosBtn = redPosBtn;
//         blueNegBtn = blueNegBtn;
//         bluePosBtn = bluePosBtn;

//         // FINISHED AUTONS:
//         set_auto_text(redNeg_6p1CornerBtn, "R 6.1 C");
//         set_auto_text(redNeg_6p1LadderBtn, "R 6.1 L");
//         set_auto_text(redNeg_6p1CornerNoSweepBtn, "R 6.1 X");
//         set_auto_text(redPos_6_LadderBtn, "R 6 L");
//         set_auto_text(bluePos_6_LadderBtn, "B 6 L");
//     }
// }
