/*
Copyright 2016 Fred Sundvik <fsundvik@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LCD_ENABLE
#error This visualizer needs that LCD is enabled
#endif

#include "visualizer.h"
#include "curlew_logo.h"

static const char* welcome_text[] = {"Curlew", "Powered by TMK"};

// Just an example how to write custom keyframe functions, we could have moved
// all this into the init function
bool display_welcome(keyframe_animation_t* animation, visualizer_state_t* state) {
    (void)animation;
    gdispImage logo;

    // Read the uGFX documentation for information how to use the displays
    // http://wiki.ugfx.org/index.php/Main_Page
    gdispClear(Black);

    if (gdispImageOpenMemory(&logo, curlew_logo) == GDISP_IMAGE_ERR_OK) {
        gdispImageDraw(&logo, 0, 0, logo.width, logo.height, 0, 0);

        gdispImageClose(&logo);
    }

    // You can use static variables for things that can't be found in the animation
    // or state structs
    gdispDrawString(25, 3, welcome_text[0], state->font_normal, White);
    gdispDrawString(25, 15, welcome_text[1], state->font_normal, White);
    // Always remember to flush the display
    gdispFlush();
    // you could set the backlight color as well, but we won't do it here, since
    // it's part of the following animation
    // lcd_backlight_color(hue, saturation, intensity);
    // We don't need constant updates, just drawing the screen once is enough
    return false;
}

// Feel free to modify the animations below, or even add new ones if needed

// Don't worry, if the startup animation is long, you can use the keyboard like normal
// during that time
static keyframe_animation_t startup_animation = {
    .num_frames = 3,
    .loop = false,
    .frame_lengths = {0, MS2ST(1000), MS2ST(5000), 0},
    .frame_functions = {
            display_welcome,
            //keyframe_animate_backlight_color,
            keyframe_no_operation,
            enable_visualization
    },
};

// The color animation animates the LCD color when you change layers
static keyframe_animation_t color_animation = {
    .num_frames = 1,
    .loop = false,
    // Note that there's a 200 ms no-operation frame,
    // this prevents the color from changing when activating the layer
    // momentarily
    .frame_lengths = {MS2ST(200), MS2ST(500)},
    .frame_functions = {
      keyframe_no_operation,
      // keyframe_animate_backlight_color
    },
};

// The LCD animation alternates between the layer name display and a
// bitmap that displays all active layers
static keyframe_animation_t lcd_animation = {
    .num_frames = 2,
    .loop = true,
    .frame_lengths = {MS2ST(2000), MS2ST(2000)},
    .frame_functions = {keyframe_display_layer_text, keyframe_display_layer_bitmap},
};

static keyframe_animation_t suspend_animation = {
    .num_frames = 2,
    .loop = false,
    .frame_lengths = {0, MS2ST(1000), 0},
    .frame_functions = {
            keyframe_display_layer_text,
            // keyframe_animate_backlight_color,
            keyframe_disable_lcd_and_backlight,
    },
};

static keyframe_animation_t resume_animation = {
    .num_frames = 3,
    .loop = false,
    .frame_lengths = {0, 0, MS2ST(1000), MS2ST(5000), 0},
    .frame_functions = {
            keyframe_enable_lcd_and_backlight,
            // display_welcome,
            // keyframe_animate_backlight_color,
            keyframe_no_operation,
            enable_visualization,
    },
};
void initialize_user_visualizer(visualizer_state_t* state) {
    // The brightness will be dynamically adjustable in the future
    // But for now, change it here.
    // lcd_backlight_brightness(0x50);
    // state->current_lcd_color = LCD_COLOR(0x00, 0x00, 0xFF);
    // state->target_lcd_color = LCD_COLOR(0x10, 0xFF, 0xFF);
    start_keyframe_animation(&startup_animation);
}

void update_user_visualizer_state(visualizer_state_t* state) {
    // Add more tests, change the colors and layer texts here
    // Usually you want to check the high bits (higher layers first)
    // because that's the order layers are processed for keypresses
    // You can for check for example:
    // state->status.layer
    // state->status.default_layer
    // state->status.leds (see led.h for available statuses)
    if (state->status.layer & 0x8) {
        // state->target_lcd_color = LCD_COLOR(0xC0, 0xB0, 0xFF);
        state->layer_text = "Numpad";
    }
    else if (state->status.layer & 0x4) {
        // state->target_lcd_color = LCD_COLOR(0, 0xB0, 0xFF);
        state->layer_text = "KBD functions";
    }
    else if (state->status.layer & 0x2) {
        // state->target_lcd_color = LCD_COLOR(0x80, 0xB0, 0xFF);
        state->layer_text = "Function keys";
    }
    else {
        // state->target_lcd_color = LCD_COLOR(0x40, 0xB0, 0xFF);
        state->layer_text = "Default";
    }
    // You can also stop existing animations, and start your custom ones here
    // remember that you should normally have only one animation for the LCD
    // and one for the background. But you can also combine them if you want.
    start_keyframe_animation(&lcd_animation);
    // start_keyframe_animation(&color_animation);
}

void user_visualizer_suspend(visualizer_state_t* state) {
    state->layer_text = "Suspending...";
    // uint8_t hue = LCD_HUE(state->current_lcd_color);
    // uint8_t sat = LCD_SAT(state->current_lcd_color);
    // state->target_lcd_color = LCD_COLOR(hue, sat, 0);
    start_keyframe_animation(&suspend_animation);
}

void user_visualizer_resume(visualizer_state_t* state) {
    // state->current_lcd_color = LCD_COLOR(0x00, 0x00, 0x00);
    // state->target_lcd_color = LCD_COLOR(0x10, 0xFF, 0xFF);
    start_keyframe_animation(&resume_animation);
}
