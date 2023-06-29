TAP_DANCE_ENABLE = yes
UNICODEMAP_ENABLE = yes
ifeq ($(strip $(AUDIO_ENABLE)), yes)
    SRC += muse.c
endif
