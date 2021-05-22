/* Copyright 2015-2017 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "muse.h"

enum preonic_layers {
  _QWERTY,
  _LOWER,
  _RAISE,
  _FN,
  _SPACE_FN,
  _ADJUST
};

enum preonic_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  BACKLIT,
  MACOS_FORCE_QUIT
};

// Fillers to make layering more clear
#define LOWER      MO(_LOWER)
#define RAISE      MO(_RAISE)
#define FN         MO(_FN)
#define SPACE_FN   LT(_SPACE_FN, KC_SPC)
#define LEFT_RAISE LT(_RAISE, KC_ESC)

enum {
  DBCBR,
  DBPRN,
  DBBRC,
  OBROBRK,
  CBRCBRK,
  SQUODQUO,
  SEMI_EQ,
  SLASH_MIN,
  COMMA_MIN,
  GRV_OR_TILD,
};
qk_tap_dance_action_t tap_dance_actions[] = {
  [DBCBR]     = ACTION_TAP_DANCE_DOUBLE(KC_LCBR, KC_RCBR),  // {}
  [DBPRN]     = ACTION_TAP_DANCE_DOUBLE(KC_LPRN, KC_RPRN),  // ()
  [DBBRC]     = ACTION_TAP_DANCE_DOUBLE(KC_LBRC, KC_RBRC),  // []
  [OBROBRK]   = ACTION_TAP_DANCE_DOUBLE(KC_LBRC, KC_LCBR),  // [{
  [CBRCBRK]   = ACTION_TAP_DANCE_DOUBLE(KC_RBRC, KC_RCBR),  // ]}
  [SQUODQUO]  = ACTION_TAP_DANCE_DOUBLE(KC_QUOT, KC_DQUO),  // ' "
  [SEMI_EQ]   = ACTION_TAP_DANCE_DOUBLE(KC_SCLN, KC_EQL),   // ; =
  [SLASH_MIN] = ACTION_TAP_DANCE_DOUBLE(KC_SLSH, KC_MINS),  // / -
  [COMMA_MIN] = ACTION_TAP_DANCE_DOUBLE(KC_COMM, KC_MINS),  // , -
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Del  |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;= |  '   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl |  FN  | Alt  | GUI  |Lower|    Space     |Raise |  Up  | Left | Down |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = LAYOUT_preonic_grid(
  KC_ESC,     KC_1,    KC_2,    KC_3,    KC_4,    KC_5,      KC_6,     KC_7,   KC_8,    KC_9,    KC_0,        KC_BSLS,
  KC_GRV,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,      KC_Y,     KC_U,   KC_I,    KC_O,    KC_P,        KC_BSPC,
  KC_TAB,     KC_A,    KC_S,    KC_D,    KC_F,    KC_G,      KC_H,     KC_J,   KC_K,    KC_L,    TD(SEMI_EQ), KC_QUOT,
  KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,      KC_N,     KC_M,   KC_COMM, KC_DOT,  KC_SLSH,     KC_SFTENT,
  KC_LCTL,    FN,      KC_LALT, KC_LGUI, LOWER,   SPACE_FN,  SPACE_FN, RAISE,  KC_LEFT, KC_DOWN, KC_UP,       KC_RIGHT
),

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |-----------------------------------------------------------------------------------|
 * |   ~  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |  )   |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |   ~  |   <  |   >  |   =  |   &  |   |  |   {  |  [   |   ]  |  }   |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |   %  |   _  |   +  |   -  |   *  |   ^  |   (  |  )   |      |  \   |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      | Home | PgUp | PgDn | End  |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_preonic_grid(
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
  KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_LPRN, KC_RPRN, KC_AMPR, KC_PIPE, _______,
  _______, KC_UNDS, KC_LT,   KC_GT,   KC_EQL,  KC_CIRC, KC_ASTR, KC_LCBR, KC_RCBR, _______, _______, _______,
  _______, _______, KC_PERC, KC_PLUS, KC_MINS, _______, _______, KC_LBRC, KC_RBRC, _______, KC_BSLS, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END
),
// KC_AMPR, KC_ASTR,
/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |      |      |      |      |      |      |      |      |      |      |      |  Del |
 * |-----------------------------------------------------------------------------------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |   =  |      |      | Prev | Vol- | Vol+ | Next |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |   -  |      |      | Play | Mute |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_preonic_grid(
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, KC_EQL,  _______, _______, KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, _______,
  _______, _______, _______, _______, KC_MINS, _______, _______, KC_MPLY, KC_MUTE, _______, _______, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END
),


/* FN 
 * ,-----------------------------------------------------------------------------------.
 * |foQuit| Reset|Debug | RGB  |RGBMOD| HUE+ | HUE- | SAT+ | SAT- |BRGTH+|BRGTH-|RESET |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_FN] = LAYOUT_preonic_grid(
  _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,    KC_F5,    KC_F6,    KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_DEL,
  _______, KC_F11,  KC_F12,  _______, _______, _______, _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),
  /* Space FN
  * ,-----------------------------------------------------------------------------------.
  * |KC_ESC |      |      |Lclick|Rclick|      |      |      | Bri- | Bri+ |      |      |
  * |-------+------+------+------+------+-------------+------+------+------+------+------|
  * |       | LEFT |  UP  | Down |RIGHT |      |      |SCL_LT|SCL_DN|SCL_UP|SCL_RT|      |
  * |-------+------+------+------+------+------|------+------+------+------+------+------|
  * |       | Prev | Vol- | Vol+ | Next | Mute | Play |      |      |      |      |      |
  * |-------+------+------+------+------+------+------+------+------+------+------+------|
  * |       |      |      |      |      |             |      |      |      |      |      |
  * `-----------------------------------------------------------------------------------'
  */
  [_SPACE_FN] = LAYOUT_preonic_grid(
      KC_ESC,  XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
      XXXXXXX, XXXXXXX, XXXXXXX,  KC_BTN1, KC_BTN2, XXXXXXX, XXXXXXX, XXXXXXX, KC_BRID, KC_BRIU, XXXXXXX, XXXXXXX,
      XXXXXXX, KC_MS_L, KC_MS_D,  KC_MS_U, KC_MS_R, XXXXXXX, XXXXXXX, KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R, XXXXXXX,
      XXXXXXX, KC_MPRV, KC_VOLD,  KC_VOLU, KC_MNXT, KC_MUTE, KC_MPLY, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
      XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
  ),


