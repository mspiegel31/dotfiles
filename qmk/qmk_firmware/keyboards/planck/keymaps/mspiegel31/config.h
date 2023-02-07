#pragma once
#define UNICODE_SELECTED_MODES UNICODE_MODE_LINUX

#ifdef AUDIO_ENABLE
    #define STARTUP_SONG SONG(PLANCK_SOUND)
    // #define STARTUP_SONG SONG(NO_SOUND)

   //  #define DEFAULT_LAYER_SONGS { SONG(QWERTY_SOUND) }
   // more songs listed here: https://git.roxanne.dev/roxxers/qmk-custom-firmware/src/commit/b2175e18493a1e36d9762fb0e6397ba7bef20839/quantum/audio/song_list.h
   #undef MARIO_THEME
   #undef MARIO_MUSHROOM
   #undef COIN_SOUND
   #undef ONE_UP_SOUND
   #undef SONIC_RING
   #undef ZELDA_TREASURE
   #undef TERMINAL_SOUND

   #define MARIO_THEME \
    Q__NOTE(_E5), \
    H__NOTE(_E5), \
    H__NOTE(_E5), \
    Q__NOTE(_C5), \
    H__NOTE(_E5), \
    W__NOTE(_G5), \
    Q__NOTE(_G4),


   #define MARIO_MUSHROOM \
      S__NOTE(_C5 ), \
      S__NOTE(_G4 ), \
      S__NOTE(_C5 ), \
      S__NOTE(_E5 ), \
      S__NOTE(_G5 ), \
      S__NOTE(_C6 ), \
      S__NOTE(_G5 ), \
      S__NOTE(_GS4), \
      S__NOTE(_C5 ), \
      S__NOTE(_DS5), \
      S__NOTE(_GS5), \
      S__NOTE(_DS5), \
      S__NOTE(_GS5), \
      S__NOTE(_C6 ), \
      S__NOTE(_DS6), \
      S__NOTE(_GS6), \
      S__NOTE(_DS6), \
      S__NOTE(_AS4), \
      S__NOTE(_D5 ), \
      S__NOTE(_F5 ), \
      S__NOTE(_AS5), \
      S__NOTE(_D6 ), \
      S__NOTE(_F6 ), \
      S__NOTE(_AS6), \
      S__NOTE(_F6 )

   #define COIN_SOUND \
    E__NOTE(_A5  ),   \
    HD_NOTE(_E6  ),

   #define ONE_UP_SOUND \
    Q__NOTE(_E6  ),  \
    Q__NOTE(_G6  ),  \
    Q__NOTE(_E7  ),  \
    Q__NOTE(_C7  ),  \
    Q__NOTE(_D7  ),  \
    Q__NOTE(_G7  ),

   #define SONIC_RING \
      E__NOTE(_E6),  \
      E__NOTE(_G6),  \
      HD_NOTE(_C7),

   #define ZELDA_TREASURE \
    Q__NOTE(_A4 ), \
    Q__NOTE(_AS4), \
    Q__NOTE(_B4 ), \
    HD_NOTE(_C5 ), \

   
   #define TERMINAL_SOUND \
      E__NOTE(_C5 )


#endif

/*
 * MIDI options
 */

/* enable basic MIDI features:
   - MIDI notes can be sent when in Music mode is on
*/

#define MIDI_BASIC

/* enable advanced MIDI features:
   - MIDI notes can be added to the keymap
   - Octave shift and transpose
   - Virtual sustain, portamento, and modulation wheel
   - etc.
*/
//#define MIDI_ADVANCED

/* override number of MIDI tone keycodes (each octave adds 12 keycodes and allocates 12 bytes) */
//#define MIDI_TONE_KEYCODE_OCTAVES 2

// Most tactile encoders have detents every 4 stages
#define ENCODER_RESOLUTION 4

