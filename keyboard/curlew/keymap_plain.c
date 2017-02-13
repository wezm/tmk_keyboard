/*
Copyright 2012,2013 Jun Wako <wakojun@gmail.com>

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

#include "keycode.h"
#include "action.h"
#include "action_macro.h"
#include "action_code.h"
#include "report.h"
#include "host.h"
#include "print.h"
#include "debug.h"
#include "keymap.h"

static const uint8_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    {{KC_CAPS, KC_FN1,  KC_SPC}},
    {{KC_TRNS, KC_TRNS, KC_BSPC}},
    // test with KC_CAPS, KC_A, KC_BTLD
};

#ifdef KEYMAP_SECTION_ENABLE
const action_t fn_actions[] __attribute__ ((section (".keymap.fn_actions"))) = {
#else
const action_t fn_actions[] PROGMEM = {
#endif
    [0] = ACTION_DEFAULT_LAYER_SET(0),                // Default layer(not used)
    [1] = ACTION_LAYER_MOMENTARY(1),                  // Mousekey layer(IJKL)
   //  [2] = ACTION_LAYER_TAP_KEY(2, KC_SLASH),          // Cursor layer with Slash*
   //  [3] = ACTION_LAYER_TAP_KEY(3, KC_SCLN),           // Mousekey layer with Semicolon*
   //  [4] = ACTION_LAYER_TAP_KEY(4, KC_SPC),            // Mousekey layer with Space
   //  [5] = ACTION_LAYER_MOMENTARY(4),                  // Mousekey layer(IJKL)
   //  [6] = ACTION_MODS_TAP_KEY(MOD_RCTL, KC_ENT),      // RControl with tap Enter
   //  [7] = ACTION_MODS_ONESHOT(MOD_LSFT),              // Oneshot Shift
   //  [8] = ACTION_MACRO(ALT_TAB),                      // Application switching
   //  [9] = ACTION_MODS_KEY(MOD_LALT, KC_LEFT),
   // [10] = ACTION_MODS_KEY(MOD_LALT, KC_RIGHT),

//  [x] = ACTION_LMOD_TAP_KEY(KC_LCTL, KC_BSPC),        // LControl with tap Backspace
//  [x] = ACTION_LMOD_TAP_KEY(KC_LCTL, KC_ESC),         // LControl with tap Esc
//  [x] = ACTION_FUNCTION_TAP(LSHIFT_LPAREN),           // Function: LShift with tap '('
//  [x] = ACTION_MACRO(HELLO),                          // Macro: say hello
//  [x] = ACTION_MACRO(VOLUP),                          // Macro: media key
};

/* translates key to keycode */
uint8_t keymap_key_to_keycode(uint8_t layer, keypos_t key)
{
    return keymaps[(layer)][(key.row)][(key.col)];
}

/* translates Fn keycode to action */
action_t keymap_fn_to_action(uint8_t keycode)
{
    return (action_t)fn_actions[FN_INDEX(keycode)];
}
