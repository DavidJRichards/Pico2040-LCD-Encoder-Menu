// ============================================================
// Example:     LCDML: use spi display ST7789
// ============================================================
// Author:      Jomelo, Markus Rudel
// Last update: 12.11.2018
// License:     MIT
// ============================================================
// Description:
// This example shows how to use the TFT_eSPI library
// with the LCDMenuLib.
// The menu can placed in a box that can be placed anywhere on
// the screen.
// ============================================================
// *********************************************************************
// special settings
// *********************************************************************
// enable this line when you are not usigng a standard arduino
// for example when your chip is an ESP or a STM or SAM or something else

#define _LCDML_cfg_use_ram 

// include libs
#include <LCDMenuLib2.h>  

#include <SPI.h>

#include "TFT_eSPI.h"

#include <RotaryEncoder.h>

#define ENCODER_PIN_IN2   20 //16 //20
#define ENCODER_PIN_IN1   22 //17 //22 

// Setup a RotaryEncoder with 2 steps per latch for the 2 signal input pins:
RotaryEncoder encoder(ENCODER_PIN_IN1, ENCODER_PIN_IN2, RotaryEncoder::LatchMode::TWO03);
// *********************************************************************
// Adafruit TFT_ST7735
// *********************************************************************
// https://learn.adafruit.com/1-8-tft-display/graphics-library

  #define _LCDML_TEXT_COLOR       TFT_WHITE
  #define _LCDML_BACKGROUND_COLOR TFT_BLACK
  
  #define _LCDML_FONT_SIZE   2   
  #define _LCDML_FONT_W      (6*_LCDML_FONT_SIZE)             // font width 
  #define _LCDML_FONT_H      (8*_LCDML_FONT_SIZE)             // font heigt 
  
  // settings for u8g lib and lcd
  #define _LCDML_lcd_w       240            // lcd width
  #define _LCDML_lcd_h       240             // lcd height
 
  
 TFT_eSPI display = TFT_eSPI();


  // nothing change here
  #define _LCDML_cols_max    (_LCDML_lcd_w/_LCDML_FONT_W)  
  #define _LCDML_rows_max    (_LCDML_lcd_h/_LCDML_FONT_H) 

  // rows and cols 
  // when you use more rows or cols as allowed change in LCDMenuLib.h the define "_LCDML_DISP_cfg_max_rows" and "_LCDML_DISP_cfg_max_string_length"
  // the program needs more ram with this changes
  #define _LCDML_cols        20                   // max cols
  #define _LCDML_rows        _LCDML_rows_max  // max rows 


  // scrollbar width
  #define _LCDML_scrollbar_w 6  // scrollbar width  


  // old defines with new content
  #define _LCDML_DISP_cols      _LCDML_cols
  #define _LCDML_DISP_rows      _LCDML_rows 


// *********************************************************************
// Prototypes
// *********************************************************************
  void lcdml_menu_display();
  void lcdml_menu_clear();
  void lcdml_menu_control();


// *********************************************************************
// Objects
// *********************************************************************
  LCDMenuLib2_menu LCDML_0 (255, 0, 0, NULL, NULL); // root menu element (do not change)
  LCDMenuLib2 LCDML(LCDML_0, _LCDML_DISP_rows, _LCDML_DISP_cols, lcdml_menu_display, lcdml_menu_clear, lcdml_menu_control);


