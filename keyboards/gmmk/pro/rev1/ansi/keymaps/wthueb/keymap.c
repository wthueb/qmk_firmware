#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
//      ESC                   F1       F2       F3       F4       F5       F6       F7       F8       F9        F10      F11      F12      PrtScr   Rotary(Mute)
//      ~                     1        2        3        4        5        6        7        8        9         0        -        =        BackSpc  Del
//      Tab                   Q        W        E        R        T        Y        U        I        O         P        [        ]        \        PgUp
//      Caps                  A        S        D        F        G        H        J        K        L         ;        '                 Enter    PgDn
//      Sh_L                  Z        X        C        V        B        N        M        ,        .         /                 Sh_R     Up       End
//      Ct_L                  Win_L    Alt_L                               SPACE                      Alt_R     FN       Ct_R     Left     Down     Right

    [0] = LAYOUT(
        KC_ESC,               KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,    KC_F10,  KC_F11,  KC_F12,  KC_PSCR, KC_MPLY,
        LT(1, KC_GRV),        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,     KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,
        KC_TAB,               KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,     KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_PGUP,
        MT(MOD_LCTL, KC_ESC), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,     KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGDN,
        KC_LSFT,              KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,   KC_SLSH,          KC_RSFT, KC_UP,   KC_END,
        KC_LCTL,              KC_LGUI, KC_LALT,                            KC_SPC,                    KC_RALT,  MO(1),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [1] = LAYOUT(
        KC_TRNS,              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MUTE,
        KC_TRNS,              KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,    KC_F10,  KC_F11,  KC_F12,  KC_TRNS, RGB_TOG,
        KC_TRNS,              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_PSCR, KC_HOME, KC_END,  QK_BOOT, RGB_MOD,
        KC_CAPS,              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, KC_PGDN, KC_PGUP, KC_TRNS, RGB_RMOD,
        KC_TRNS,              KC_TRNS, KC_TRNS, KC_TRNS, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU,  KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS,              KC_TRNS, KC_TRNS,                            KC_TRNS,                   KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),
};

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (IS_LAYER_ON(0))
    {
        if (clockwise)
            tap_code(KC_VOLU);
        else
            tap_code(KC_VOLD);
    }
    else if (IS_LAYER_ON(1))
    {
        if (clockwise)
            rgblight_increase_hue();
        else
            rgblight_decrease_hue();
    }

    return true;
}
#endif // ENCODER_ENABLE

#ifdef RGB_MATRIX_ENABLE

#define LED_FLAG_CAPS LED_FLAG_NONE

static void set_rgb_caps_leds(void);

bool led_update_user(led_t led_state) {
    if (led_state.caps_lock) {
        if (!rgb_matrix_is_enabled()) {
            rgb_matrix_set_flags(LED_FLAG_CAPS);
            rgb_matrix_enable();
        }
    } else if (rgb_matrix_get_flags() == LED_FLAG_CAPS) {
        rgb_matrix_set_flags(LED_FLAG_ALL);
        rgb_matrix_disable();
    }

    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case RGB_MOD:
        case RGB_RMOD:
        case RGB_HUI:
        case RGB_HUD:
        case RGB_SAI:
        case RGB_SAD:
        case RGB_VAI:
        case RGB_VAD:
        case RGB_SPI:
        case RGB_SPD:
            if (record->event.pressed) {
                if (rgb_matrix_get_flags() != LED_FLAG_ALL) {
                    return false;
                }
            }
            break;
        case RGB_TOG:
            if (record->event.pressed) {
                if (rgb_matrix_is_enabled()) {
                    switch (rgb_matrix_get_flags()) {
                        case LED_FLAG_CAPS:
                            rgb_matrix_set_flags(LED_FLAG_ALL);
                            rgb_matrix_disable_noeeprom();
                            break;
                        case LED_FLAG_ALL:
                            if (host_keyboard_led_state().caps_lock) {
                                rgb_matrix_set_flags(LED_FLAG_CAPS);
                                rgb_matrix_disable_noeeprom();
                            }
                            break;
                    }
                }
            }
            break;
    }
    return true;
}

bool rgb_matrix_indicators_user() {
    if (rgb_matrix_get_flags() == LED_FLAG_CAPS) {
        rgb_matrix_set_color_all(0x0, 0x0, 0x0);
    }

    if (host_keyboard_led_state().caps_lock) {
        set_rgb_caps_leds();
    }

    return false;
}

static void set_rgb_caps_leds() {
    rgb_matrix_set_color(67, 0xFF, 0x0, 0x0); // Left side LED 1
    rgb_matrix_set_color(68, 0xFF, 0x0, 0x0); // Right side LED 1
    rgb_matrix_set_color(70, 0xFF, 0x0, 0x0); // Left side LED 2
    rgb_matrix_set_color(71, 0xFF, 0x0, 0x0); // Right side LED 2
    rgb_matrix_set_color(73, 0xFF, 0x0, 0x0); // Left side LED 3
    rgb_matrix_set_color(74, 0xFF, 0x0, 0x0); // Right side LED 3
    rgb_matrix_set_color(76, 0xFF, 0x0, 0x0); // Left side LED 4
    rgb_matrix_set_color(77, 0xFF, 0x0, 0x0); // Right side LED 4
    rgb_matrix_set_color(80, 0xFF, 0x0, 0x0); // Left side LED 5
    rgb_matrix_set_color(81, 0xFF, 0x0, 0x0); // Right side LED 5
    rgb_matrix_set_color(83, 0xFF, 0x0, 0x0); // Left side LED 6
    rgb_matrix_set_color(84, 0xFF, 0x0, 0x0); // Right side LED 6
    rgb_matrix_set_color(87, 0xFF, 0x0, 0x0); // Left side LED 7
    rgb_matrix_set_color(88, 0xFF, 0x0, 0x0); // Right side LED 7
    rgb_matrix_set_color(91, 0xFF, 0x0, 0x0); // Left side LED 8
    rgb_matrix_set_color(92, 0xFF, 0x0, 0x0); // Right side LED 8
    rgb_matrix_set_color(3, 0xFF, 0x0, 0x0); // CAPS LED
}

#endif // RGB_MATRIX_ENABLE
