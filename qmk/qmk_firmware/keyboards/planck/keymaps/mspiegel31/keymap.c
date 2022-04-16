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


enum planck_layers {
  _QWERTY,
  _SHIFT,
  _GAME_MODE,
  _LOWER,
  _RAISE,
  _FN,
  _SPACE_FN,
  _ADJUST,
};

enum planck_keycodes {
  QWERTY = SAFE_RANGE,
  GAME_MODE,
  LOWER,
  RAISE,
  BACKLIT
};

// Fillers to make layering more clear
#define LOWER            MO(_LOWER)
#define RAISE            MO(_RAISE)
#define FN               MO(_FN)
#define SPACE_FN         LT(_SPACE_FN, KC_SPC)
#define TAB_RAISE        LT(_RAISE, KC_TAB)
#define SEMI_EQ          TD(_SEMI_EQ)
#define MACOS_FORCE_QUIT LALT(LGUI(KC_ESC))

enum {
  _DBCBR,
  _DBPRN,
  _DBBRC,
  _OBROBRK,
  _CBRCBRK,
  _SQUODQUO,
  _SEMI_EQ,
  _SLASH_MIN,
  _COMMA_MIN,
  _CAPS_LOCK,
  _SHIFT_LAYER,
  GRV_OR_TILD,
};
qk_tap_dance_action_t tap_dance_actions[] = {
  [_DBCBR]     = ACTION_TAP_DANCE_DOUBLE(KC_LCBR, KC_RCBR),  // {}
  [_DBPRN]     = ACTION_TAP_DANCE_DOUBLE(KC_LPRN, KC_RPRN),  // ()
  [_DBBRC]     = ACTION_TAP_DANCE_DOUBLE(KC_LBRC, KC_RBRC),  // []
  [_OBROBRK]   = ACTION_TAP_DANCE_DOUBLE(KC_LBRC, KC_LCBR),  // [{
  [_CBRCBRK]   = ACTION_TAP_DANCE_DOUBLE(KC_RBRC, KC_RCBR),  // ]}
  [_SQUODQUO]  = ACTION_TAP_DANCE_DOUBLE(KC_QUOT, KC_DQUO),  // ' "
  [_SEMI_EQ]   = ACTION_TAP_DANCE_DOUBLE(KC_SCLN, KC_EQL),   // ; =
  [_SLASH_MIN] = ACTION_TAP_DANCE_DOUBLE(KC_SLSH, KC_MINS),  // / -
  [_COMMA_MIN] = ACTION_TAP_DANCE_DOUBLE(KC_COMM, KC_MINS),  // , -
  [_SHIFT_LAYER] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_LSFT, _SHIFT)

};


// https://beta.docs.qmk.fm/using-qmk/software-features/feature_unicodeqw
enum unicode_names {
    BANG,
    IRONY,
    SNEK
};

