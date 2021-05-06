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
  _LOWER,
  _RAISE,
  _ADJUST,
  _FN,
  _SPACE_FN
};

enum planck_keycodes {
  QWERTY = SAFE_RANGE,
  BACKLIT,
  EXT_PLV
};

// Fillers to make layering more clear
#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define FN MO(_FN)
#define SPACE_FN LT(_SPACE_FN, KC_SPC)

// Tap Dance
enum {
  DBCBR,
  DBPRN,
  DBBRC,
  Q_OR_1,
  W_OR_2,
  E_OR_3,
  R_OR_4,
  T_OR_5,
  Y_OR_6,
  U_OR_7,
  I_OR_8,
  O_OR_9,
  P_OR_0,
};
qk_tap_dance_action_t tap_dance_actions[] = {
  [DBCBR] = ACTION_TAP_DANCE_DOUBLE(KC_LCBR, KC_RCBR),  // {}
  [DBPRN] = ACTION_TAP_DANCE_DOUBLE(KC_LPRN, KC_RPRN),  // ()
  [DBBRC] = ACTION_TAP_DANCE_DOUBLE(KC_LBRC, KC_RBRC),     // []
  [Q_OR_1] = ACTION_TAP_DANCE_DOUBLE(KC_Q, KC_1),
  [W_OR_2] = ACTION_TAP_DANCE_DOUBLE(KC_W, KC_2),
  [E_OR_3] = ACTION_TAP_DANCE_DOUBLE(KC_E, KC_3),
  [R_OR_4] = ACTION_TAP_DANCE_DOUBLE(KC_R, KC_4),
  [T_OR_5] = ACTION_TAP_DANCE_DOUBLE(KC_T, KC_5),
  [Y_OR_6] = ACTION_TAP_DANCE_DOUBLE(KC_Y, KC_6),
  [U_OR_7] = ACTION_TAP_DANCE_DOUBLE(KC_U, KC_7),
  [I_OR_8] = ACTION_TAP_DANCE_DOUBLE(KC_I, KC_8),
  [O_OR_9] = ACTION_TAP_DANCE_DOUBLE(KC_O, KC_9),
  [P_OR_0] = ACTION_TAP_DANCE_DOUBLE(KC_P, KC_0),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | `    |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Tab  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |  Up  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl |  FN  | Alt  | GUI  |Lower |    Space    |Raise | /    | Left | Down |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = LAYOUT_planck_grid(
    KC_GRV,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,      KC_Y,     KC_U,   KC_I,    KC_O,    KC_P,    KC_BSPC,
    KC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,      KC_H,     KC_J,   KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,      KC_N,     KC_M,   KC_COMM, KC_DOT,  KC_UP,   KC_ENT,
    KC_LCTL, FN,      KC_LALT, KC_LGUI, LOWER,   SPACE_FN,  SPACE_FN, RAISE,  KC_SLSH, KC_LEFT, KC_DOWN, KC_RIGHT
),


/* Lower
 * ,-----------------------------------------------------------------------------------.
 * | Esc  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Del  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   _  |   +  |   {  |   }  |  |   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |      |      |      | PgUp |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |  \   |      | PgDn |      |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_planck_grid(
    KC_ESC,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR,    KC_ASTR,    KC_LPRN, KC_RPRN, KC_BSPC,
    KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_UNDS,    KC_PLUS,    KC_LCBR, KC_RCBR, KC_PIPE,
    _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  TD(DBCBR),  TD(DBBRC),  _______, KC_PGUP, _______,
    _______, _______, _______, _______, _______, _______, _______, _______,    KC_BSLS,    _______, KC_PGDN, _______
),

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |   ~  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Del  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   -  |   =  |   [  |   ]  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |      |      | Mute | Vol+ | Play |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |  \   | Prev | Vol- | Next |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_planck_grid(
    KC_TILD,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
    KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, _______,
    _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, KC_MUTE, KC_VOLU, KC_MPLY,
    _______, _______, _______, _______, _______, _______, _______, _______, KC_BSLS, KC_MPRV, KC_VOLD, KC_MNXT
),

  /* fn
  * ,-----------------------------------------------------------------------------------.
  * |      |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F7  |  F8  |  F9  | F10  | Del  |
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
      XXXXXXX,        KC_F11,  KC_F12,  XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
      XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
      XXXXXXX,        _______, XXXXXXX, XXXXXXX, XXXXXXX,  KC_SPC,   KC_SPC,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
  ),


  /* Space fn
  * ,-----------------------------------------------------------------------------------.
  * |KC_ESC |      |  UP  |      |      |      |      |      |      |      |      |SLEEP |
  * |-------+------+------+------+------+-------------+------+------+------+------+------|
  * |       | LEFT | DOWN | RIGHT|      |      |      | LEFT | DOWN |  UP  | RIGHT|      |
  * |-------+------+------+------+------+------|------+------+------+------+------+------| 
  * |       |      |      |      |      |      |      |      |      |      |      |      |
  * |-------+------+------+------+------+------+------+------+------+------+------+------|
  * |       |      |      |      |      |             |      | MUTE |VOLDWN|VOL UP|      |
  * `-----------------------------------------------------------------------------------'
  */
  [_SPACE_FN] = LAYOUT_planck_grid(
      KC_ESC,  XXXXXXX, KC_UP,    XXXXXXX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX,  XXXXXXX,     XXXXXXX,    KC_SLEP, 
      XXXXXXX,  KC_LEFT, KC_DOWN,  KC_RIGHT,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_LEFT, KC_DOWN,  KC_UP,       KC_RIGHT,   XXXXXXX, 
      XXXXXXX,  XXXXXXX, XXXXXXX,  XXXXXXX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX,  XXXXXXX,     XXXXXXX,    XXXXXXX, 
      XXXXXXX,  XXXXXXX, XXXXXXX,  XXXXXXX,   XXXXXXX,  _______,  _______,  XXXXXXX, KC__MUTE, KC__VOLDOWN, KC__VOLUP,  XXXXXXX
  ),
\
/* Adjust (Lower + Raise)
 *                      v------------------------RGB CONTROL--------------------v
 * ,-----------------------------------------------------------------------------------.
 * |      | Reset|Debug | RGB  |RGBMOD| HUE+ | HUE- | SAT+ | SAT- |BRGTH+|BRGTH-|RESET |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |MUSmod|Aud on|Audoff|AGnorm|AGswap|Qwerty|      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |Voice-|Voice+|Mus on|Musoff|MIDIon|MIDIof|TermOn|TermOf|      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = LAYOUT_planck_grid(
    _______, RESET,   DEBUG,   RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD,  RGB_VAI, RGB_VAD, RESET ,
    _______, _______, MU_MOD,  AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, QWERTY,  _______,  _______,  _______,  _______,
    _______, MUV_DE,  MUV_IN,  MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  TERM_ON, TERM_OFF, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______
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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        print("mode just switched to qwerty and this is a huge string\n");
        set_single_persistent_default_layer(_QWERTY);
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

    case EXT_PLV:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(plover_gb_song);
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

void encoder_update(bool clockwise) {
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
}

void dip_switch_update_user(uint8_t index, bool active) {
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
