/*
 * Copyright (c) 2026 thoughtfix
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_behavior_vol_shift

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zephyr/logging/log.h>
#include <zmk/behavior.h>
#include <zmk/hid.h>
#include <zmk/endpoints.h>
#include <dt-bindings/zmk/hid_usage_pages.h>
#include <dt-bindings/zmk/hid_usage.h>
#include <dt-bindings/zmk/modifiers.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

// Volume/Mute fix (see NOTES.md): Speaker key alone = Volume Down;
// Shift+Speaker = Volume Up, matching ClusterM's factory-spec
// do_the_key() behavior. A plain zmk,behavior-mod-morph can pick which
// *code* to send based on Shift state, but Consumer-page reports have no
// modifier byte, so Shift's own physical key keeps reporting held the
// whole time regardless, which stops strict WM-level keybind matching
// (e.g. labwc's unmodified XF86_AudioRaiseVolume bind) from ever firing.
// This behavior explicitly
// unregisters the Shift bit for the duration of the press and restores it
// on release, mirroring what hid_listener.c does internally for ordinary
// modifier handling.

struct behavior_vol_shift_data {
    zmk_mod_flags_t suppressed_mods;
    bool sent_vol_up;
};

static struct behavior_vol_shift_data vol_shift_data = {0};

static int on_vol_shift_pressed(struct zmk_behavior_binding *binding,
                                 struct zmk_behavior_binding_event event) {
    zmk_mod_flags_t shift_mods = zmk_hid_get_explicit_mods() & (MOD_LSFT | MOD_RSFT);

    if (shift_mods) {
        zmk_hid_unregister_mods(shift_mods);
        zmk_endpoints_send_report(HID_USAGE_KEY);
        vol_shift_data.suppressed_mods = shift_mods;
        vol_shift_data.sent_vol_up = true;
        zmk_hid_consumer_press(HID_USAGE_CONSUMER_VOLUME_INCREMENT);
    } else {
        vol_shift_data.suppressed_mods = 0;
        vol_shift_data.sent_vol_up = false;
        zmk_hid_consumer_press(HID_USAGE_CONSUMER_VOLUME_DECREMENT);
    }

    return zmk_endpoints_send_report(HID_USAGE_CONSUMER);
}

static int on_vol_shift_released(struct zmk_behavior_binding *binding,
                                  struct zmk_behavior_binding_event event) {
    zmk_hid_consumer_release(vol_shift_data.sent_vol_up ? HID_USAGE_CONSUMER_VOLUME_INCREMENT
                                                         : HID_USAGE_CONSUMER_VOLUME_DECREMENT);
    zmk_endpoints_send_report(HID_USAGE_CONSUMER);

    if (vol_shift_data.suppressed_mods) {
        zmk_hid_register_mods(vol_shift_data.suppressed_mods);
        zmk_endpoints_send_report(HID_USAGE_KEY);
        vol_shift_data.suppressed_mods = 0;
    }

    return 0;
}

static const struct behavior_driver_api behavior_vol_shift_driver_api = {
    .binding_pressed = on_vol_shift_pressed,
    .binding_released = on_vol_shift_released,
};

static int behavior_vol_shift_init(const struct device *dev) { return 0; }

#define VS_INST(n)                                                                                \
    BEHAVIOR_DT_INST_DEFINE(n, behavior_vol_shift_init, NULL, NULL, NULL, POST_KERNEL,            \
                             CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &behavior_vol_shift_driver_api);

DT_INST_FOREACH_STATUS_OKAY(VS_INST)

#endif