const uint32_t PROGMEM unicode_map[] = {
    [BANG]  = 0x203D,  // ‽
    [IRONY] = 0x2E2E,  // ⸮
    [SNEK]  = 0x1F40D, // 🐍
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | `    |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Tab  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |  ;=  |  '   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,- |   .  |  /   |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl |  FN  | Alt  | GUI  |Lower|    Space     |Raise |  Up  | Left | Down  |Right |
 * `-----------------------------------------------------------------------------------`
 */
[_QWERTY] = LAYOUT_planck_grid(
    KC_GRV,         KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,      KC_Y,     KC_U,   KC_I,    KC_O,    KC_P,    KC_BSPC,
    KC_TAB,         KC_A,    KC_S,    KC_D,    KC_F,    KC_G,      KC_H,     KC_J,   KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    TD(_SHIFT_LAYER), KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,      KC_N,     KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_SFTENT,
    KC_LCTL,        FN,      KC_LALT, KC_LGUI, RAISE,   KC_SPC,    KC_SPC,   LOWER,  KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT
),

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | `    |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Tab  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |  ;=  |  '   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,- |   .  |  /   |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl |  FN  | Alt  | GUI  |Lower|    Space     |Raise |  Up  | Left | Down  |Right |
 * `-----------------------------------------------------------------------------------`
 */
[_SHIFT] = LAYOUT_planck_grid(
    S(KC_GRV),        S(KC_Q),    S(KC_W),    S(KC_E),    S(KC_R),    S(KC_T),  S(KC_Y), S(KC_U),  S(KC_I),    S(KC_O),   S(KC_P),     _______,
    _______,          S(KC_A),    S(KC_S),    S(KC_D),    S(KC_F),    S(KC_G),  S(KC_H), S(KC_J),  S(KC_K),    S(KC_L),   S(KC_SCLN),  _______,
    TD(_SHIFT_LAYER), S(KC_Z),    S(KC_X),    S(KC_C),    S(KC_V),    S(KC_B),  S(KC_N), S(KC_M),  S(KC_COMM), S(KC_DOT), S(KC_SLSH),  _______,
    _______,          _______,    _______,    _______,    _______,    KC_UNDS,   KC_UNDS,  _______,  _______,    _______,   _______,   _______
),


/* Game Mode
 * ,-----------------------------------------------------------------------------------.
 * | `    |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Tab  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |  ;=  |  '   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,- |   .  |  /   |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl |  FN  | Alt  | GUI  |Lower|    Space     |Raise |  Up  | Left | Down  |Right |
 * `-----------------------------------------------------------------------------------`
 */
[_GAME_MODE] = LAYOUT_planck_grid(
    KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,      KC_Y,     KC_U,   KC_I,    KC_O,    KC_P,    KC_BSPC,
    KC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,      KC_H,     KC_J,   KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,      KC_N,     KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_SFTENT,
    KC_LCTL, KC_1,    KC_2,    KC_3,    RAISE,   KC_SPC,    KC_SPC,   LOWER,  KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT
),

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |   ~  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |  *   |   (  |  )   | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |   ~  |   <  |   >  |   =  |   &  |   |  |   {  |  [   |   ]  |  }   |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |   %  |   _  |   +  |   -  |   *  |   ^  |   (  |  )   |      |  \   |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      | Home | PgUp | PgDn | End  |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_planck_grid(
    KC_TILD,  KC_1,   KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
    _______, _______, KC_LT,   KC_GT,   KC_EQL,  KC_PIPE, KC_AMPR, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_BSLS,
    _______, _______, _______, KC_PLUS, KC_MINS, _______, _______, KC_F5,   _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END
),

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * | Esc  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |   4  |   5  |   6  |   +  |   \  |      | Prev | Vol- | Vol+ | Next |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |   7  |   8  |   9  |   -  |   *  |      | Mute | Play |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_planck_grid(
  KC_ESC,     KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
  C(KC_GRV), _______,  _______, KC_COLN, KC_EQL,  KC_PIPE, KC_AMPR, KC_LCBR, KC_LBRC, KC_RBRC, KC_RCBR, KC_BSLS,
  _______,   C(KC_B),  KC_PERC, KC_UNDS, KC_MINS, KC_PLUS, _______, KC_LPRN, KC_RPRN, _______, _______, _______,
  _______,   _______,  _______, _______, _______, KC_UNDS, KC_UNDS, _______, _______, _______, _______, _______
),


  /* FN
  * ,-----------------------------------------------------------------------------------.
  * |      |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F7  |  F8  |  F9  | F10  |  Del |
  * |------+------+------+------+------+-------------+------+------+------+------+------|
  * |      |  F11 |  F12 |      |      |      |      |      |      |      |      |      |
  * |------+------+------+------+------+------|------+------+------+------+------+------|
  * |      |      |      |      |      |      |      |      |      |      |      | Enter|
  * |------+------+------+------+------+------+------+------+------+------+------+------|
  * |      |      |      |      |      |    Space    |      |      |      |      |      |
  * `-----------------------------------------------------------------------------------'
  */
  [_FN] = LAYOUT_planck_grid(
      XXXXXXX,  KC_F1,   KC_F2,   KC_F3,   KC_F4,    KC_F5,    KC_F6,    KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_DEL,
      XXXXXXX,  KC_F11,  KC_F12,  XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
      XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
      XXXXXXX,  _______, XXXXXXX, XXXXXXX, XXXXXXX,  KC_SPC,   KC_SPC,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
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
  [_SPACE_FN] = LAYOUT_planck_grid(
      XXXXXXX, XXXXXXX, KC_BTN2,  KC_MS_U,   KC_BTN1,  XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,
      XXXXXXX, XXXXXXX, KC_MS_L,  KC_MS_D,   KC_MS_R,  XXXXXXX,  XXXXXXX,  KC_WH_L, KC_WH_D, KC_WH_U,  KC_WH_R, XXXXXXX,
      XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,
      XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,   XXXXXXX,  _______,  _______,  XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX
  ),

/* Adjust (Lower + Raise)
 *                      v------------------------RGB CONTROL--------------------v
 * ,-----------------------------------------------------------------------------------.
 * |foQuit| Reset|Debug | RGB  |RGBMOD| HUE+ | HUE- | SAT+ | SAT- |BRGTH+|BRGTH-|RESET |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = LAYOUT_planck_grid(
    MACOS_FORCE_QUIT, _______, KC_BTN2, KC_MS_U, KC_BTN1, _______, _______,   _______, KC_BRID, KC_BRIU, _______,  RESET,
    _______,          _______, KC_MS_L, KC_MS_D, KC_MS_R, QWERTY,  GAME_MODE, KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, _______,
    _______,          KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R, _______, _______,   KC_MPLY, KC_MUTE, _______, _______, RGB_TOG,
    _______,          _______, _______, _______, _______, _______, _______,   _______, RGB_SAD, RGB_HUD, RGB_HUI,  RGB_SAI
)

};
/* Template
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Brite| Ctrl | Alt  | GUI  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */

#ifdef AUDIO_ENABLE
  float plover_song[][2]     = SONG(PLOVER_SOUND);
  float plover_gb_song[][2]  = SONG(PLOVER_GOODBYE_SOUND);
#endif

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

float qwerty_song[][2] = SONG(QWERTY_SOUND);
float sonic_ring[][2] = SONG(SONIC_RING);

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        PLAY_SONG(qwerty_song);
        set_single_persistent_default_layer(_QWERTY);
      }
      return false;
      break;

    case GAME_MODE:
      if (record->event.pressed) {
        PLAY_SONG(sonic_ring);
        set_single_persistent_default_layer(_GAME_MODE);
      }
      return false;
      break;

    case BACKLIT:
      if (record->event.pressed) {
        register_code(KC_RSFT);
        #ifdef BACKLIGHT_ENABLE
          backlight_step();
        #endif
        #ifdef KEYBOARD_planck_rev5
          writePinLow(E6);
        #endif
      } else {
        unregister_code(KC_RSFT);
        #ifdef KEYBOARD_planck_rev5
          writePinHigh(E6);
        #endif
      }
      return false;
      break;
  }
  return true;
}