/* Adjust (Lower + Raise)
 * ,-----------------------------------------------------------------------------------.
 * |foQuit| Reset|Debug | RGB  |RGBMOD| HUE+ | HUE- | SAT+ | SAT- |BRGTH+|BRGTH-|RESET |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = LAYOUT_preonic_grid(
  MACOS_FORCE_QUIT, _______, DEBUG,   RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD,  RGB_VAI, RGB_VAD, RESET,
  _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
  _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
  _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
  _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
)


};
/* Template
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Del  |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Brite| Ctrl | Alt  | GUI  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
        case QWERTY:
          if (record->event.pressed) {
            set_single_persistent_default_layer(_QWERTY);
          }
          return false;
          break;
        case MACOS_FORCE_QUIT:
          if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_LGUI);
            tap_code(KC_ESC);
            unregister_code(KC_LALT);
            unregister_code(KC_LGUI);
          }
          return false;
          break;
        case LOWER:
          if (record->event.pressed) {
            layer_on(_LOWER);
            update_tri_layer(_LOWER, _RAISE, _ADJUST);
          } else {
            layer_off(_LOWER);
            update_tri_layer(_LOWER, _RAISE, _ADJUST);
          }
          return false;
          break;
        case RAISE:
          if (record->event.pressed) {
            layer_on(_RAISE);
            update_tri_layer(_LOWER, _RAISE, _ADJUST);
          } else {
            layer_off(_RAISE);
            update_tri_layer(_LOWER, _RAISE, _ADJUST);
          }
          return false;
          break;
        case BACKLIT:
          if (record->event.pressed) {
            register_code(KC_RSFT);
            #ifdef BACKLIGHT_ENABLE
              backlight_step();
            #endif
            #ifdef __AVR__
            writePinLow(E6);
            #endif
          } else {
            unregister_code(KC_RSFT);
            #ifdef __AVR__
            writePinHigh(E6);
            #endif
          }
          return false;
          break;
      }
    return true;
};

bool muse_mode = false;
uint8_t last_muse_note = 0;
uint16_t muse_counter = 0;
uint8_t muse_offset = 70;
uint16_t muse_tempo = 50;

void encoder_update_user(uint8_t index, bool clockwise) {
  if (muse_mode) {
    if (IS_LAYER_ON(_RAISE)) {
      if (clockwise) {
        muse_offset++;
      } else {
        muse_offset--;
      }
    } else {
      if (clockwise) {
        muse_tempo+=1;
      } else {
        muse_tempo-=1;
      }
    }
  } else {
    if (clockwise) {
      register_code(KC_PGDN);
      unregister_code(KC_PGDN);
    } else {
      register_code(KC_PGUP);
      unregister_code(KC_PGUP);
    }
  }
}

void dip_switch_update_user(uint8_t index, bool active) {
    switch (index) {
        case 0:
            if (active) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
            break;
        case 1:
            if (active) {
                muse_mode = true;
            } else {
                muse_mode = false;
            }
    }
}


void matrix_scan_user(void) {
#ifdef AUDIO_ENABLE
    if (muse_mode) {
        if (muse_counter == 0) {
            uint8_t muse_note = muse_offset + SCALE[muse_clock_pulse()];
            if (muse_note != last_muse_note) {
                stop_note(compute_freq_for_midi_note(last_muse_note));
                play_note(compute_freq_for_midi_note(muse_note), 0xF);
                last_muse_note = muse_note;
            }
        }
        muse_counter = (muse_counter + 1) % muse_tempo;
    } else {
        if (muse_counter) {
            stop_all_notes();
            muse_counter = 0;
        }
    }
#endif
}

bool music_mask_user(uint16_t keycode) {
  switch (keycode) {
    case RAISE:
    case LOWER:
      return false;
    default:
      return true;
  }
}