// *********************************************************************
// LCDML MENU/DISP
// *********************************************************************
  // LCDML_0        => layer 0
  // LCDML_0_X      => layer 1
  // LCDML_0_X_X    => layer 2
  // LCDML_0_X_X_X  => layer 3
  // LCDML_0_...      => layer ...

  // For beginners
  // LCDML_add(id, prev_layer, new_num, lang_char_array, callback_function)
  LCDML_add         (0  , LCDML_0         , 1  , "Information"      , mFunc_information);       // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (1  , LCDML_0         , 2  , "Time info"        , mFunc_timer_info);        // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (2  , LCDML_0         , 3  , "Program"          , NULL);                    // NULL = no menu function
  LCDML_add         (3  , LCDML_0_3       , 1  , "Program 1"        , NULL);                    // NULL = no menu function
  LCDML_add         (4  , LCDML_0_3_1     , 1  , "P1 dummy"         , NULL);                    // NULL = no menu function
  LCDML_add         (5  , LCDML_0_3_1     , 2  , "P1 Settings"      , NULL);                    // NULL = no menu function
  LCDML_add         (6  , LCDML_0_3_1_2   , 1  , "Warm"             , NULL);                    // NULL = no menu function
  LCDML_add         (7  , LCDML_0_3_1_2   , 2  , "Cold"             , NULL);                    // NULL = no menu function
  LCDML_add         (8  , LCDML_0_3_1_2   , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (9  , LCDML_0_3_1     , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (10 , LCDML_0_3       , 2  , "Program 2"        , mFunc_p2);                // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (11 , LCDML_0_3       , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (12 , LCDML_0         , 4  , "Special"          , NULL);                    // NULL = no menu function
  LCDML_add         (13 , LCDML_0_4       , 1  , "Go to Root"       , mFunc_goToRootMenu);      // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (14 , LCDML_0_4       , 2  , "Jump to Time info", mFunc_jumpTo_timer_info); // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (15 , LCDML_0_4       , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab


  // Advanced menu (for profit) part with more settings
  // Example for one function and different parameters
  // It is recommend to use parameters for switching settings like, (small drink, medium drink, big drink) or (200ml, 400ml, 600ml, 800ml) ...
  // the parameter change can also be released with dynParams on the next example
  // LCDMenuLib_addAdvanced(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
  LCDML_addAdvanced (16 , LCDML_0         , 5  , NULL,          "Parameter"      , NULL,                0,            _LCDML_TYPE_default);                    // NULL = no menu function
  LCDML_addAdvanced (17 , LCDML_0_5       , 1  , NULL,          "Parameter 1"      , mFunc_para,       10,            _LCDML_TYPE_default);                    // NULL = no menu function
  LCDML_addAdvanced (18 , LCDML_0_5       , 2  , NULL,          "Parameter 2"      , mFunc_para,       20,            _LCDML_TYPE_default);                    // NULL = no menu function
  LCDML_addAdvanced (19 , LCDML_0_5       , 3  , NULL,          "Parameter 3"      , mFunc_para,       30,            _LCDML_TYPE_default);                    // NULL = no menu function
  LCDML_add         (20 , LCDML_0_5       , 4  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab


  // Example for dynamic content
  // 1. set the string to ""
  // 2. use type  _LCDML_TYPE_dynParam   instead of    _LCDML_TYPE_default
  // this function type can not be used in combination with different parameters
  // LCDMenuLib_addAdvanced(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
  LCDML_addAdvanced (21 , LCDML_0         , 6  , NULL,          ""                  , mDyn_para,                0,   _LCDML_TYPE_dynParam);                     // NULL = no menu function

  // Example for conditions (for example for a screensaver)
  // 1. define a condition as a function of a boolean type -> return false = not displayed, return true = displayed
  // 2. set the function name as callback (remove the braces '()' it gives bad errors)
  // LCDMenuLib_addAdvanced(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
  LCDML_addAdvanced (22 , LCDML_0         , 7  , COND_hide,  "screensaver"        , mFunc_screensaver,        0,   _LCDML_TYPE_default);       // this menu function can be found on "LCDML_display_menuFunction" tab

  // Example function for event handling (only serial output in this example)  
  LCDML_add         (23 , LCDML_0         , 8  , "Event Handling"                 , mFunc_exampleEventHandling);  // this menu function can be found on "LCDML_display_menuFunction" tab

  // ***TIP*** Try to update _LCDML_DISP_cnt when you add a menu element.

  // menu element count - last element id
  // this value must be the same as the last menu element
  #define _LCDML_DISP_cnt    23

  // create menu
  LCDML_createMenu(_LCDML_DISP_cnt);




// *********************************************************************
// SETUP
// *********************************************************************
  void setup()
  {
    // serial init; only be needed if serial control is used
    Serial.begin(115200);
    while (!Serial && millis() < 5000);
    delay(100);
    Serial.println(F(_LCDML_VERSION)); // only for examples

    /* INIT DISPLAY */
    SPI.begin();
    display.begin();
    display.setRotation(0);
//    display.setSPISpeed(40000000);

    // clear lcd
    display.fillScreen(_LCDML_BACKGROUND_COLOR);
    
    // set text color / Textfarbe setzen
    display.setTextColor(_LCDML_TEXT_COLOR);  
    // set text size / Textgroesse setzen
    display.setTextSize(_LCDML_FONT_SIZE);
    display.setCursor(0, _LCDML_FONT_H * (3));
    display.println(_LCDML_VERSION);

    /* INIT LCDML */
    // LCDMenuLib Setup
    LCDML_setup(_LCDML_DISP_cnt);

    // Enable Menu Rollover
    LCDML.MENU_enRollover();

    // Enable Screensaver (screensaver menu function, time to activate in ms)
    LCDML.SCREEN_enable(mFunc_screensaver, 60000); // set to 60 seconds
    //LCDML.SCREEN_disable();

    // Some needful methods

    // You can jump to a menu function from anywhere with
    //LCDML.OTHER_jumpToFunc(mFunc_p2); // the parameter is the function name
  }

// *********************************************************************
// LOOP
// *********************************************************************
  void loop()
  {
    encoder.tick();
    // this function must called here, do not delete it
    LCDML.loop();
  }


// *********************************************************************
// check some errors - do not change here anything
// *********************************************************************
# if(_LCDML_glcd_tft_box_x1 > _LCDML_glcd_tft_w)
# error _LCDML_glcd_tft_box_x1 is to big
# endif

# if(_LCDML_glcd_tft_box_y1 > _LCDML_glcd_tft_h)
# error _LCDML_glcd_tft_box_y1 is to big
# endif