bool muse_mode = false;
uint8_t last_muse_note = 0;
uint16_t muse_counter = 0;
uint8_t muse_offset = 70;
uint16_t muse_tempo = 50;

bool encoder_update(bool clockwise) {
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
      #ifdef MOUSEKEY_ENABLE
        tap_code(KC_MS_WH_DOWN);
      #else
        tap_code(KC_PGDN);
      #endif
    } else {
      #ifdef MOUSEKEY_ENABLE
        tap_code(KC_MS_WH_UP);
      #else
        tap_code(KC_PGUP);
      #endif
    }
  }
    return true;
}

bool dip_switch_update_user(uint8_t index, bool active) {
    switch (index) {
        case 0: {
#ifdef AUDIO_ENABLE
            static bool play_sound = false;
#endif
            if (active) {
#ifdef AUDIO_ENABLE
                if (play_sound) { PLAY_SONG(plover_song); }
#endif
                layer_on(_ADJUST);
            } else {
#ifdef AUDIO_ENABLE
                if (play_sound) { PLAY_SONG(plover_gb_song); }
#endif
                layer_off(_ADJUST);
            }
#ifdef AUDIO_ENABLE
            play_sound = true;
#endif
            break;
        }
        case 1:
            if (active) {
                muse_mode = true;
            } else {
                muse_mode = false;
            }
    }
    return true;
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
